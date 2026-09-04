#ifndef WEBRTC_DATA_CHANNEL_H
#define WEBRTC_DATA_CHANNEL_H

extern "C" {
#include "php.h"
}
#include "../ZendUtil.h"

#include <rtc/rtc.hpp>

#include <atomic>
#include <deque>
#include <memory>
#include <mutex>

struct data_channel_budget {
	std::atomic<size_t> used{0};
	std::atomic<size_t> messages{0};
	/* both mean "no limit" when set to zero */
	size_t max = 0;
	size_t max_messages = 0;
};

struct data_channel_shared {
	std::mutex lock;
	std::deque<rtc::message_variant> queue;
	size_t queued_bytes = 0;
	std::shared_ptr<data_channel_budget> budget;
	/* taken from the options at construction; zero means no limit */
	size_t max_send_queue = 0;
	/* set once a message has been dropped; the channel cannot be trusted after that */
	bool overflowed = false;

	/*
	 * Whatever was still queued is handed back here rather than in receive(),
	 * because a channel that is dropped unread frees its memory without anyone
	 * calling receive() at all. Leaving it charged would keep the allowance
	 * spent for as long as the connection lives.
	 */
	~data_channel_shared() {
		if (budget != nullptr) {
			if (queued_bytes != 0) {
				budget->used.fetch_sub(queued_bytes);
			}
			if (!queue.empty()) {
				budget->messages.fetch_sub(queue.size());
			}
		}
	}
};

typedef struct _data_channel_zend_object {
	std::shared_ptr<rtc::DataChannel>* channel;
	std::shared_ptr<data_channel_shared>* state;
	zend_object std;
} data_channel_zend_object;

#define DATA_CHANNEL_FROM_ZVAL(zv) fetch_from_zend_object<data_channel_zend_object>(Z_OBJ_P(zv))
#define DATA_CHANNEL_THIS() DATA_CHANNEL_FROM_ZVAL(ZEND_THIS)

extern zend_class_entry* data_channel_ce;

zend_class_entry* init_class_DataChannel(void);

/*
 * Takes delivery of the channel's messages, charging them to the given budget.
 * Must be called before the channel can receive anything.
 */
std::shared_ptr<data_channel_shared> data_channel_attach(const std::shared_ptr<rtc::DataChannel>& channel, std::shared_ptr<data_channel_budget> budget, size_t max_send_queue);

/* Wrap an existing channel in a new PHP object stored into out. */
void data_channel_create_zval(zval* out, std::shared_ptr<rtc::DataChannel> channel, std::shared_ptr<data_channel_shared> state);

#endif
