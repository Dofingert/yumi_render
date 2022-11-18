//
// Created by Dofingert on 2022/11/18.
//

#ifndef SOFTWARE_RASTERIZER_HEADER_SYNC_FIFO_H_
#define SOFTWARE_RASTERIZER_HEADER_SYNC_FIFO_H_

#include <mutex>
#include <condition_variable>
template <typename T, int FIFO_SIZE>
class SyncFifo {
  unsigned int fifo_head = 0;
  bool is_killed = false;
  std::mutex modify;
  std::condition_variable cond_producer;
  std::condition_variable cond_consumer;
  T slot[FIFO_SIZE];
  unsigned int fifo_end = 0;
 public:
  int push(const T &input_ref) {
	std::unique_lock<std::mutex> modify_lock(modify);
	unsigned int size = fifo_end - fifo_head;
	while (size >= FIFO_SIZE) {
	  cond_producer.wait(modify_lock);
	  size = fifo_end - fifo_head;
	}
	unsigned int new_fifo_end = fifo_end + 1;
	unsigned int write_fifo_ptr = fifo_end % (FIFO_SIZE);
	slot[write_fifo_ptr] = input_ref;
	fifo_end = new_fifo_end;
	modify_lock.unlock();
	cond_consumer.notify_one();
	return 0;
  }
  void kill(){
	std::unique_lock<std::mutex> modify_lock(modify);
	is_killed = true;
	modify_lock.unlock();
	cond_consumer.notify_all();
  } // Called by push side.
  int pop(T *target_buf) {
	std::unique_lock<std::mutex> modify_lock(modify);
	unsigned int size = fifo_end - fifo_head;
	while (size <= 0) {
	  cond_consumer.wait(modify_lock);
	  size = fifo_end - fifo_head;
	  if (size == 0 && is_killed) {
		return -1;
	  }
	}
	unsigned int new_fifo_head = fifo_head + 1;
	unsigned int read_fifo_ptr = fifo_head % (FIFO_SIZE);
	*target_buf = slot[read_fifo_ptr];
	fifo_head = new_fifo_head;
	modify_lock.unlock();
	return 0;
  }
};

#endif //SOFTWARE_RASTERIZER_HEADER_SYNC_FIFO_H_
