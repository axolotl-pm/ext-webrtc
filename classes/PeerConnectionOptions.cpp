extern "C" {
#include "php.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_enum.h"
#include "../stubs/PeerConnectionOptions_arginfo.h"
}

#include "PeerConnectionOptions.h"
#include "IceServer.h"
#include "Enums.h"
#include "WebRtcException.h"

#include <string>
#include <vector>

static zend_object_handlers peer_connection_options_zend_object_handlers;
zend_class_entry* peer_connection_options_ce;

static zend_object* options_new(zend_class_entry* ce) {
	auto object = alloc_custom_zend_object<peer_connection_options_zend_object>(ce, &peer_connection_options_zend_object_handlers);

	object->config = NULL;
	object->max_receive_queue = OPTIONS_DEFAULT_MAX_RECEIVE_QUEUE;
	object->max_pending_data_channels = OPTIONS_DEFAULT_MAX_PENDING_DATA_CHANNELS;

	object->config = new rtc::Configuration();

	return &object->std;
}

static void options_free(zend_object* std) {
	auto object = fetch_from_zend_object<peer_connection_options_zend_object>(std);

	delete object->config;
	object->config = NULL;
	zend_object_std_dtor(&object->std);
}

static bool options_check_pem_file(zend_string* path, uint32_t arg_num) {
	const char* reason;

	if (php_check_open_basedir_ex(ZSTR_VAL(path), 0) != 0) {
		reason = "path is restricted by PHP's open_basedir setting";
	} else {
		errno = 0;
		if (VCWD_ACCESS(ZSTR_VAL(path), R_OK) == 0) {
			return true;
		}

		switch (errno) {
			case ENOENT: reason = "no such file or directory"; break;
			case EACCES: reason = "permission denied"; break;
			default: reason = "cannot be opened for reading"; break;
		}
	}

	zend_argument_value_error(arg_num, "must be a readable file (\"%s\": %s)", ZSTR_VAL(path), reason);
	return false;
}

#define OPTIONS_METHOD(name) PHP_METHOD(pmmp_webrtc_PeerConnectionOptions, name)

OPTIONS_METHOD(__construct) {
	WEBRTC_PARSE_NO_PARAMETERS();

	zend_throw_exception(webrtc_exception_ce, "PeerConnectionOptions must be created via create()", 0);
}

OPTIONS_METHOD(create) {
	WEBRTC_PARSE_NO_PARAMETERS();

	object_init_ex(return_value, peer_connection_options_ce);
}

