/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 5cf8de214f01dfeb99ea363bc430ced3cb9850b7 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_pmmp_webrtc_DataChannel___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_pmmp_webrtc_DataChannel_getLabel, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_pmmp_webrtc_DataChannel_getProtocol arginfo_class_pmmp_webrtc_DataChannel_getLabel

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_pmmp_webrtc_DataChannel_getId, 0, 0, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_pmmp_webrtc_DataChannel_isOpen, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_pmmp_webrtc_DataChannel_isClosed arginfo_class_pmmp_webrtc_DataChannel_isOpen

#define arginfo_class_pmmp_webrtc_DataChannel_isUnordered arginfo_class_pmmp_webrtc_DataChannel_isOpen

#define arginfo_class_pmmp_webrtc_DataChannel_getMaxRetransmits arginfo_class_pmmp_webrtc_DataChannel_getId

#define arginfo_class_pmmp_webrtc_DataChannel_getMaxPacketLifeTime arginfo_class_pmmp_webrtc_DataChannel_getId

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_pmmp_webrtc_DataChannel_getMaxMessageSize, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_pmmp_webrtc_DataChannel_getBufferedAmount arginfo_class_pmmp_webrtc_DataChannel_getMaxMessageSize

#define arginfo_class_pmmp_webrtc_DataChannel_getAvailableAmount arginfo_class_pmmp_webrtc_DataChannel_getMaxMessageSize

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_pmmp_webrtc_DataChannel_send, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_pmmp_webrtc_DataChannel_receive, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

#define arginfo_class_pmmp_webrtc_DataChannel_peek arginfo_class_pmmp_webrtc_DataChannel_receive

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_pmmp_webrtc_DataChannel_close, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()


ZEND_METHOD(pmmp_webrtc_DataChannel, __construct);
ZEND_METHOD(pmmp_webrtc_DataChannel, getLabel);
ZEND_METHOD(pmmp_webrtc_DataChannel, getProtocol);
ZEND_METHOD(pmmp_webrtc_DataChannel, getId);
ZEND_METHOD(pmmp_webrtc_DataChannel, isOpen);
ZEND_METHOD(pmmp_webrtc_DataChannel, isClosed);
ZEND_METHOD(pmmp_webrtc_DataChannel, isUnordered);
ZEND_METHOD(pmmp_webrtc_DataChannel, getMaxRetransmits);
ZEND_METHOD(pmmp_webrtc_DataChannel, getMaxPacketLifeTime);
ZEND_METHOD(pmmp_webrtc_DataChannel, getMaxMessageSize);
ZEND_METHOD(pmmp_webrtc_DataChannel, getBufferedAmount);
ZEND_METHOD(pmmp_webrtc_DataChannel, getAvailableAmount);
ZEND_METHOD(pmmp_webrtc_DataChannel, send);
ZEND_METHOD(pmmp_webrtc_DataChannel, receive);
ZEND_METHOD(pmmp_webrtc_DataChannel, peek);
ZEND_METHOD(pmmp_webrtc_DataChannel, close);


static const zend_function_entry class_pmmp_webrtc_DataChannel_methods[] = {
	ZEND_ME(pmmp_webrtc_DataChannel, __construct, arginfo_class_pmmp_webrtc_DataChannel___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(pmmp_webrtc_DataChannel, getLabel, arginfo_class_pmmp_webrtc_DataChannel_getLabel, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_DataChannel, getProtocol, arginfo_class_pmmp_webrtc_DataChannel_getProtocol, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_DataChannel, getId, arginfo_class_pmmp_webrtc_DataChannel_getId, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_DataChannel, isOpen, arginfo_class_pmmp_webrtc_DataChannel_isOpen, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_DataChannel, isClosed, arginfo_class_pmmp_webrtc_DataChannel_isClosed, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_DataChannel, isUnordered, arginfo_class_pmmp_webrtc_DataChannel_isUnordered, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_DataChannel, getMaxRetransmits, arginfo_class_pmmp_webrtc_DataChannel_getMaxRetransmits, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_DataChannel, getMaxPacketLifeTime, arginfo_class_pmmp_webrtc_DataChannel_getMaxPacketLifeTime, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_DataChannel, getMaxMessageSize, arginfo_class_pmmp_webrtc_DataChannel_getMaxMessageSize, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_DataChannel, getBufferedAmount, arginfo_class_pmmp_webrtc_DataChannel_getBufferedAmount, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_DataChannel, getAvailableAmount, arginfo_class_pmmp_webrtc_DataChannel_getAvailableAmount, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_DataChannel, send, arginfo_class_pmmp_webrtc_DataChannel_send, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_DataChannel, receive, arginfo_class_pmmp_webrtc_DataChannel_receive, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_DataChannel, peek, arginfo_class_pmmp_webrtc_DataChannel_peek, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_DataChannel, close, arginfo_class_pmmp_webrtc_DataChannel_close, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_pmmp_webrtc_DataChannel(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "pmmp\\webrtc", "DataChannel", class_pmmp_webrtc_DataChannel_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	return class_entry;
}
