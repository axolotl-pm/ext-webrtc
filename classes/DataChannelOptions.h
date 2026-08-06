#ifndef WEBRTC_DATA_CHANNEL_OPTIONS_H
#define WEBRTC_DATA_CHANNEL_OPTIONS_H

extern "C" {
#include "php.h"
}
#include "../ZendUtil.h"

#include <rtc/rtc.hpp>

typedef struct _data_channel_options_zend_object {
	rtc::DataChannelInit* init;
	zend_object std;
} data_channel_options_zend_object;

#define DATA_CHANNEL_OPTIONS_FROM_ZVAL(zv) fetch_from_zend_object<data_channel_options_zend_object>(Z_OBJ_P(zv))
#define DATA_CHANNEL_OPTIONS_THIS() DATA_CHANNEL_OPTIONS_FROM_ZVAL(ZEND_THIS)

extern zend_class_entry* data_channel_options_ce;

zend_class_entry* init_class_DataChannelOptions();

#endif
