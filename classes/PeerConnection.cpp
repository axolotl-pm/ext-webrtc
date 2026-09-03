extern "C" {
#include "php.h"
#include "Zend/zend_exceptions.h"
#include "../stubs/PeerConnection_arginfo.h"
}

#include "PeerConnection.h"
#include "PeerConnectionOptions.h"
#include "DataChannel.h"
#include "DataChannelOptions.h"
#include "IceCandidate.h"
#include "Enums.h"
#include "WebRtcException.h"

#include <string>
#include <utility>
#include <vector>

static zend_object_handlers peer_connection_zend_object_handlers;
zend_class_entry* peer_connection_ce;

static zend_object* peer_connection_new(zend_class_entry* ce) {
	auto object = alloc_custom_zend_object<peer_connection_zend_object>(ce, &peer_connection_zend_object_handlers);

	object->connection = NULL;
	object->lock = NULL;
	object->shared = NULL;

	object->lock = new std::mutex();
	object->shared = new std::shared_ptr(std::make_shared<peer_connection_shared>());

	return &object->std;
}

static void peer_connection_free(zend_object* std) {
	auto object = fetch_from_zend_object<peer_connection_zend_object>(std);

	if (object->connection != NULL) {
		try {
			object->connection->close();
		} catch (...) {
		}
		delete object->connection;
		object->connection = NULL;
	}

	delete object->shared;
	object->shared = NULL;

	delete object->lock;
	object->lock = NULL;
	zend_object_std_dtor(&object->std);
}

#define REQUIRE_CONNECTION(object) \
	do { \
		if ((object)->connection == NULL) { \
			zend_throw_exception(webrtc_exception_ce, "PeerConnection is closed", 0); \
			RETURN_THROWS(); \
		} \
	} while (0)

#define PEER_CONNECTION_METHOD(name) PHP_METHOD(pmmp_webrtc_PeerConnection, name)

