//
// Created by Dofingert on 2022/11/8.
//

#ifndef SOFTWARE_RASTERIZER_HEADER_PRIMITIVE_BANK_H_
#define SOFTWARE_RASTERIZER_HEADER_PRIMITIVE_BANK_H_
#include "render_type.h"
#include "sync_fifo.h"

#define PRIMITIVE_FIFO_SIZE (128)

struct PrimitiveInput {
  unsigned int index[3]; // Sub-index, just used to index in vertex_bank
  unsigned char flag;     // reserve for further usage.
};

struct Primitive {
  vec4f pos[3];
  vec3f normal[3];
  vec2s uv[3];
  unsigned int special_flag; // Used for set primitive_color directly without uv.
}; // 22 Word= 88 bytes

typedef SyncFifo<Primitive, (PRIMITIVE_FIFO_SIZE)> PrimitiveFifo;
void primitive_assembler_controller(int bank_id, int primitive_num, PrimitiveInput *input_ptr);
void primitive_dispatcher_controller();

#endif //SOFTWARE_RASTERIZER_HEADER_PRIMITIVE_BANK_H_
