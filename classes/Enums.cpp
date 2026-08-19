extern "C" {
#include "php.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_enum.h"
#include "../stubs/Enums_arginfo.h"
}

#include "Enums.h"
#include "WebRtcException.h"

zend_class_entry* gathering_state_ce;
zend_class_entry* connection_state_ce;
zend_class_entry* relay_type_ce;
zend_class_entry* signaling_state_ce;
zend_class_entry* transport_policy_ce;

bool webrtc_set_enum(zval* out, zend_class_entry* ce, zend_long value) {
	zend_object* case_obj = NULL;
	if (zend_enum_get_case_by_value(&case_obj, ce, value, NULL, false) == FAILURE) {
		zend_throw_exception(webrtc_exception_ce, "unknown enum value from libdatachannel", 0);
		return false;
	}
	ZVAL_OBJ_COPY(out, case_obj);
	return true;
}

void init_class_Enums() {
	gathering_state_ce = register_class_pmmp_webrtc_GatheringState();
	connection_state_ce = register_class_pmmp_webrtc_ConnectionState();
	relay_type_ce = register_class_pmmp_webrtc_RelayType();
	signaling_state_ce = register_class_pmmp_webrtc_SignalingState();
	transport_policy_ce = register_class_pmmp_webrtc_TransportPolicy();
}