PEER_CONNECTION_METHOD(__construct) {
	zval* options_zval;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
		Z_PARAM_OBJECT_OF_CLASS(options_zval, peer_connection_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	auto object = PEER_CONNECTION_THIS();
	auto options = OPTIONS_FROM_ZVAL(options_zval);

	std::lock_guard guard(*object->lock);

	if (object->connection != NULL) {
		zend_throw_exception(webrtc_exception_ce, "PeerConnection is already constructed", 0);
		RETURN_THROWS();
	}

	WEBRTC_TRY
		object->connection = new rtc::PeerConnection(*options->config);

		/*
		 * A state of its own rather than the one already there: close() clears
		 * accepting for good, so constructing over a closed connection would
		 * otherwise inherit it and quietly drop everything the callbacks queue.
		 */
		auto shared = std::make_shared<peer_connection_shared>();
		shared->max_pending_channels = options->max_pending_data_channels;
		shared->receive_budget = std::make_shared<data_channel_budget>();
		shared->receive_budget->max = options->max_receive_queue;
		*object->shared = shared;

		object->connection->onGatheringStateChange([shared](rtc::PeerConnection::GatheringState state) {
			try {
				std::lock_guard guard(shared->lock);
				shared->gathering_state = state;
			} catch (...) {
			}
		});

		object->connection->onLocalCandidate([shared](rtc::Candidate candidate) {
			try {
				std::lock_guard guard(shared->lock);
				if (shared->accepting) {
					shared->pending_candidates.push_back(std::move(candidate));
				}
			} catch (...) {
			}
		});

		object->connection->onDataChannel([shared](std::shared_ptr<rtc::DataChannel> channel) {
			try {
				auto state = data_channel_attach(channel, shared->receive_budget);

				std::lock_guard guard(shared->lock);
				if (shared->accepting && (shared->max_pending_channels == 0 || shared->pending_channels.size() < shared->max_pending_channels)) {
					shared->pending_channels.emplace_back(std::move(channel), std::move(state));
				}
			} catch (...) {
			}
		});
	WEBRTC_CATCH
}

PEER_CONNECTION_METHOD(setRemoteAnswer) {
	zend_string* sdp;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
		Z_PARAM_STR(sdp)
	ZEND_PARSE_PARAMETERS_END();

	auto object = PEER_CONNECTION_THIS();

	std::lock_guard guard(*object->lock);
	REQUIRE_CONNECTION(object);

	WEBRTC_TRY
		object->connection->setRemoteDescription(
			rtc::Description(std::string(ZSTR_VAL(sdp), ZSTR_LEN(sdp)), "answer")
		);
	WEBRTC_CATCH
}

PEER_CONNECTION_METHOD(createDataChannel) {
	zend_string* label;
	zval* options_zval = NULL;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 2)
		Z_PARAM_STR(label)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(options_zval, data_channel_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	auto object = PEER_CONNECTION_THIS();

	std::lock_guard guard(*object->lock);
	REQUIRE_CONNECTION(object);

	WEBRTC_TRY
		rtc::DataChannelInit init;
		if (options_zval != NULL) {
			init = *DATA_CHANNEL_OPTIONS_FROM_ZVAL(options_zval)->init;
		}

		auto shared = *object->shared;
		auto channel = object->connection->createDataChannel(
			std::string(ZSTR_VAL(label), ZSTR_LEN(label)),
			init
		);
		auto state = data_channel_attach(channel, shared->receive_budget);

		data_channel_create_zval(return_value, std::move(channel), std::move(state));
	WEBRTC_CATCH
}

PEER_CONNECTION_METHOD(pollDataChannels) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = PEER_CONNECTION_THIS();

	std::vector<std::pair<std::shared_ptr<rtc::DataChannel>, std::shared_ptr<data_channel_shared>>> taken;

	WEBRTC_TRY
		auto shared = *object->shared;
		std::lock_guard guard(shared->lock);
		taken.swap(shared->pending_channels);
	WEBRTC_CATCH

	array_init_size(return_value, static_cast<uint32_t>(taken.size()));

	WEBRTC_TRY
		for (auto& entry_pair : taken) {
			zval entry;
			data_channel_create_zval(&entry, std::move(entry_pair.first), std::move(entry_pair.second));
			add_next_index_zval(return_value, &entry);
		}
	WEBRTC_CATCH
}

PEER_CONNECTION_METHOD(pollLocalCandidates) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = PEER_CONNECTION_THIS();

	std::vector<rtc::Candidate> taken;

	WEBRTC_TRY
		auto shared = *object->shared;
		std::lock_guard guard(shared->lock);
		taken.swap(shared->pending_candidates);
	WEBRTC_CATCH

	array_init_size(return_value, static_cast<uint32_t>(taken.size()));

	WEBRTC_TRY
		for (const auto& candidate : taken) {
			zval entry;
			ice_candidate_create_zval(&entry, candidate);
			add_next_index_zval(return_value, &entry);
		}
	WEBRTC_CATCH
}

PEER_CONNECTION_METHOD(addRemoteCandidate) {
	zval* candidate_zval;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
		Z_PARAM_OBJECT_OF_CLASS(candidate_zval, ice_candidate_ce)
	ZEND_PARSE_PARAMETERS_END();

	auto object = PEER_CONNECTION_THIS();
	auto candidate = ICE_CANDIDATE_FROM_ZVAL(candidate_zval);

	if (candidate->candidate == NULL) {
		zend_throw_exception(webrtc_exception_ce, "IceCandidate is not initialized", 0);
		RETURN_THROWS();
	}

	std::lock_guard guard(*object->lock);
	REQUIRE_CONNECTION(object);

	WEBRTC_TRY
		object->connection->addRemoteCandidate(*candidate->candidate);
	WEBRTC_CATCH
}

PEER_CONNECTION_METHOD(setRemoteOffer) {
	zend_string* sdp;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
		Z_PARAM_STR(sdp)
	ZEND_PARSE_PARAMETERS_END();

	auto object = PEER_CONNECTION_THIS();

	std::lock_guard guard(*object->lock);
	REQUIRE_CONNECTION(object);

	WEBRTC_TRY
		object->connection->setRemoteDescription(
			rtc::Description(std::string(ZSTR_VAL(sdp), ZSTR_LEN(sdp)), "offer")
		);
	WEBRTC_CATCH
}

