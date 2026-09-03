extern "C" {
#include "php.h"
#include "Zend/zend_exceptions.h"
#include "../stubs/DataChannelOptions_arginfo.h"
}

#include "DataChannelOptions.h"
#include "WebRtcException.h"

#include <chrono>
#include <string>

static zend_object_handlers data_channel_options_zend_object_handlers;
zend_class_entry* data_channel_options_ce;

static zend_object* data_channel_options_new(zend_class_entry* ce) {
	auto object = alloc_custom_zend_object<data_channel_options_zend_object>(ce, &data_channel_options_zend_object_handlers);

	object->init = NULL;
	object->init = new rtc::DataChannelInit();

	return &object->std;
}

static void data_channel_options_free(zend_object* std) {
	auto object = fetch_from_zend_object<data_channel_options_zend_object>(std);

	delete object->init;
	object->init = NULL;
	zend_object_std_dtor(&object->std);
}

#define DATA_CHANNEL_OPTIONS_METHOD(name) PHP_METHOD(pmmp_webrtc_DataChannelOptions, name)

DATA_CHANNEL_OPTIONS_METHOD(__construct) {
	WEBRTC_PARSE_NO_PARAMETERS();

	zend_throw_exception(webrtc_exception_ce, "DataChannelOptions must be created via create()", 0);
}

DATA_CHANNEL_OPTIONS_METHOD(create) {
	WEBRTC_PARSE_NO_PARAMETERS();

	object_init_ex(return_value, data_channel_options_ce);
}

DATA_CHANNEL_OPTIONS_METHOD(setUnordered) {
	bool unordered;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
		Z_PARAM_BOOL(unordered)
	ZEND_PARSE_PARAMETERS_END();

	DATA_CHANNEL_OPTIONS_THIS()->init->reliability.unordered = unordered;

	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

DATA_CHANNEL_OPTIONS_METHOD(setMaxRetransmits) {
	zend_long count;
	bool count_is_null;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
		Z_PARAM_LONG_OR_NULL(count, count_is_null)
	ZEND_PARSE_PARAMETERS_END();

	auto reliability = &DATA_CHANNEL_OPTIONS_THIS()->init->reliability;

	if (count_is_null) {
		reliability->maxRetransmits.reset();
		RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
	}

	if (count < 0 || count > UINT_MAX) {
		zend_argument_value_error(1, "must be between 0 and %u", UINT_MAX);
		RETURN_THROWS();
	}

	WEBRTC_TRY
		reliability->maxPacketLifeTime.reset();
		reliability->maxRetransmits = static_cast<unsigned int>(count);
	WEBRTC_CATCH

	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

DATA_CHANNEL_OPTIONS_METHOD(setMaxPacketLifeTime) {
	zend_long milliseconds;
	bool milliseconds_is_null;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
		Z_PARAM_LONG_OR_NULL(milliseconds, milliseconds_is_null)
	ZEND_PARSE_PARAMETERS_END();

	auto reliability = &DATA_CHANNEL_OPTIONS_THIS()->init->reliability;

	if (milliseconds_is_null) {
		reliability->maxPacketLifeTime.reset();
		RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
	}

	/* libdatachannel narrows this to uint32_t when it opens the channel, and a
	 * value that does not fit makes the open fail where nobody can see it */
	if (milliseconds < 0 || milliseconds > UINT32_MAX) {
		zend_argument_value_error(1, "must be between 0 and %u", UINT32_MAX);
		RETURN_THROWS();
	}

	WEBRTC_TRY
		reliability->maxRetransmits.reset();
		reliability->maxPacketLifeTime = std::chrono::milliseconds(milliseconds);
	WEBRTC_CATCH

	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

DATA_CHANNEL_OPTIONS_METHOD(setProtocol) {
	zend_string* protocol;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
		Z_PARAM_STR(protocol)
	ZEND_PARSE_PARAMETERS_END();

	WEBRTC_TRY
		DATA_CHANNEL_OPTIONS_THIS()->init->protocol = std::string(ZSTR_VAL(protocol), ZSTR_LEN(protocol));
	WEBRTC_CATCH

	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

DATA_CHANNEL_OPTIONS_METHOD(setNegotiated) {
	bool negotiated;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
		Z_PARAM_BOOL(negotiated)
	ZEND_PARSE_PARAMETERS_END();

	DATA_CHANNEL_OPTIONS_THIS()->init->negotiated = negotiated;

	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

DATA_CHANNEL_OPTIONS_METHOD(setId) {
	zend_long id;
	bool id_is_null;

	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
		Z_PARAM_LONG_OR_NULL(id, id_is_null)
	ZEND_PARSE_PARAMETERS_END();

	auto init = DATA_CHANNEL_OPTIONS_THIS()->init;

	if (id_is_null) {
		init->id.reset();
		RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
	}

	if (id < 0 || id > UINT16_MAX) {
		zend_argument_value_error(1, "must be between 0 and %u", UINT16_MAX);
		RETURN_THROWS();
	}

	init->id = static_cast<uint16_t>(id);

	RETURN_OBJ_COPY(Z_OBJ_P(ZEND_THIS));
}

DATA_CHANNEL_OPTIONS_METHOD(isUnordered) {
	WEBRTC_PARSE_NO_PARAMETERS();

	RETURN_BOOL(DATA_CHANNEL_OPTIONS_THIS()->init->reliability.unordered);
}

DATA_CHANNEL_OPTIONS_METHOD(getMaxRetransmits) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto reliability = &DATA_CHANNEL_OPTIONS_THIS()->init->reliability;
	if (!reliability->maxRetransmits.has_value()) {
		RETURN_NULL();
	}
	RETURN_LONG(static_cast<zend_long>(*reliability->maxRetransmits));
}

DATA_CHANNEL_OPTIONS_METHOD(getMaxPacketLifeTime) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto reliability = &DATA_CHANNEL_OPTIONS_THIS()->init->reliability;
	if (!reliability->maxPacketLifeTime.has_value()) {
		RETURN_NULL();
	}
	RETURN_LONG(static_cast<zend_long>(reliability->maxPacketLifeTime->count()));
}

DATA_CHANNEL_OPTIONS_METHOD(getProtocol) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto protocol = &DATA_CHANNEL_OPTIONS_THIS()->init->protocol;
	RETURN_STRINGL(protocol->c_str(), protocol->size());
}

DATA_CHANNEL_OPTIONS_METHOD(isNegotiated) {
	WEBRTC_PARSE_NO_PARAMETERS();

	RETURN_BOOL(DATA_CHANNEL_OPTIONS_THIS()->init->negotiated);
}

DATA_CHANNEL_OPTIONS_METHOD(getId) {
	WEBRTC_PARSE_NO_PARAMETERS();

	auto init = DATA_CHANNEL_OPTIONS_THIS()->init;
	if (!init->id.has_value()) {
		RETURN_NULL();
	}
	RETURN_LONG(static_cast<zend_long>(*init->id));
}

zend_class_entry* init_class_DataChannelOptions() {
	data_channel_options_ce = register_class_pmmp_webrtc_DataChannelOptions();
	data_channel_options_ce->create_object = data_channel_options_new;

	data_channel_options_zend_object_handlers = *zend_get_std_object_handlers();
	data_channel_options_zend_object_handlers.offset = XtOffsetOf(data_channel_options_zend_object, std);
	data_channel_options_zend_object_handlers.free_obj = data_channel_options_free;
	data_channel_options_zend_object_handlers.clone_obj = NULL;

	return data_channel_options_ce;
}
