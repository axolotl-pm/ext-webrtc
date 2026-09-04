#ifndef WEBRTC_PEER_CONNECTION_OPTIONS_H
#define WEBRTC_PEER_CONNECTION_OPTIONS_H

extern "C" {
#include "php.h"
}
#include "../ZendUtil.h"

#include <rtc/rtc.hpp>

/* all of these mean "no limit" when set to zero */
#define OPTIONS_DEFAULT_MAX_RECEIVE_QUEUE (16 * 1024 * 1024)
#define OPTIONS_DEFAULT_MAX_RECEIVE_QUEUE_MESSAGES 8192
#define OPTIONS_DEFAULT_MAX_SEND_QUEUE (16 * 1024 * 1024)
#define OPTIONS_DEFAULT_MAX_PENDING_DATA_CHANNELS 64

#define OPTIONS_MAX_MESSAGE_SIZE_LIMIT (256 * 1024 * 1024)

typedef struct _peer_connection_options_zend_object {
	rtc::Configuration* config;
	size_t max_receive_queue;
	size_t max_receive_queue_messages;
	size_t max_send_queue;
	size_t max_pending_data_channels;
	zend_object std;
} peer_connection_options_zend_object;

#define OPTIONS_FROM_ZVAL(zv) fetch_from_zend_object<peer_connection_options_zend_object>(Z_OBJ_P(zv))
#define OPTIONS_THIS() OPTIONS_FROM_ZVAL(ZEND_THIS)

extern zend_class_entry* peer_connection_options_ce;

zend_class_entry* init_class_PeerConnectionOptions();

#endif
