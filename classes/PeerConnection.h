#ifndef WEBRTC_PEER_CONNECTION_H
#define WEBRTC_PEER_CONNECTION_H

extern "C" {
#include "php.h"
}
#include "../ZendUtil.h"

#include "DataChannel.h"

#include <rtc/rtc.hpp>

#include <memory>
#include <mutex>
#include <vector>

struct peer_connection_shared {
	std::mutex lock;
	/* the queue state is created with the channel, so they travel together */
	std::vector<std::pair<std::shared_ptr<rtc::DataChannel>, std::shared_ptr<data_channel_shared>>> pending_channels;
	/* gathered locally rather than sent by the peer, so nothing bounds these
	 * beyond the interfaces and servers the connection was configured with */
	std::vector<rtc::Candidate> pending_candidates;
	/* cleared by close(), so a callback still in flight stops queueing */
	bool accepting = true;
	/*
	 * Gathering as the callbacks saw it, which is not what the connection
	 * reports.
	 */
	rtc::PeerConnection::GatheringState gathering_state = rtc::PeerConnection::GatheringState::New;
	/* taken from the options at construction; zero means no limit */
	size_t max_pending_channels = 0;
	/* one allowance for every channel of this connection together */
	std::shared_ptr<data_channel_budget> receive_budget;
};

typedef struct _peer_connection_zend_object {
	rtc::PeerConnection* connection;
	std::mutex* lock;
	std::shared_ptr<peer_connection_shared>* shared;
	zend_object std;
} peer_connection_zend_object;

#define PEER_CONNECTION_FROM_ZVAL(zv) fetch_from_zend_object<peer_connection_zend_object>(Z_OBJ_P(zv))
#define PEER_CONNECTION_THIS() PEER_CONNECTION_FROM_ZVAL(ZEND_THIS)

extern zend_class_entry* peer_connection_ce;

zend_class_entry* init_class_PeerConnection(void);

#endif
