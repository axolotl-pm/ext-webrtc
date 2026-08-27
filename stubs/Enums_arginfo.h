/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: a4403c235b232bd91e38fa1ecf5bbeef6e6570a0 */




static const zend_function_entry class_pmmp_webrtc_GatheringState_methods[] = {
	ZEND_FE_END
};


static const zend_function_entry class_pmmp_webrtc_ConnectionState_methods[] = {
	ZEND_FE_END
};


static const zend_function_entry class_pmmp_webrtc_RelayType_methods[] = {
	ZEND_FE_END
};


static const zend_function_entry class_pmmp_webrtc_SignalingState_methods[] = {
	ZEND_FE_END
};


static const zend_function_entry class_pmmp_webrtc_TransportPolicy_methods[] = {
	ZEND_FE_END
};

static zend_class_entry *register_class_pmmp_webrtc_GatheringState(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("pmmp\\webrtc\\GatheringState", IS_LONG, class_pmmp_webrtc_GatheringState_methods);

	zval enum_case_NOT_STARTED_value;
	ZVAL_LONG(&enum_case_NOT_STARTED_value, 0);
	zend_enum_add_case_cstr(class_entry, "NOT_STARTED", &enum_case_NOT_STARTED_value);

	zval enum_case_IN_PROGRESS_value;
	ZVAL_LONG(&enum_case_IN_PROGRESS_value, 1);
	zend_enum_add_case_cstr(class_entry, "IN_PROGRESS", &enum_case_IN_PROGRESS_value);

	zval enum_case_COMPLETE_value;
	ZVAL_LONG(&enum_case_COMPLETE_value, 2);
	zend_enum_add_case_cstr(class_entry, "COMPLETE", &enum_case_COMPLETE_value);

	return class_entry;
}

static zend_class_entry *register_class_pmmp_webrtc_ConnectionState(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("pmmp\\webrtc\\ConnectionState", IS_LONG, class_pmmp_webrtc_ConnectionState_methods);

	zval enum_case_NOT_STARTED_value;
	ZVAL_LONG(&enum_case_NOT_STARTED_value, 0);
	zend_enum_add_case_cstr(class_entry, "NOT_STARTED", &enum_case_NOT_STARTED_value);

	zval enum_case_CONNECTING_value;
	ZVAL_LONG(&enum_case_CONNECTING_value, 1);
	zend_enum_add_case_cstr(class_entry, "CONNECTING", &enum_case_CONNECTING_value);

	zval enum_case_CONNECTED_value;
	ZVAL_LONG(&enum_case_CONNECTED_value, 2);
	zend_enum_add_case_cstr(class_entry, "CONNECTED", &enum_case_CONNECTED_value);

	zval enum_case_DISCONNECTED_value;
	ZVAL_LONG(&enum_case_DISCONNECTED_value, 3);
	zend_enum_add_case_cstr(class_entry, "DISCONNECTED", &enum_case_DISCONNECTED_value);

	zval enum_case_FAILED_value;
	ZVAL_LONG(&enum_case_FAILED_value, 4);
	zend_enum_add_case_cstr(class_entry, "FAILED", &enum_case_FAILED_value);

	zval enum_case_CLOSED_value;
	ZVAL_LONG(&enum_case_CLOSED_value, 5);
	zend_enum_add_case_cstr(class_entry, "CLOSED", &enum_case_CLOSED_value);

	return class_entry;
}

static zend_class_entry *register_class_pmmp_webrtc_RelayType(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("pmmp\\webrtc\\RelayType", IS_LONG, class_pmmp_webrtc_RelayType_methods);

	zval enum_case_UDP_value;
	ZVAL_LONG(&enum_case_UDP_value, 0);
	zend_enum_add_case_cstr(class_entry, "UDP", &enum_case_UDP_value);

	zval enum_case_TCP_value;
	ZVAL_LONG(&enum_case_TCP_value, 1);
	zend_enum_add_case_cstr(class_entry, "TCP", &enum_case_TCP_value);

	zval enum_case_TLS_value;
	ZVAL_LONG(&enum_case_TLS_value, 2);
	zend_enum_add_case_cstr(class_entry, "TLS", &enum_case_TLS_value);

	return class_entry;
}

static zend_class_entry *register_class_pmmp_webrtc_SignalingState(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("pmmp\\webrtc\\SignalingState", IS_LONG, class_pmmp_webrtc_SignalingState_methods);

	zval enum_case_STABLE_value;
	ZVAL_LONG(&enum_case_STABLE_value, 0);
	zend_enum_add_case_cstr(class_entry, "STABLE", &enum_case_STABLE_value);

	zval enum_case_HAVE_LOCAL_OFFER_value;
	ZVAL_LONG(&enum_case_HAVE_LOCAL_OFFER_value, 1);
	zend_enum_add_case_cstr(class_entry, "HAVE_LOCAL_OFFER", &enum_case_HAVE_LOCAL_OFFER_value);

	zval enum_case_HAVE_REMOTE_OFFER_value;
	ZVAL_LONG(&enum_case_HAVE_REMOTE_OFFER_value, 2);
	zend_enum_add_case_cstr(class_entry, "HAVE_REMOTE_OFFER", &enum_case_HAVE_REMOTE_OFFER_value);

	zval enum_case_HAVE_LOCAL_PRANSWER_value;
	ZVAL_LONG(&enum_case_HAVE_LOCAL_PRANSWER_value, 3);
	zend_enum_add_case_cstr(class_entry, "HAVE_LOCAL_PRANSWER", &enum_case_HAVE_LOCAL_PRANSWER_value);

	zval enum_case_HAVE_REMOTE_PRANSWER_value;
	ZVAL_LONG(&enum_case_HAVE_REMOTE_PRANSWER_value, 4);
	zend_enum_add_case_cstr(class_entry, "HAVE_REMOTE_PRANSWER", &enum_case_HAVE_REMOTE_PRANSWER_value);

	return class_entry;
}

static zend_class_entry *register_class_pmmp_webrtc_TransportPolicy(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("pmmp\\webrtc\\TransportPolicy", IS_LONG, class_pmmp_webrtc_TransportPolicy_methods);

	zval enum_case_ALL_value;
	ZVAL_LONG(&enum_case_ALL_value, 0);
	zend_enum_add_case_cstr(class_entry, "ALL", &enum_case_ALL_value);

	zval enum_case_RELAY_value;
	ZVAL_LONG(&enum_case_RELAY_value, 1);
	zend_enum_add_case_cstr(class_entry, "RELAY", &enum_case_RELAY_value);

	return class_entry;
}
