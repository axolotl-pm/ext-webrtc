#ifndef WEBRTC_ENUMS_H
#define WEBRTC_ENUMS_H

extern "C" {
#include "php.h"
}

extern zend_class_entry* gathering_state_ce;
extern zend_class_entry* connection_state_ce;
extern zend_class_entry* relay_type_ce;
extern zend_class_entry* signaling_state_ce;
extern zend_class_entry* transport_policy_ce;

void init_class_Enums(void);

bool webrtc_set_enum(zval* out, zend_class_entry* ce, zend_long value);

#endif