PEER_CONNECTION_METHOD(getGatheringState) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = PEER_CONNECTION_THIS();

	std::lock_guard guard(*object->lock);
	REQUIRE_CONNECTION(object);

	WEBRTC_TRY
		/* the callbacks' view, not the connection's; see peer_connection_shared */
		auto shared = *object->shared;
		rtc::PeerConnection::GatheringState state;
		{
			std::lock_guard shared_guard(shared->lock);
			state = shared->gathering_state;
		}

		if (!webrtc_set_enum(return_value, gathering_state_ce, static_cast<zend_long>(state))) {
			RETURN_THROWS();
		}
	WEBRTC_CATCH
}

PEER_CONNECTION_METHOD(getState) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = PEER_CONNECTION_THIS();

	std::lock_guard guard(*object->lock);
	REQUIRE_CONNECTION(object);

	WEBRTC_TRY
		if (!webrtc_set_enum(return_value, connection_state_ce, static_cast<zend_long>(object->connection->state()))) {
			RETURN_THROWS();
		}
	WEBRTC_CATCH
}

PEER_CONNECTION_METHOD(getSignalingState) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = PEER_CONNECTION_THIS();

	std::lock_guard guard(*object->lock);
	REQUIRE_CONNECTION(object);

	WEBRTC_TRY
		if (!webrtc_set_enum(return_value, signaling_state_ce, static_cast<zend_long>(object->connection->signalingState()))) {
			RETURN_THROWS();
		}
	WEBRTC_CATCH
}

PEER_CONNECTION_METHOD(isNegotiationNeeded) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = PEER_CONNECTION_THIS();

	std::lock_guard guard(*object->lock);
	REQUIRE_CONNECTION(object);

	WEBRTC_TRY
		RETURN_BOOL(object->connection->negotiationNeeded());
	WEBRTC_CATCH
}

PEER_CONNECTION_METHOD(getRemoteDescription) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = PEER_CONNECTION_THIS();

	std::lock_guard guard(*object->lock);
	REQUIRE_CONNECTION(object);

	WEBRTC_TRY
		auto description = object->connection->remoteDescription();
		if (!description.has_value()) {
			RETURN_NULL();
		}

		std::string sdp = description->generateSdp("\r\n");
		RETURN_STRINGL(sdp.c_str(), sdp.size());
	WEBRTC_CATCH
}

PEER_CONNECTION_METHOD(getRemoteFingerprint) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = PEER_CONNECTION_THIS();

	std::lock_guard guard(*object->lock);
	REQUIRE_CONNECTION(object);

	WEBRTC_TRY
		/* an absent fingerprint comes back as an empty value rather than a throw */
		auto fingerprint = object->connection->remoteFingerprint();
		if (fingerprint.value.empty()) {
			RETURN_NULL();
		}
		RETURN_STRINGL(fingerprint.value.c_str(), fingerprint.value.size());
	WEBRTC_CATCH
}

PEER_CONNECTION_METHOD(getLocalDescription) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = PEER_CONNECTION_THIS();

	std::lock_guard guard(*object->lock);
	REQUIRE_CONNECTION(object);

	WEBRTC_TRY
		auto description = object->connection->localDescription();
		if (!description.has_value()) {
			RETURN_NULL();
		}

		std::string sdp = description->generateSdp("\r\n");
		RETURN_STRINGL(sdp.c_str(), sdp.size());
	WEBRTC_CATCH
}

PEER_CONNECTION_METHOD(getLocalFingerprint) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = PEER_CONNECTION_THIS();

	std::lock_guard guard(*object->lock);
	REQUIRE_CONNECTION(object);

	WEBRTC_TRY
		auto description = object->connection->localDescription();
		if (!description.has_value()) {
			RETURN_NULL();
		}

		auto fingerprint = description->fingerprint();
		if (!fingerprint.has_value()) {
			RETURN_NULL();
		}

		RETURN_STRINGL(fingerprint->value.c_str(), fingerprint->value.size());
	WEBRTC_CATCH
}

