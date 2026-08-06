extern "C" {
#include "php.h"
#include "Zend/zend_exceptions.h"
#include "../stubs/DataChannel_arginfo.h"
}

#include "DataChannel.h"
#include "WebRtcException.h"

#include <atomic>
#include <string>
#include <variant>
#include <vector>

static zend_object_handlers data_channel_zend_object_handlers;
zend_class_entry* data_channel_ce;

static zend_object* data_channel_new(zend_class_entry* ce) {
	auto object = alloc_custom_zend_object<data_channel_zend_object>(ce, &data_channel_zend_object_handlers);

	object->channel = NULL;
	object->state = NULL;

	return &object->std;
}

static void data_channel_free(zend_object* std) {
	auto object = fetch_from_zend_object<data_channel_zend_object>(std);

	delete object->channel;
	object->channel = NULL;

	delete object->state;
	object->state = NULL;
	zend_object_std_dtor(&object->std);
}

static size_t message_size(const rtc::message_variant& message) {
	return std::holds_alternative<rtc::binary>(message)
		? std::get<rtc::binary>(message).size()
		: std::get<std::string>(message).size();
}

std::shared_ptr<data_channel_shared> data_channel_attach(const std::shared_ptr<rtc::DataChannel>& channel, std::shared_ptr<data_channel_budget> budget) {
	auto state = std::make_shared<data_channel_shared>();
	state->budget = std::move(budget);

	/* weak, so that the callback the channel owns does not own the channel */
	std::weak_ptr weak = channel;

	channel->onMessage([state, weak](rtc::message_variant message) {
		bool overflowed = false;

		{
			std::lock_guard guard(state->lock);

			size_t size = message_size(message);
			auto& budget = *state->budget;

			size_t previous = budget.used.fetch_add(size);
			if (budget.max != 0 && previous + size > budget.max) {
				budget.used.fetch_sub(size);
				/*
				 * Dropped rather than queued. Recording it matters: a caller that
				 * reassembles a stream must not be handed one with a hole in it.
				 */
				overflowed = !state->overflowed;
				state->overflowed = true;
			} else {
				state->queued_bytes += size;
				state->queue.push_back(std::move(message));
			}
		}

		if (overflowed) {
			try {
				if (auto locked = weak.lock()) {
					locked->close();
				}
			} catch (...) {
			}
		}
	});

	return state;
}

void data_channel_create_zval(zval* out, std::shared_ptr<rtc::DataChannel> channel, std::shared_ptr<data_channel_shared> state) {
	object_init_ex(out, data_channel_ce);

	auto object = fetch_from_zend_object<data_channel_zend_object>(Z_OBJ_P(out));
	object->channel = new std::shared_ptr(std::move(channel));
	object->state = new std::shared_ptr(std::move(state));
}

#define REQUIRE_CHANNEL(object) \
	do { \
		if ((object)->channel == NULL || !*(object)->channel || (object)->state == NULL) { \
			zend_throw_exception(webrtc_exception_ce, "DataChannel is not initialized", 0); \
			RETURN_THROWS(); \
		} \
	} while (0)

#define DATA_CHANNEL_METHOD(name) PHP_METHOD(pmmp_webrtc_DataChannel, name)

/* message_variant holds either binary or text; PHP strings are binary safe, so
 * both are surfaced as a plain string. */
static void return_message(zval* return_value, const rtc::message_variant& message) {
	if (std::holds_alternative<rtc::binary>(message)) {
		const auto& data = std::get<rtc::binary>(message);
		RETURN_STRINGL(reinterpret_cast<const char*>(data.data()), data.size());
	}

	const auto& text = std::get<std::string>(message);
	RETURN_STRINGL(text.c_str(), text.size());
}

DATA_CHANNEL_METHOD(__construct) {
	WEBRTC_PARSE_NO_PARAMETERS();

	zend_throw_exception(webrtc_exception_ce, "DataChannel cannot be constructed directly", 0);
}

DATA_CHANNEL_METHOD(getLabel) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = DATA_CHANNEL_THIS();
	REQUIRE_CHANNEL(object);

	WEBRTC_TRY
		std::string label = (*object->channel)->label();
		RETURN_STRINGL(label.c_str(), label.size());
	WEBRTC_CATCH
}

DATA_CHANNEL_METHOD(getProtocol) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = DATA_CHANNEL_THIS();
	REQUIRE_CHANNEL(object);

	WEBRTC_TRY
		std::string protocol = (*object->channel)->protocol();
		RETURN_STRINGL(protocol.c_str(), protocol.size());
	WEBRTC_CATCH
}

DATA_CHANNEL_METHOD(getId) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = DATA_CHANNEL_THIS();
	REQUIRE_CHANNEL(object);

	WEBRTC_TRY
		auto id = (*object->channel)->id();
		if (!id.has_value()) {
			RETURN_NULL();
		}
		RETURN_LONG((zend_long)*id);
	WEBRTC_CATCH
}

DATA_CHANNEL_METHOD(isOpen) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = DATA_CHANNEL_THIS();
	REQUIRE_CHANNEL(object);

	WEBRTC_TRY
		RETURN_BOOL((*object->channel)->isOpen());
	WEBRTC_CATCH
}

