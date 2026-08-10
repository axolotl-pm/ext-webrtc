/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: a45571bcd907643b0dd5cb85e1cdae92943b8b88 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_pmmp_webrtc_PeerConnection___construct, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, options, pmmp\\webrtc\\PeerConnectionOptions, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_pmmp_webrtc_PeerConnection_setRemoteOffer, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, sdp, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_pmmp_webrtc_PeerConnection_setRemoteAnswer arginfo_class_pmmp_webrtc_PeerConnection_setRemoteOffer

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_pmmp_webrtc_PeerConnection_createDataChannel, 0, 1, pmmp\\webrtc\\DataChannel, 0)
	ZEND_ARG_TYPE_INFO(0, label, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, options, pmmp\\webrtc\\DataChannelOptions, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_pmmp_webrtc_PeerConnection_pollDataChannels, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_pmmp_webrtc_PeerConnection_pollLocalCandidates arginfo_class_pmmp_webrtc_PeerConnection_pollDataChannels

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_pmmp_webrtc_PeerConnection_addRemoteCandidate, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, candidate, pmmp\\webrtc\\IceCandidate, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_pmmp_webrtc_PeerConnection_getGatheringState, 0, 0, pmmp\\webrtc\\GatheringState, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_pmmp_webrtc_PeerConnection_getState, 0, 0, pmmp\\webrtc\\ConnectionState, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_pmmp_webrtc_PeerConnection_getLocalDescription, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

#define arginfo_class_pmmp_webrtc_PeerConnection_getLocalFingerprint arginfo_class_pmmp_webrtc_PeerConnection_getLocalDescription

#define arginfo_class_pmmp_webrtc_PeerConnection_getLocalAddress arginfo_class_pmmp_webrtc_PeerConnection_getLocalDescription

#define arginfo_class_pmmp_webrtc_PeerConnection_getRemoteAddress arginfo_class_pmmp_webrtc_PeerConnection_getLocalDescription

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_pmmp_webrtc_PeerConnection_close, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()


ZEND_METHOD(pmmp_webrtc_PeerConnection, __construct);
ZEND_METHOD(pmmp_webrtc_PeerConnection, setRemoteOffer);
ZEND_METHOD(pmmp_webrtc_PeerConnection, setRemoteAnswer);
ZEND_METHOD(pmmp_webrtc_PeerConnection, createDataChannel);
ZEND_METHOD(pmmp_webrtc_PeerConnection, pollDataChannels);
ZEND_METHOD(pmmp_webrtc_PeerConnection, pollLocalCandidates);
ZEND_METHOD(pmmp_webrtc_PeerConnection, addRemoteCandidate);
ZEND_METHOD(pmmp_webrtc_PeerConnection, getGatheringState);
ZEND_METHOD(pmmp_webrtc_PeerConnection, getState);
ZEND_METHOD(pmmp_webrtc_PeerConnection, getLocalDescription);
ZEND_METHOD(pmmp_webrtc_PeerConnection, getLocalFingerprint);
ZEND_METHOD(pmmp_webrtc_PeerConnection, getLocalAddress);
ZEND_METHOD(pmmp_webrtc_PeerConnection, getRemoteAddress);
ZEND_METHOD(pmmp_webrtc_PeerConnection, close);


static const zend_function_entry class_pmmp_webrtc_PeerConnection_methods[] = {
	ZEND_ME(pmmp_webrtc_PeerConnection, __construct, arginfo_class_pmmp_webrtc_PeerConnection___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_PeerConnection, setRemoteOffer, arginfo_class_pmmp_webrtc_PeerConnection_setRemoteOffer, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_PeerConnection, setRemoteAnswer, arginfo_class_pmmp_webrtc_PeerConnection_setRemoteAnswer, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_PeerConnection, createDataChannel, arginfo_class_pmmp_webrtc_PeerConnection_createDataChannel, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_PeerConnection, pollDataChannels, arginfo_class_pmmp_webrtc_PeerConnection_pollDataChannels, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_PeerConnection, pollLocalCandidates, arginfo_class_pmmp_webrtc_PeerConnection_pollLocalCandidates, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_PeerConnection, addRemoteCandidate, arginfo_class_pmmp_webrtc_PeerConnection_addRemoteCandidate, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_PeerConnection, getGatheringState, arginfo_class_pmmp_webrtc_PeerConnection_getGatheringState, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_PeerConnection, getState, arginfo_class_pmmp_webrtc_PeerConnection_getState, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_PeerConnection, getLocalDescription, arginfo_class_pmmp_webrtc_PeerConnection_getLocalDescription, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_PeerConnection, getLocalFingerprint, arginfo_class_pmmp_webrtc_PeerConnection_getLocalFingerprint, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_PeerConnection, getLocalAddress, arginfo_class_pmmp_webrtc_PeerConnection_getLocalAddress, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_PeerConnection, getRemoteAddress, arginfo_class_pmmp_webrtc_PeerConnection_getRemoteAddress, ZEND_ACC_PUBLIC)
	ZEND_ME(pmmp_webrtc_PeerConnection, close, arginfo_class_pmmp_webrtc_PeerConnection_close, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_pmmp_webrtc_PeerConnection(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "pmmp\\webrtc", "PeerConnection", class_pmmp_webrtc_PeerConnection_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	return class_entry;
}
