//
// Created by Dofingert on 2022/11/8.
//

#include "../header/primitive_bank.h"
#include "../header/vertex_bank.h"

extern PrimitiveFifo primitive_fifo;
extern VertexBank vertex_bank[2];

bool primitive_assembler(const VertexBank &bank, PrimitiveInput input, Primitive *output) {
  Primitive target;
  for (int i = 0; i < 3; i++) {
	const VertexSlot *vertex = bank.get(input.index[i]);
	if(vertex->vertex_flag.master_matrix_id ==0xff) {
	  return false;
	}
	target.pos[i] = vertex->pos;
	target.normal[i] = vertex->normal;
	target.uv[i] = vertex->uv;
	if (input.flag != 0) {
	  target.special_flag = 0x000000ff; // Further, we can add a color plat for this.
	} else {
	  target.special_flag = 0x00000000;
	}
  }
  *output = target;
  return true;
}

void primitive_assembler_controller(int bank_id, int primitive_num, PrimitiveInput *input_ptr) {
  Primitive next_primitive;
  for (int primitive_id = 0; primitive_id < primitive_num; primitive_id++) {
	if (primitive_assembler(vertex_bank[bank_id], input_ptr[primitive_id], &next_primitive))
	  primitive_fifo.push(next_primitive);
  }
}