OPTIONS_METHOD(setMaxMessageSize) {
	zend_long bytes;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
		Z_PARAM_LONG(bytes)
	ZEND_PARSE_PARAMETERS_END();

	if (bytes < 1 || bytes > OPTIONS_MAX_MESSAGE_SIZE_LIMIT) {
		zend_argument_value_error(1, "must be between 1 and %d", OPTIONS_MAX_MESSAGE_SIZE_LIMIT);
		RETURN_THROWS();
	}

	OPTIONS_THIS()->config->maxMessageSize = (size_t)bytes;

	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

OPTIONS_METHOD(setMaxReceiveQueueSize) {
	zend_long bytes;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
		Z_PARAM_LONG(bytes)
	ZEND_PARSE_PARAMETERS_END();

	if (bytes < 0) {
		zend_argument_value_error(1, "must be greater than or equal to 0");
		RETURN_THROWS();
	}

	OPTIONS_THIS()->max_receive_queue = (size_t)bytes;

	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

OPTIONS_METHOD(getMaxReceiveQueueSize) {
	WEBRTC_PARSE_NO_PARAMETERS();

	RETURN_LONG((zend_long)OPTIONS_THIS()->max_receive_queue);
}

OPTIONS_METHOD(setMaxPendingDataChannels) {
	zend_long count;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
		Z_PARAM_LONG(count)
	ZEND_PARSE_PARAMETERS_END();

	if (count < 0) {
		zend_argument_value_error(1, "must be greater than or equal to 0");
		RETURN_THROWS();
	}

	OPTIONS_THIS()->max_pending_data_channels = static_cast<size_t>(count);

	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

OPTIONS_METHOD(getMaxPendingDataChannels) {
	WEBRTC_PARSE_NO_PARAMETERS();

	RETURN_LONG(static_cast<zend_long>(OPTIONS_THIS()->max_pending_data_channels));
}

OPTIONS_METHOD(setIceServers) {
	zval* args = NULL;
	uint32_t argc = 0;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, -1)
		Z_PARAM_VARIADIC('*', args, argc)
	ZEND_PARSE_PARAMETERS_END();

	std::vector<rtc::IceServer> servers;
	servers.reserve(argc);

	for (uint32_t i = 0; i < argc; i++) {
		if (Z_TYPE(args[i]) != IS_OBJECT || !instanceof_function(Z_OBJCE(args[i]), ice_server_ce)) {
			zend_argument_type_error(i + 1, "must be of type pmmp\\webrtc\\IceServer");
			RETURN_THROWS();
		}

		auto server = fetch_from_zend_object<ice_server_zend_object>(Z_OBJ(args[i]));
		if (server->server == NULL) {
			zend_throw_exception(webrtc_exception_ce, "IceServer is not initialized", 0);
			RETURN_THROWS();
		}

		servers.push_back(*server->server);
	}

	WEBRTC_TRY
		OPTIONS_THIS()->config->iceServers = std::move(servers);
	WEBRTC_CATCH

	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

OPTIONS_METHOD(setPortRange) {
	zend_long begin;
	zend_long end;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 2)
		Z_PARAM_LONG(begin)
		Z_PARAM_LONG(end)
	ZEND_PARSE_PARAMETERS_END();

	if (begin < 1 || begin > 65535) {
		zend_argument_value_error(1, "must be between 1 and 65535");
		RETURN_THROWS();
	}
	if (end < begin || end > 65535) {
		zend_argument_value_error(2, "must be between the start port and 65535");
		RETURN_THROWS();
	}

	auto config = OPTIONS_THIS()->config;
	config->portRangeBegin = static_cast<uint16_t>(begin);
	config->portRangeEnd = static_cast<uint16_t>(end);

	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

OPTIONS_METHOD(setBindAddress) {
	zend_string* address = NULL;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
		Z_PARAM_STR_OR_NULL(address)
	ZEND_PARSE_PARAMETERS_END();

	WEBRTC_TRY
		auto config = OPTIONS_THIS()->config;
		if (address == NULL) {
			config->bindAddress.reset();
		} else {
			config->bindAddress = std::string(ZSTR_VAL(address), ZSTR_LEN(address));
		}
	WEBRTC_CATCH

	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

OPTIONS_METHOD(setCertificate) {
	zend_string* cert_file;
	zend_string* key_file;
	zend_string* pass = NULL;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 3)
		Z_PARAM_PATH_STR(cert_file)
		Z_PARAM_PATH_STR(key_file)
		Z_PARAM_OPTIONAL
		Z_PARAM_STR_OR_NULL(pass)
	ZEND_PARSE_PARAMETERS_END();

	if (!options_check_pem_file(cert_file, 1) || !options_check_pem_file(key_file, 2)) {
		RETURN_THROWS();
	}

	WEBRTC_TRY
		auto config = OPTIONS_THIS()->config;
		config->certificatePemFile = std::string(ZSTR_VAL(cert_file), ZSTR_LEN(cert_file));
		config->keyPemFile = std::string(ZSTR_VAL(key_file), ZSTR_LEN(key_file));
		if (pass == NULL) {
			config->keyPemPass.reset();
		} else {
			config->keyPemPass = std::string(ZSTR_VAL(pass), ZSTR_LEN(pass));
		}
	WEBRTC_CATCH

	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

OPTIONS_METHOD(setIceTcpEnabled) {
	bool enable;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
		Z_PARAM_BOOL(enable)
	ZEND_PARSE_PARAMETERS_END();

	OPTIONS_THIS()->config->enableIceTcp = enable;

	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

OPTIONS_METHOD(setIceTransportPolicy) {
	zval* policy_zval;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
		Z_PARAM_OBJECT_OF_CLASS(policy_zval, transport_policy_ce)
	ZEND_PARSE_PARAMETERS_END();

	switch (Z_LVAL_P(zend_enum_fetch_case_value(Z_OBJ_P(policy_zval)))) {
		case 0: OPTIONS_THIS()->config->iceTransportPolicy = rtc::TransportPolicy::All; break;
		case 1: OPTIONS_THIS()->config->iceTransportPolicy = rtc::TransportPolicy::Relay; break;
		default:
			zend_throw_exception(webrtc_exception_ce, "unknown TransportPolicy", 0);
			RETURN_THROWS();
	}

	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

OPTIONS_METHOD(getIceTransportPolicy) {
	WEBRTC_PARSE_NO_PARAMETERS();

	if (!webrtc_set_enum(return_value, transport_policy_ce,
			static_cast<zend_long>(OPTIONS_THIS()->config->iceTransportPolicy))) {
		RETURN_THROWS();
	}
}

OPTIONS_METHOD(getMaxMessageSize) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto config = OPTIONS_THIS()->config;
	RETURN_LONG(config->maxMessageSize.has_value() ? static_cast<zend_long>(*config->maxMessageSize) : 0);
}

OPTIONS_METHOD(getIceServers) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto config = OPTIONS_THIS()->config;

	array_init_size(return_value, static_cast<uint32_t>(config->iceServers.size()));

	WEBRTC_TRY
		for (const auto& server : config->iceServers) {
			zval entry;
			ice_server_create_zval(&entry, server);
			add_next_index_zval(return_value, &entry);
		}
	WEBRTC_CATCH
}

OPTIONS_METHOD(getPortRangeBegin) {
	WEBRTC_PARSE_NO_PARAMETERS();

	RETURN_LONG(OPTIONS_THIS()->config->portRangeBegin);
}

OPTIONS_METHOD(getPortRangeEnd) {
	WEBRTC_PARSE_NO_PARAMETERS();

	RETURN_LONG(OPTIONS_THIS()->config->portRangeEnd);
}

OPTIONS_METHOD(getBindAddress) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto config = OPTIONS_THIS()->config;
	if (!config->bindAddress.has_value()) {
		RETURN_NULL();
	}
	RETURN_STRINGL(config->bindAddress->c_str(), config->bindAddress->size());
}

OPTIONS_METHOD(getCertificatePemFile) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto config = OPTIONS_THIS()->config;
	if (!config->certificatePemFile.has_value()) {
		RETURN_NULL();
	}
	RETURN_STRINGL(config->certificatePemFile->c_str(), config->certificatePemFile->size());
}

OPTIONS_METHOD(getKeyPemFile) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto config = OPTIONS_THIS()->config;
	if (!config->keyPemFile.has_value()) {
		RETURN_NULL();
	}
	RETURN_STRINGL(config->keyPemFile->c_str(), config->keyPemFile->size());
}

OPTIONS_METHOD(getKeyPemPass) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto config = OPTIONS_THIS()->config;
	if (!config->keyPemPass.has_value()) {
		RETURN_NULL();
	}
	RETURN_STRINGL(config->keyPemPass->c_str(), config->keyPemPass->size());
}

OPTIONS_METHOD(isIceTcpEnabled) {
	WEBRTC_PARSE_NO_PARAMETERS();

	RETURN_BOOL(OPTIONS_THIS()->config->enableIceTcp);
}

zend_class_entry* init_class_PeerConnectionOptions() {
	peer_connection_options_ce = register_class_pmmp_webrtc_PeerConnectionOptions();
	peer_connection_options_ce->create_object = options_new;

	peer_connection_options_zend_object_handlers = *zend_get_std_object_handlers();
	peer_connection_options_zend_object_handlers.offset = XtOffsetOf(peer_connection_options_zend_object, std);
	peer_connection_options_zend_object_handlers.free_obj = options_free;
	peer_connection_options_zend_object_handlers.clone_obj = NULL;

	return peer_connection_options_ce;
}
