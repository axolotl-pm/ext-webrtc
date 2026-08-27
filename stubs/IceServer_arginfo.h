/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 5a76246b26d798003fae09d83a3647eabd8885b1 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_pmmp_webrtc_IceServer___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_pmmp_webrtc_IceServer_stun, 0, 1, pmmp\\webrtc\\IceServer, 0)
	ZEND_ARG_TYPE_INFO(0, hostname, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, port, IS_LONG, 0, "3478")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_pmmp_webrtc_IceServer_turn, 0, 4, pmmp\\webrtc\\IceServer, 0)
	ZEND_ARG_TYPE_INFO(0, hostname, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, port, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, username, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, password, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, relayType, pmmp\\webrtc\\RelayType, 0, "pmmp\\webrtc\\RelayType::UDP")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_pmmp_webrtc_IceServer_getHostname, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_pmmp_webrtc_IceServer_getPort, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_pmmp_webrtc_IceServer_getUsername, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

#define arginfo_class_pmmp_webrtc_IceServer_getPassword arginfo_class_pmmp_webrtc_IceServer_getUsername

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_pmmp_webrtc_IceServer_getRelayType, 0, 0, pmmp\\webrtc\\RelayType, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_pmmp_webrtc_IceServer_isTurn, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()


ZEND_METHOD(pmmp_webrtc_IceServer, __construct);
ZEND_METHOD(pmmp_webrtc_IceServer, stun);
ZEND_METHOD(pmmp_webrtc_IceServer, turn);
ZEND_METHOD(pmmp_webrtc_IceServer, getHostname);
ZEND_METHOD(pmmp_webrtc_IceServer, getPort);
ZEND_METHOD(pmmp_webrtc_IceServer, getUsername);
ZEND_METHOD(pmmp_webrtc_IceServer, getPassword);
ZEND_METHOD(pmmp_webrtc_IceServer, getRelayType);
ZEND_METHOD(pmmp_webrtc_IceServer, isTurn);


static const zend_function_entry class_pmmp_webrtc_IceServer_methods[] = {
	ZEND_ME(pmmp_webrtc_IceServer, __construct, arginfo_class_pmmp_webrtc_IceServer___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(pmmp_webrtc_IceServer, stun, arginfo_class_pmmp_webrtc_IceServer_stun, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(pmmp_webrtc_IceServer, turn, arginfo_class_pmmp_webrtc_IceServer_turn, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(pmmp_webrtc_IceServer, getHostname, arginfo_class_pmmp_webrtc_IceServer_getHostname, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_IceServer, getPort, arginfo_class_pmmp_webrtc_IceServer_getPort, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_IceServer, getUsername, arginfo_class_pmmp_webrtc_IceServer_getUsername, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_IceServer, getPassword, arginfo_class_pmmp_webrtc_IceServer_getPassword, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_IceServer, getRelayType, arginfo_class_pmmp_webrtc_IceServer_getRelayType, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_IceServer, isTurn, arginfo_class_pmmp_webrtc_IceServer_isTurn, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_pmmp_webrtc_IceServer(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "pmmp\\webrtc", "IceServer", class_pmmp_webrtc_IceServer_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	return class_entry;
}
