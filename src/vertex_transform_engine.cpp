//
// Created by Dofingert on 2022/11/8.
//

#include "../header/vertex_bank.h"

extern VertexBank vertex_bank[2];

extern Matrix44f V;
extern Matrix44f P;    // VP is saved in register,
// for every write to P, V is automatically mul on it.
// for every write to MVP's pos, P*V is automatically mul on it.
// for every write to MVP's normal, V is automatically mul on it.
Matrix44f MVP[2][4][2]; // [Bank][sel][pos/uv]

VertexSlot vertex_transform_core(int bank_id, VertexSlot input) {
  vec4f handling_a;
  vec4f handling_b;
  handling_a << input.pos[0], input.pos[1], input.pos[2], 1;
  handling_b = MVP[bank_id][input.vertex_flag.master_matrix_id][0] * handling_a;
  handling_b = (handling_b * (65536 - input.vertex_flag.weight)) / 65536;
  handling_a = MVP[bank_id][input.vertex_flag.addition_matrix_id][0] * handling_a;
  handling_a = (handling_a * input.vertex_flag.weight) / 65536;
  handling_a = handling_a + handling_b;
  float w = handling_a[3];
  if (handling_a[0] < -w || handling_a[0] > w ||
	  handling_a[1] < -w || handling_a[1] > w ||
	  handling_a[2] < -w || handling_a[2] > w) {
	input.vertex_flag.master_matrix_id = 0xff;
	return input; // out of windows, delete
  }
  float inv_w = 1 / w;
  input.pos << handling_a[0] * inv_w, handling_a[1] * inv_w, handling_a[2] * inv_w; // XYZ

  handling_a << input.normal[0], input.normal[1], input.normal[2], 0;
  handling_b = MVP[bank_id][input.vertex_flag.master_matrix_id][1] * handling_a;
  handling_b = (handling_b * (65536 - input.vertex_flag.weight)) / 65536;
  handling_a = MVP[bank_id][input.vertex_flag.addition_matrix_id][1] * handling_a;
  handling_a = (handling_a * input.vertex_flag.weight) / 65536;
  handling_a = handling_a + handling_b;
  if (handling_a[2] < 0) {
	// backward to cam, delete.
	input.vertex_flag.master_matrix_id = 0xff;
  }
  input.normal << handling_a[0], handling_a[1], handling_a[2];
  return input;
}

void vertex_transform_controller(int bank_id, int vertex_num, VertexSlot *input_ptr) {
  for (int vertex_id = 0; vertex_id < vertex_num; vertex_id++) {
	vertex_bank[bank_id].push(vertex_transform_core(bank_id, input_ptr[vertex_id]));
  }
}