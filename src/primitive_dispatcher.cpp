//
// Created by Dofingert on 2022/11/10.
//
#include "../header/primitive_bank.h"

extern PrimitiveFifo primitive_fifo;

void primitive_dispatcher_controller()
{
  Primitive handling_primitive;
  while(true) {
	int ret = primitive_fifo.pop(&handling_primitive);
	if(ret != 0) {
	  return;
	}
  }
}