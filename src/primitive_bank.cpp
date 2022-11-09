//
// Created by Dofingert on 2022/11/8.
//
#include "../header/primitive_bank.h"

int PrimitiveFifo::push(const Primitive &input_ref) {
  std::unique_lock<std::mutex> modify_lock(modify);
  unsigned int size = fifo_end - fifo_head;
  while (size >= PRIMITIVE_FIFO_SIZE) {
	cond_producer.wait(modify_lock);
	size = fifo_end - fifo_head;
  }
  unsigned int new_fifo_end = fifo_end + 1;
  unsigned int write_fifo_ptr = fifo_end % (PRIMITIVE_FIFO_SIZE);
  slot[write_fifo_ptr] = input_ref;
  fifo_end = new_fifo_end;
  modify_lock.unlock();
  cond_consumer.notify_one();
  return 0;
}
int PrimitiveFifo::pop(Primitive *target_buf) {
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
  unsigned int read_fifo_ptr = fifo_head % (PRIMITIVE_FIFO_SIZE);
  *target_buf = slot[read_fifo_ptr];
  fifo_head = new_fifo_head;
  modify_lock.unlock();
  return 0;
}
void PrimitiveFifo::kill() {
  std::unique_lock<std::mutex> modify_lock(modify);
  is_killed = true;
  modify_lock.unlock();
  cond_consumer.notify_all();
}
