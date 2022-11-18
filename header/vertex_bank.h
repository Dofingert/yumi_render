//
// Created by Dofingert on 2022/11/8.
//

#ifndef SOFTWARE_RASTERIZER_HEADER_VERTEX_BANK_H_
#define SOFTWARE_RASTERIZER_HEADER_VERTEX_BANK_H_

#include "render_type.h"

#define VERTEX_BUF_SIZE (128)

struct flag_t {
  unsigned char master_matrix_id: 8;
  unsigned char addition_matrix_id: 8;
  unsigned short weight: 16; // Weight for master_matrix_id, should / 65536 for true value.
};

struct VertexSlot {
  vec3f pos;
  vec3f normal;
  vec2s uv;
  flag_t vertex_flag;
};
// CommandSlot load VertexNum and TriangleNum
// VertexSlot export one address to axi write.
// Once 8 word received (32 byte)
// A transformed is automatically happened and push a vertex in buffer.
// Matrix is an axi-controlled mem, data should be directly write into it.
// 8 * 2 * 2 World matrix is supply,
// 8 is available for per vertex bank's pos and normal (normal is not the same as pos).

class VertexBank {
  int size = 0;
  VertexSlot slot[VERTEX_BUF_SIZE];
 public:
  int push(const VertexSlot &input_ref);
  int clear();
  const VertexSlot *get(int id) const;
};

#endif //SOFTWARE_RASTERIZER_HEADER_VERTEX_BANK_H_
