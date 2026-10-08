/* webrtc extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

extern "C" {
#include "php.h"
#include "Zend/zend_exceptions.h"
#include "ext/standard/info.h"
#include "php_webrtc.h"
#include "ext/spl/spl_exceptions.h"
#include "stubs/WebRtcException_arginfo.h"
}

#include <rtc/global.hpp>
#include <rtc/version.h>

#include <chrono>
#include <future>
#include <utility>

extern "C" {
#include "stubs/constants_arginfo.h"
#include "stubs/functions_arginfo.h"
}

#include "classes/Enums.h"
#include "classes/IceServer.h"
#include "classes/IceCandidate.h"
#include "classes/DataChannel.h"
#include "classes/DataChannelOptions.h"
#include "classes/PeerConnectionOptions.h"
#include "classes/PeerConnection.h"
#include "classes/WebRtcException.h"

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(webrtc)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "Version", PHP_WEBRTC_VERSION);
	php_info_print_table_header(2, "libdatachannel", RTC_VERSION);
	php_info_print_table_header(2, "Experimental", "YES");
	php_info_print_table_end();
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(webrtc)
{
#if defined(ZTS) && defined(COMPILE_DL_WEBRTC)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

zend_class_entry* webrtc_exception_ce;

PHP_MINIT_FUNCTION(webrtc) {
	register_constants_symbols(module_number);

	webrtc_exception_ce = register_class_pmmp_webrtc_WebRtcException(spl_ce_RuntimeException);
	init_class_Enums();
	init_class_IceServer();
	init_class_IceCandidate();
	init_class_DataChannelOptions();
	init_class_DataChannel();
	init_class_PeerConnectionOptions();
	init_class_PeerConnection();

	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(webrtc) {
	try {
		/*
		 * libdatachannel runs background worker threads that outlive individual
		 * connections. Shut them down here to prevent interpreter shutdown from
		 * racing against active threads and crashing after script completion.
		 */
		if (rtc::Cleanup().wait_for(std::chrono::seconds(10)) == std::future_status::timeout) {
			fprintf(stderr, "webrtc: timed out unloading libdatachannel\n");
		}
	} catch (...) {
	}

	return SUCCESS;
}

static bool sctp_setting_in_range(uint32_t arg_num, zend_long value, bool is_null) {
	if (!is_null && (value < 1 || value > UINT32_MAX)) {
		zend_argument_value_error(arg_num, "must be between 1 and %u", UINT32_MAX);
		return false;
	}
	return true;
}

PHP_FUNCTION(pmmp_webrtc_set_sctp_settings) {
	zend_long heartbeat_interval = 0;
	bool heartbeat_interval_is_null = true;
	zend_long max_retransmit_attempts = 0;
	bool max_retransmit_attempts_is_null = true;
	zend_long min_retransmit_timeout = 0;
	bool min_retransmit_timeout_is_null = true;
	zend_long max_retransmit_timeout = 0;
	bool max_retransmit_timeout_is_null = true;
	zend_long initial_retransmit_timeout = 0;
	bool initial_retransmit_timeout_is_null = true;

#if PHP_VERSION_ID >= 80600
	ZEND_PARSE_PARAMETERS_START(0, 5)
#else
	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 5)
#endif
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG_OR_NULL(heartbeat_interval, heartbeat_interval_is_null)
		Z_PARAM_LONG_OR_NULL(max_retransmit_attempts, max_retransmit_attempts_is_null)
		Z_PARAM_LONG_OR_NULL(min_retransmit_timeout, min_retransmit_timeout_is_null)
		Z_PARAM_LONG_OR_NULL(max_retransmit_timeout, max_retransmit_timeout_is_null)
		Z_PARAM_LONG_OR_NULL(initial_retransmit_timeout, initial_retransmit_timeout_is_null)
	ZEND_PARSE_PARAMETERS_END();

	if (!sctp_setting_in_range(1, heartbeat_interval, heartbeat_interval_is_null)
		|| !sctp_setting_in_range(2, max_retransmit_attempts, max_retransmit_attempts_is_null)
		|| !sctp_setting_in_range(3, min_retransmit_timeout, min_retransmit_timeout_is_null)
		|| !sctp_setting_in_range(4, max_retransmit_timeout, max_retransmit_timeout_is_null)
		|| !sctp_setting_in_range(5, initial_retransmit_timeout, initial_retransmit_timeout_is_null)
	) {
		RETURN_THROWS();
	}

	WEBRTC_TRY
		rtc::SctpSettings settings;
		if (!heartbeat_interval_is_null) {
			settings.heartbeatInterval = std::chrono::milliseconds(heartbeat_interval);
		}
		if (!max_retransmit_attempts_is_null) {
			settings.maxRetransmitAttempts = static_cast<unsigned int>(max_retransmit_attempts);
		}
		if (!min_retransmit_timeout_is_null) {
			settings.minRetransmitTimeout = std::chrono::milliseconds(min_retransmit_timeout);
		}
		if (!max_retransmit_timeout_is_null) {
			settings.maxRetransmitTimeout = std::chrono::milliseconds(max_retransmit_timeout);
		}
		if (!initial_retransmit_timeout_is_null) {
			settings.initialRetransmitTimeout = std::chrono::milliseconds(initial_retransmit_timeout);
		}
		rtc::SetSctpSettings(std::move(settings));
	WEBRTC_CATCH
}

static const zend_module_dep module_dependencies[] = {
	ZEND_MOD_REQUIRED("spl")
	ZEND_MOD_END
};

/* {{{ webrtc_module_entry */
zend_module_entry webrtc_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,					/* ini_entries */
	module_dependencies,
	"webrtc",				/* Extension name */
	ext_functions,			/* zend_function_entry */
	PHP_MINIT(webrtc),		/* PHP_MINIT - Module initialization */
	PHP_MSHUTDOWN(webrtc),	/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(webrtc),		/* PHP_RINIT - Request initialization */
	NULL,					/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(webrtc),		/* PHP_MINFO - Module info */
	PHP_WEBRTC_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_WEBRTC
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(webrtc)
#endif
