/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 8cc11bb21df2bf8dd27919f35762cd0d425468f1 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_pmmp_webrtc_DataChannelOptions___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_pmmp_webrtc_DataChannelOptions_create, 0, 0, pmmp\\webrtc\\DataChannelOptions, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_pmmp_webrtc_DataChannelOptions_setUnordered, 0, 1, pmmp\\webrtc\\DataChannelOptions, 0)
	ZEND_ARG_TYPE_INFO(0, unordered, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_pmmp_webrtc_DataChannelOptions_setMaxRetransmits, 0, 1, pmmp\\webrtc\\DataChannelOptions, 0)
	ZEND_ARG_TYPE_INFO(0, count, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_pmmp_webrtc_DataChannelOptions_setMaxPacketLifeTime, 0, 1, pmmp\\webrtc\\DataChannelOptions, 0)
	ZEND_ARG_TYPE_INFO(0, milliseconds, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_pmmp_webrtc_DataChannelOptions_setProtocol, 0, 1, pmmp\\webrtc\\DataChannelOptions, 0)
	ZEND_ARG_TYPE_INFO(0, protocol, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_pmmp_webrtc_DataChannelOptions_setNegotiated, 0, 1, pmmp\\webrtc\\DataChannelOptions, 0)
	ZEND_ARG_TYPE_INFO(0, negotiated, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_pmmp_webrtc_DataChannelOptions_setId, 0, 1, pmmp\\webrtc\\DataChannelOptions, 0)
	ZEND_ARG_TYPE_INFO(0, id, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_pmmp_webrtc_DataChannelOptions_isUnordered, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_pmmp_webrtc_DataChannelOptions_getMaxRetransmits, 0, 0, IS_LONG, 1)
ZEND_END_ARG_INFO()

#define arginfo_class_pmmp_webrtc_DataChannelOptions_getMaxPacketLifeTime arginfo_class_pmmp_webrtc_DataChannelOptions_getMaxRetransmits

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_pmmp_webrtc_DataChannelOptions_getProtocol, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_pmmp_webrtc_DataChannelOptions_isNegotiated arginfo_class_pmmp_webrtc_DataChannelOptions_isUnordered

#define arginfo_class_pmmp_webrtc_DataChannelOptions_getId arginfo_class_pmmp_webrtc_DataChannelOptions_getMaxRetransmits


ZEND_METHOD(pmmp_webrtc_DataChannelOptions, __construct);
ZEND_METHOD(pmmp_webrtc_DataChannelOptions, create);
ZEND_METHOD(pmmp_webrtc_DataChannelOptions, setUnordered);
ZEND_METHOD(pmmp_webrtc_DataChannelOptions, setMaxRetransmits);
ZEND_METHOD(pmmp_webrtc_DataChannelOptions, setMaxPacketLifeTime);
ZEND_METHOD(pmmp_webrtc_DataChannelOptions, setProtocol);
ZEND_METHOD(pmmp_webrtc_DataChannelOptions, setNegotiated);
ZEND_METHOD(pmmp_webrtc_DataChannelOptions, setId);
ZEND_METHOD(pmmp_webrtc_DataChannelOptions, isUnordered);
ZEND_METHOD(pmmp_webrtc_DataChannelOptions, getMaxRetransmits);
ZEND_METHOD(pmmp_webrtc_DataChannelOptions, getMaxPacketLifeTime);
ZEND_METHOD(pmmp_webrtc_DataChannelOptions, getProtocol);
ZEND_METHOD(pmmp_webrtc_DataChannelOptions, isNegotiated);
ZEND_METHOD(pmmp_webrtc_DataChannelOptions, getId);


static const zend_function_entry class_pmmp_webrtc_DataChannelOptions_methods[] = {
	ZEND_ME(pmmp_webrtc_DataChannelOptions, __construct, arginfo_class_pmmp_webrtc_DataChannelOptions___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(pmmp_webrtc_DataChannelOptions, create, arginfo_class_pmmp_webrtc_DataChannelOptions_create, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(pmmp_webrtc_DataChannelOptions, setUnordered, arginfo_class_pmmp_webrtc_DataChannelOptions_setUnordered, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_DataChannelOptions, setMaxRetransmits, arginfo_class_pmmp_webrtc_DataChannelOptions_setMaxRetransmits, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_DataChannelOptions, setMaxPacketLifeTime, arginfo_class_pmmp_webrtc_DataChannelOptions_setMaxPacketLifeTime, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_DataChannelOptions, setProtocol, arginfo_class_pmmp_webrtc_DataChannelOptions_setProtocol, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_DataChannelOptions, setNegotiated, arginfo_class_pmmp_webrtc_DataChannelOptions_setNegotiated, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_DataChannelOptions, setId, arginfo_class_pmmp_webrtc_DataChannelOptions_setId, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_DataChannelOptions, isUnordered, arginfo_class_pmmp_webrtc_DataChannelOptions_isUnordered, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_DataChannelOptions, getMaxRetransmits, arginfo_class_pmmp_webrtc_DataChannelOptions_getMaxRetransmits, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_DataChannelOptions, getMaxPacketLifeTime, arginfo_class_pmmp_webrtc_DataChannelOptions_getMaxPacketLifeTime, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_DataChannelOptions, getProtocol, arginfo_class_pmmp_webrtc_DataChannelOptions_getProtocol, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_DataChannelOptions, isNegotiated, arginfo_class_pmmp_webrtc_DataChannelOptions_isNegotiated, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_DataChannelOptions, getId, arginfo_class_pmmp_webrtc_DataChannelOptions_getId, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_pmmp_webrtc_DataChannelOptions(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "pmmp\\webrtc", "DataChannelOptions", class_pmmp_webrtc_DataChannelOptions_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	return class_entry;
}
