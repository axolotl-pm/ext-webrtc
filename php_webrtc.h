//
// Created by minjae on 2026-08-04.
//

#ifndef EXT_WEBRTC_PHP_WEBRTC_H
#define EXT_WEBRTC_PHP_WEBRTC_H

extern zend_module_entry webrtc_module_entry;
#define phpext_webrtc_ptr &webrtc_module_entry

#define PHP_WEBRTC_VERSION "0.1.0"

#if defined(ZTS) && defined(COMPILE_DL_WEBRTC)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif //EXT_WEBRTC_PHP_WEBRTC_H
