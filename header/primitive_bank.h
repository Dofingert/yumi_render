//
// Created by Dofingert on 2022/11/8.
//

#ifndef SOFTWARE_RASTERIZER_HEADER_PRIMITIVE_BANK_H_
#define SOFTWARE_RASTERIZER_HEADER_PRIMITIVE_BANK_H_

#define PRIMITIVE_FIFO_SIZE 64

#include <mutex>
#include <condition_variable>
#include "render_type.h"
struct PrimitiveInput {
  unsigned char index[3]; // Sub-index, just used to index in vertex_bank
  unsigned char flag;     // reserve for further usage.
};

struct Primitive {
  vec3f pos[3];
  vec3f normal[3];
  vec2s uv[3];
  unsigned int special_flag; // Used for set primitive_color directly without uv.
}; // 22 Word= 88 bytes

class PrimitiveFifo {
  unsigned int fifo_head;
  bool is_killed = false;
  std::mutex modify;
  std::condition_variable cond_producer;
  std::condition_variable cond_consumer;
  Primitive slot[PRIMITIVE_FIFO_SIZE];
  unsigned int fifo_end;
 public:
  int push(const Primitive &input_ref);
  void kill(); // Called by push side.
  int pop(Primitive *target_buf);
};
// Warning, this module may be used in multiple-thread.
// But fortunately, not need a mutex_lock.
// More over, We put fifo_head and fifo_end on the two side of fifo

#endif //SOFTWARE_RASTERIZER_HEADER_PRIMITIVE_BANK_H_
