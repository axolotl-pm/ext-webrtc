/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: f7cb37eb65a4fbfdd7b1a8ec0836d04f5671f618 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_pmmp_webrtc_IceCandidate___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_pmmp_webrtc_IceCandidate_create, 0, 1, pmmp\\webrtc\\IceCandidate, 0)
	ZEND_ARG_TYPE_INFO(0, candidate, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, mid, IS_STRING, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_pmmp_webrtc_IceCandidate_getCandidate, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_pmmp_webrtc_IceCandidate_getMid arginfo_class_pmmp_webrtc_IceCandidate_getCandidate

ZEND_METHOD(pmmp_webrtc_IceCandidate, __construct);
ZEND_METHOD(pmmp_webrtc_IceCandidate, create);
ZEND_METHOD(pmmp_webrtc_IceCandidate, getCandidate);
ZEND_METHOD(pmmp_webrtc_IceCandidate, getMid);

static const zend_function_entry class_pmmp_webrtc_IceCandidate_methods[] = {
	ZEND_ME(pmmp_webrtc_IceCandidate, __construct, arginfo_class_pmmp_webrtc_IceCandidate___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(pmmp_webrtc_IceCandidate, create, arginfo_class_pmmp_webrtc_IceCandidate_create, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(pmmp_webrtc_IceCandidate, getCandidate, arginfo_class_pmmp_webrtc_IceCandidate_getCandidate, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_IceCandidate, getMid, arginfo_class_pmmp_webrtc_IceCandidate_getMid, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_pmmp_webrtc_IceCandidate(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "pmmp\\webrtc", "IceCandidate", class_pmmp_webrtc_IceCandidate_methods);
#if (PHP_VERSION_ID >= 80400)
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE);
#else
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE;
#endif

	return class_entry;
}
