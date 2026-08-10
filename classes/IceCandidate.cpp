extern "C" {
#include "php.h"
#include "Zend/zend_exceptions.h"
#include "../stubs/IceCandidate_arginfo.h"
}

#include "IceCandidate.h"
#include "WebRtcException.h"

#include <string>

static zend_object_handlers ice_candidate_zend_object_handlers;
zend_class_entry* ice_candidate_ce;

static zend_object* ice_candidate_new(zend_class_entry* ce) {
	auto object = alloc_custom_zend_object<ice_candidate_zend_object>(ce, &ice_candidate_zend_object_handlers);

	object->candidate = NULL;

	return &object->std;
}

static void ice_candidate_free(zend_object* std) {
	auto object = fetch_from_zend_object<ice_candidate_zend_object>(std);

	delete object->candidate;
	object->candidate = NULL;
	zend_object_std_dtor(&object->std);
}

void ice_candidate_create_zval(zval* out, const rtc::Candidate& candidate) {
	object_init_ex(out, ice_candidate_ce);

	auto object = fetch_from_zend_object<ice_candidate_zend_object>(Z_OBJ_P(out));
	object->candidate = new rtc::Candidate(candidate);
}

#define REQUIRE_CANDIDATE(object) \
	do { \
		if ((object)->candidate == NULL) { \
			zend_throw_exception(webrtc_exception_ce, "IceCandidate is not initialized", 0); \
			RETURN_THROWS(); \
		} \
	} while (0)

#define ICE_CANDIDATE_METHOD(name) PHP_METHOD(pmmp_webrtc_IceCandidate, name)

ICE_CANDIDATE_METHOD(__construct) {
	WEBRTC_PARSE_NO_PARAMETERS();

	zend_throw_exception(webrtc_exception_ce, "IceCandidate must be created via create()", 0);
}

ICE_CANDIDATE_METHOD(create) {
	zend_string* candidate;
	zend_string* mid = NULL;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 2)
		Z_PARAM_STR(candidate)
		Z_PARAM_OPTIONAL
		Z_PARAM_STR_OR_NULL(mid)
	ZEND_PARSE_PARAMETERS_END();

	/* an empty line parses into a placeholder candidate rather than failing */
	if (ZSTR_LEN(candidate) == 0) {
		zend_argument_value_error(1, "must not be empty");
		RETURN_THROWS();
	}

	object_init_ex(return_value, ice_candidate_ce);
	auto object = fetch_from_zend_object<ice_candidate_zend_object>(Z_OBJ_P(return_value));

	WEBRTC_TRY
		if (mid == NULL) {
			object->candidate = new rtc::Candidate(std::string(ZSTR_VAL(candidate), ZSTR_LEN(candidate)));
		} else {
			object->candidate = new rtc::Candidate(
				std::string(ZSTR_VAL(candidate), ZSTR_LEN(candidate)),
				std::string(ZSTR_VAL(mid), ZSTR_LEN(mid))
			);
		}
	WEBRTC_CATCH
}

ICE_CANDIDATE_METHOD(getCandidate) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = ICE_CANDIDATE_THIS();
	REQUIRE_CANDIDATE(object);

	WEBRTC_TRY
		std::string line = object->candidate->candidate();
		RETURN_STRINGL(line.c_str(), line.size());
	WEBRTC_CATCH
}

ICE_CANDIDATE_METHOD(getMid) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto object = ICE_CANDIDATE_THIS();
	REQUIRE_CANDIDATE(object);

	WEBRTC_TRY
		std::string mid = object->candidate->mid();
		RETURN_STRINGL(mid.c_str(), mid.size());
	WEBRTC_CATCH
}

zend_class_entry* init_class_IceCandidate(void) {
	ice_candidate_ce = register_class_pmmp_webrtc_IceCandidate();
	ice_candidate_ce->create_object = ice_candidate_new;

	ice_candidate_zend_object_handlers = *zend_get_std_object_handlers();
	ice_candidate_zend_object_handlers.offset = XtOffsetOf(ice_candidate_zend_object, std);
	ice_candidate_zend_object_handlers.free_obj = ice_candidate_free;
	ice_candidate_zend_object_handlers.clone_obj = NULL;

	return ice_candidate_ce;
}
