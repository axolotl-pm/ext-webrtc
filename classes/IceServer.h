#ifndef WEBRTC_ICE_SERVER_H
#define WEBRTC_ICE_SERVER_H

extern "C" {
#include "php.h"
}
#include "../ZendUtil.h"

#include <rtc/rtc.hpp>

typedef struct _ice_server_zend_object {
	rtc::IceServer* server;
	zend_object std;
} ice_server_zend_object;

#define ICE_SERVER_FROM_ZVAL(zv) fetch_from_zend_object<ice_server_zend_object>(Z_OBJ_P(zv))
#define ICE_SERVER_THIS() ICE_SERVER_FROM_ZVAL(ZEND_THIS)

extern zend_class_entry* ice_server_ce;

zend_class_entry* init_class_IceServer(void);

void ice_server_create_zval(zval* out, const rtc::IceServer& server);

#endif
