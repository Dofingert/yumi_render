//
// Created by Dofingert on 2022/11/8.
//
#include "../header/vertex_bank.h"

int VertexBank::push(const VertexSlot &input_ref) {
  if (size == VERTEX_BUF_SIZE) {
	return -1;
  }
  slot[size++] = input_ref;
  return 0;
}

int VertexBank::clear() {
  size = 0;
  return 0;
}

const VertexSlot *VertexBank::get(int id) const {
  if (id < 0 || id >= size) {
	return nullptr;
  }
  return &slot[id];
}

