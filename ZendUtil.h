#ifndef WEBRTC_ZEND_UTIL_H
#define WEBRTC_ZEND_UTIL_H

extern "C" {
#include "php.h"
}

#include <exception>

template<typename class_name>
static class_name* fetch_from_zend_object(zend_object* obj) {
	return reinterpret_cast<class_name *>(reinterpret_cast<char *>(obj) - XtOffsetOf(class_name, std));
}

template<typename class_name>
static class_name* alloc_custom_zend_object(zend_class_entry* ce, zend_object_handlers* handlers) {
	auto* object = static_cast<class_name *>(emalloc(sizeof(class_name) + zend_object_properties_size(ce)));

	zend_object_std_init(&object->std, ce);
	object_properties_init(&object->std, ce);

	object->std.handlers = handlers;

	return object;
}

#define WEBRTC_PARSE_NO_PARAMETERS() \
	if (zend_parse_parameters_none_throw() == FAILURE) { \
		RETURN_THROWS(); \
	}

#define WEBRTC_TRY try {
#define WEBRTC_CATCH \
	} catch (const std::exception& e) { \
		zend_throw_exception(webrtc_exception_ce, e.what(), 0); \
		RETURN_THROWS(); \
	} catch (...) { \
		zend_throw_exception(webrtc_exception_ce, "unknown C++ exception", 0); \
		RETURN_THROWS(); \
	}

#endif