PEER_CONNECTION_METHOD(getLocalAddress) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = PEER_CONNECTION_THIS();

	std::lock_guard guard(*object->lock);
	REQUIRE_CONNECTION(object);

	WEBRTC_TRY
		auto address = object->connection->localAddress();
		if (!address.has_value()) {
			RETURN_NULL();
		}
		RETURN_STRINGL(address->c_str(), address->size());
	WEBRTC_CATCH
}

PEER_CONNECTION_METHOD(getRemoteAddress) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = PEER_CONNECTION_THIS();

	std::lock_guard guard(*object->lock);
	REQUIRE_CONNECTION(object);

	WEBRTC_TRY
		auto address = object->connection->remoteAddress();
		if (!address.has_value()) {
			RETURN_NULL();
		}
		RETURN_STRINGL(address->c_str(), address->size());
	WEBRTC_CATCH
}

PEER_CONNECTION_METHOD(getRoundTripTime) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = PEER_CONNECTION_THIS();

	std::lock_guard guard(*object->lock);
	REQUIRE_CONNECTION(object);

	WEBRTC_TRY
		auto rtt = object->connection->rtt();
		if (!rtt.has_value()) {
			RETURN_NULL();
		}
		RETURN_LONG(static_cast<zend_long>(rtt->count()));
	WEBRTC_CATCH
}

PEER_CONNECTION_METHOD(getBytesSent) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = PEER_CONNECTION_THIS();

	std::lock_guard guard(*object->lock);
	REQUIRE_CONNECTION(object);

	WEBRTC_TRY
		RETURN_LONG(static_cast<zend_long>(object->connection->bytesSent()));
	WEBRTC_CATCH
}

PEER_CONNECTION_METHOD(getBytesReceived) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = PEER_CONNECTION_THIS();

	std::lock_guard guard(*object->lock);
	REQUIRE_CONNECTION(object);

	WEBRTC_TRY
		RETURN_LONG(static_cast<zend_long>(object->connection->bytesReceived()));
	WEBRTC_CATCH
}

PEER_CONNECTION_METHOD(clearStats) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = PEER_CONNECTION_THIS();

	std::lock_guard guard(*object->lock);
	REQUIRE_CONNECTION(object);

	WEBRTC_TRY
		object->connection->clearStats();
	WEBRTC_CATCH
}

PEER_CONNECTION_METHOD(close) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = PEER_CONNECTION_THIS();

	std::lock_guard guard(*object->lock);

	/*
	 * Channels nobody collected are released here rather than at destruction:
	 * a closed connection has no use for them, and holding them would keep the
	 * memory for as long as PHP keeps the object.
	 */
	std::vector<std::pair<std::shared_ptr<rtc::DataChannel>, std::shared_ptr<data_channel_shared>>> abandoned;

	WEBRTC_TRY
		auto shared = *object->shared;
		std::lock_guard shared_guard(shared->lock);
		shared->accepting = false;
		abandoned.swap(shared->pending_channels);
		shared->pending_candidates.clear();
	WEBRTC_CATCH

	if (object->connection == NULL) {
		/* already closed; nothing further to do */
		return;
	}

	WEBRTC_TRY
		object->connection->close();
		delete object->connection;
		object->connection = NULL;
	WEBRTC_CATCH
}

zend_class_entry* init_class_PeerConnection() {
	peer_connection_ce = register_class_pmmp_webrtc_PeerConnection();
	peer_connection_ce->create_object = peer_connection_new;

	peer_connection_zend_object_handlers = *zend_get_std_object_handlers();
	peer_connection_zend_object_handlers.offset = XtOffsetOf(peer_connection_zend_object, std);
	peer_connection_zend_object_handlers.free_obj = peer_connection_free;
	peer_connection_zend_object_handlers.clone_obj = NULL;

	return peer_connection_ce;
}