DATA_CHANNEL_METHOD(isClosed) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = DATA_CHANNEL_THIS();
	REQUIRE_CHANNEL(object);

	WEBRTC_TRY
		RETURN_BOOL((*object->channel)->isClosed());
	WEBRTC_CATCH
}

DATA_CHANNEL_METHOD(isUnordered) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = DATA_CHANNEL_THIS();
	REQUIRE_CHANNEL(object);

	WEBRTC_TRY
		RETURN_BOOL((*object->channel)->reliability().unordered);
	WEBRTC_CATCH
}

DATA_CHANNEL_METHOD(getMaxRetransmits) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = DATA_CHANNEL_THIS();
	REQUIRE_CHANNEL(object);

	WEBRTC_TRY
		auto reliability = (*object->channel)->reliability();
		if (!reliability.maxRetransmits.has_value()) {
			RETURN_NULL();
		}
		RETURN_LONG((zend_long)*reliability.maxRetransmits);
	WEBRTC_CATCH
}

DATA_CHANNEL_METHOD(getMaxPacketLifeTime) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = DATA_CHANNEL_THIS();
	REQUIRE_CHANNEL(object);

	WEBRTC_TRY
		auto reliability = (*object->channel)->reliability();
		if (!reliability.maxPacketLifeTime.has_value()) {
			RETURN_NULL();
		}
		RETURN_LONG((zend_long)reliability.maxPacketLifeTime->count());
	WEBRTC_CATCH
}

DATA_CHANNEL_METHOD(getMaxMessageSize) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = DATA_CHANNEL_THIS();
	REQUIRE_CHANNEL(object);

	WEBRTC_TRY
		RETURN_LONG((zend_long)(*object->channel)->maxMessageSize());
	WEBRTC_CATCH
}

DATA_CHANNEL_METHOD(getBufferedAmount) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = DATA_CHANNEL_THIS();
	REQUIRE_CHANNEL(object);

	WEBRTC_TRY
		RETURN_LONG((zend_long)(*object->channel)->bufferedAmount());
	WEBRTC_CATCH
}

DATA_CHANNEL_METHOD(getAvailableAmount) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = DATA_CHANNEL_THIS();
	REQUIRE_CHANNEL(object);

	WEBRTC_TRY
		auto state = *object->state;
		std::lock_guard guard(state->lock);
		RETURN_LONG((zend_long)state->queued_bytes);
	WEBRTC_CATCH
}

DATA_CHANNEL_METHOD(send) {
	zend_string* data;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
		Z_PARAM_STR(data)
	ZEND_PARSE_PARAMETERS_END();

	auto object = DATA_CHANNEL_THIS();
	REQUIRE_CHANNEL(object);

	WEBRTC_TRY
		/* always binary: a framed payload is rarely valid UTF-8 */
		auto bytes = reinterpret_cast<const std::byte*>(ZSTR_VAL(data));
		RETURN_BOOL((*object->channel)->send(bytes, ZSTR_LEN(data)));
	WEBRTC_CATCH
}

/* throws if anything was dropped, since the stream is no longer complete */
#define REQUIRE_NOT_OVERFLOWED(state) \
	do { \
		if ((state)->overflowed) { \
			zend_throw_exception(webrtc_exception_ce, "DataChannel exceeded its receive queue limit and lost messages", 0); \
			RETURN_THROWS(); \
		} \
	} while (0)

DATA_CHANNEL_METHOD(receive) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = DATA_CHANNEL_THIS();
	REQUIRE_CHANNEL(object);

	rtc::message_variant message;

	WEBRTC_TRY
		auto state = *object->state;
		std::lock_guard guard(state->lock);

		REQUIRE_NOT_OVERFLOWED(state);
		if (state->queue.empty()) {
			RETURN_NULL();
		}

		message = std::move(state->queue.front());
		state->queue.pop_front();
		size_t size = message_size(message);
		state->queued_bytes -= size;
		state->budget->used.fetch_sub(size);
	WEBRTC_CATCH

	return_message(return_value, message);
}

DATA_CHANNEL_METHOD(peek) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = DATA_CHANNEL_THIS();
	REQUIRE_CHANNEL(object);

	rtc::message_variant message;

	WEBRTC_TRY
		auto state = *object->state;
		std::lock_guard guard(state->lock);

		REQUIRE_NOT_OVERFLOWED(state);
		if (state->queue.empty()) {
			RETURN_NULL();
		}

		message = state->queue.front();
	WEBRTC_CATCH

	return_message(return_value, message);
}

DATA_CHANNEL_METHOD(close) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = DATA_CHANNEL_THIS();

	if (object->channel == NULL || !*object->channel) {
		return;
	}

	WEBRTC_TRY
		(*object->channel)->close();
	WEBRTC_CATCH
}

zend_class_entry* init_class_DataChannel() {
	data_channel_ce = register_class_pmmp_webrtc_DataChannel();
	data_channel_ce->create_object = data_channel_new;

	data_channel_zend_object_handlers = *zend_get_std_object_handlers();
	data_channel_zend_object_handlers.offset = XtOffsetOf(data_channel_zend_object, std);
	data_channel_zend_object_handlers.free_obj = data_channel_free;
	data_channel_zend_object_handlers.clone_obj = NULL;

	return data_channel_ce;
}
