#ifndef WEBRTC_ICE_CANDIDATE_H
#define WEBRTC_ICE_CANDIDATE_H

extern "C" {
#include "php.h"
}
#include "../ZendUtil.h"

#include <rtc/rtc.hpp>

typedef struct _ice_candidate_zend_object {
	rtc::Candidate* candidate;
	zend_object std;
} ice_candidate_zend_object;

#define ICE_CANDIDATE_FROM_ZVAL(zv) fetch_from_zend_object<ice_candidate_zend_object>(Z_OBJ_P(zv))
#define ICE_CANDIDATE_THIS() ICE_CANDIDATE_FROM_ZVAL(ZEND_THIS)

extern zend_class_entry* ice_candidate_ce;

zend_class_entry* init_class_IceCandidate(void);

void ice_candidate_create_zval(zval* out, const rtc::Candidate& candidate);

#endif
