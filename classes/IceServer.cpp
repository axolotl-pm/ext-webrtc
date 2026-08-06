extern "C" {
#include "php.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_enum.h"
#include "../stubs/IceServer_arginfo.h"
}

#include "IceServer.h"
#include "Enums.h"
#include "WebRtcException.h"

#include <string>

static zend_object_handlers ice_server_zend_object_handlers;
zend_class_entry* ice_server_ce;

static zend_object* ice_server_new(zend_class_entry* ce) {
	auto object = alloc_custom_zend_object<ice_server_zend_object>(ce, &ice_server_zend_object_handlers);

	object->server = NULL;

	return &object->std;
}

static void ice_server_free(zend_object* std) {
	auto object = fetch_from_zend_object<ice_server_zend_object>(std);

	delete object->server;
	object->server = NULL;
	zend_object_std_dtor(&object->std);
}

void ice_server_create_zval(zval* out, const rtc::IceServer& server) {
	object_init_ex(out, ice_server_ce);

	auto object = fetch_from_zend_object<ice_server_zend_object>(Z_OBJ_P(out));
	object->server = new rtc::IceServer(server);
}

#define REQUIRE_SERVER(object) \
	do { \
		if ((object)->server == NULL) { \
			zend_throw_exception(webrtc_exception_ce, "IceServer is not initialized", 0); \
			RETURN_THROWS(); \
		} \
	} while (0)

#define ICE_SERVER_METHOD(name) PHP_METHOD(pmmp_webrtc_IceServer, name)

ICE_SERVER_METHOD(__construct) {
	WEBRTC_PARSE_NO_PARAMETERS();

	zend_throw_exception(webrtc_exception_ce, "IceServer must be created via stun() or turn()", 0);
}

ICE_SERVER_METHOD(stun) {
	zend_string* hostname;
	zend_long port = 3478;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 2)
		Z_PARAM_STR(hostname)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(port)
	ZEND_PARSE_PARAMETERS_END();

	if (port < 1 || port > 65535) {
		zend_argument_value_error(2, "must be between 1 and 65535");
		RETURN_THROWS();
	}

	object_init_ex(return_value, ice_server_ce);
	auto object = fetch_from_zend_object<ice_server_zend_object>(Z_OBJ_P(return_value));

	WEBRTC_TRY
		object->server = new rtc::IceServer(
			std::string(ZSTR_VAL(hostname), ZSTR_LEN(hostname)),
			static_cast<uint16_t>(port)
		);
	WEBRTC_CATCH
}

ICE_SERVER_METHOD(turn) {
	zend_string* hostname;
	zend_string* username;
	zend_string* password;
	zend_long port;
	zval* relay = NULL;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 4, 5)
		Z_PARAM_STR(hostname)
		Z_PARAM_LONG(port)
		Z_PARAM_STR(username)
		Z_PARAM_STR(password)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(relay, relay_type_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (port < 1 || port > 65535) {
		zend_argument_value_error(2, "must be between 1 and 65535");
		RETURN_THROWS();
	}

	rtc::IceServer::RelayType relay_type = rtc::IceServer::RelayType::TurnUdp;
	if (relay != NULL) {
		switch (Z_LVAL_P(zend_enum_fetch_case_value(Z_OBJ_P(relay)))) {
			case 0: relay_type = rtc::IceServer::RelayType::TurnUdp; break;
			case 1: relay_type = rtc::IceServer::RelayType::TurnTcp; break;
			case 2: relay_type = rtc::IceServer::RelayType::TurnTls; break;
			default:
				zend_throw_exception(webrtc_exception_ce, "unknown RelayType", 0);
				RETURN_THROWS();
		}
	}

	object_init_ex(return_value, ice_server_ce);
	auto object = fetch_from_zend_object<ice_server_zend_object>(Z_OBJ_P(return_value));

	WEBRTC_TRY
		object->server = new rtc::IceServer(
			std::string(ZSTR_VAL(hostname), ZSTR_LEN(hostname)),
			static_cast<uint16_t>(port),
			std::string(ZSTR_VAL(username), ZSTR_LEN(username)),
			std::string(ZSTR_VAL(password), ZSTR_LEN(password)),
			relay_type
		);
	WEBRTC_CATCH
}

ICE_SERVER_METHOD(getHostname) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = ICE_SERVER_THIS();
	REQUIRE_SERVER(object);

	RETURN_STRINGL(object->server->hostname.c_str(), object->server->hostname.size());
}

ICE_SERVER_METHOD(getPort) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = ICE_SERVER_THIS();
	REQUIRE_SERVER(object);

	RETURN_LONG(object->server->port);
}

ICE_SERVER_METHOD(getUsername) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = ICE_SERVER_THIS();
	REQUIRE_SERVER(object);

	if (object->server->type != rtc::IceServer::Type::Turn) {
		RETURN_NULL();
	}
	RETURN_STRINGL(object->server->username.c_str(), object->server->username.size());
}

ICE_SERVER_METHOD(getPassword) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = ICE_SERVER_THIS();
	REQUIRE_SERVER(object);

	if (object->server->type != rtc::IceServer::Type::Turn) {
		RETURN_NULL();
	}
	RETURN_STRINGL(object->server->password.c_str(), object->server->password.size());
}

ICE_SERVER_METHOD(getRelayType) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = ICE_SERVER_THIS();
	REQUIRE_SERVER(object);

	if (object->server->type != rtc::IceServer::Type::Turn) {
		RETURN_NULL();
	}
	if (!webrtc_set_enum(return_value, relay_type_ce, (zend_long)object->server->relayType)) {
		RETURN_THROWS();
	}
}

ICE_SERVER_METHOD(isTurn) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = ICE_SERVER_THIS();
	REQUIRE_SERVER(object);

	RETURN_BOOL(object->server->type == rtc::IceServer::Type::Turn);
}

zend_class_entry* init_class_IceServer(void) {
	ice_server_ce = register_class_pmmp_webrtc_IceServer();
	ice_server_ce->create_object = ice_server_new;

	ice_server_zend_object_handlers = *zend_get_std_object_handlers();
	ice_server_zend_object_handlers.offset = XtOffsetOf(ice_server_zend_object, std);
	ice_server_zend_object_handlers.free_obj = ice_server_free;
	ice_server_zend_object_handlers.clone_obj = NULL;

	return ice_server_ce;
}
