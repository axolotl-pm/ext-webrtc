/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 284967d6b48b06f6c342b1e8d1c0056dc2698f03 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_pmmp_webrtc_set_sctp_settings, 0, 0, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, heartbeatInterval, IS_LONG, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, maxRetransmitAttempts, IS_LONG, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, minRetransmitTimeout, IS_LONG, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, maxRetransmitTimeout, IS_LONG, 1, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, initialRetransmitTimeout, IS_LONG, 1, "null")
ZEND_END_ARG_INFO()

ZEND_FUNCTION(pmmp_webrtc_set_sctp_settings);

static const zend_function_entry ext_functions[] = {
#if (PHP_VERSION_ID >= 80400)
	ZEND_RAW_FENTRY(ZEND_NS_NAME("pmmp\\webrtc", "set_sctp_settings"), zif_pmmp_webrtc_set_sctp_settings, arginfo_pmmp_webrtc_set_sctp_settings, 0, NULL, NULL)
#else
	ZEND_RAW_FENTRY(ZEND_NS_NAME("pmmp\\webrtc", "set_sctp_settings"), zif_pmmp_webrtc_set_sctp_settings, arginfo_pmmp_webrtc_set_sctp_settings, 0)
#endif
	ZEND_FE_END
};
