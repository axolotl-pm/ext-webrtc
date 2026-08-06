/* webrtc extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

extern "C" {
#include "php.h"
#include "ext/standard/info.h"
#include "php_webrtc.h"
#include "ext/spl/spl_exceptions.h"
#include "stubs/WebRtcException_arginfo.h"
}

#include <rtc/global.hpp>
#include <rtc/version.h>

#include <chrono>
#include <future>

extern "C" {
#include "stubs/constants_arginfo.h"
}

#include "classes/Enums.h"
#include "classes/IceServer.h"
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
	init_class_DataChannelOptions();
	init_class_DataChannel();
	init_class_PeerConnectionOptions();
	init_class_PeerConnection();

	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(webrtc) {
	try {
		// libdatachannel maintains their global thread pool that outlives every connection.
		// In here, we try calling a cleanup so that libdatachannel can cleanup all existing connections.
		// Skipping this races the interpreter shutdown against those threads and segfaults after the
		// script has already finished, so we leave a message here rather than let it look accidental.
		if (rtc::Cleanup().wait_for(std::chrono::seconds(10)) == std::future_status::timeout) { // 10s timeout from libdatachannel C bindings
			fprintf(stderr, "webrtc: timed out unloading libdatachannel\n");
		}
	} catch (...) {
	}

	return SUCCESS;
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
	NULL,					/* zend_function_entry */
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
