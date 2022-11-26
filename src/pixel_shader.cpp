//
// Created by Dofingert on 2022/11/18.
//
#include "../header/pixel_shader.h"

extern int screen_size_w; // main render
extern int screen_size_h;
extern uint32_t *frame_buffer; // Frame buffer
extern bool d_flag;
uint16_t *depth_buffer;
PixelShaderWarpFifo pixel_shader_fifo[PIXEL_SHADER_BANK_W_N][PIXEL_SHADER_BANK_H_N];
extern uint16_t *tex;

void simple_pixel_shader(const PixelShaderWarp &info) {
  for (int dx = 0; dx < PIXEL_SHADER_W; dx++) {
	for (int dy = 0; dy < PIXEL_SHADER_H; dy++) {
	  unsigned short d = depth_buffer[info.fb_ptr + dx + dy * screen_size_w];
	  if (d_flag ^ (d >> 15)) {
		// update z-buffer;
		d = 0x7fff | (d_flag << 15);
		depth_buffer[info.fb_ptr + dx + dy * screen_size_w] = d;
	  }
	  if ((info.depth[dy][dx] >> 1) < (d & 0x7fff)) {
		// draw a pixel.
		frame_buffer[info.fb_ptr + dx + dy * screen_size_w] = 0xffffffff; // Pure white.
		// update z-buffer;
		depth_buffer[info.fb_ptr + dx + dy * screen_size_w] = (info.depth[dy][dx] >> 1) | (d_flag << 15);
	  }
	}
  }
}


void primitive_dispatcher_controller(PixelShaderWarpFifo& fifo) {
  PixelShaderWarp handling_warp;
  while (true) {
	int ret = fifo.pop(&handling_warp);
	if (ret != 0) {
	  return;
	}
	simple_pixel_shader(handling_warp);
  }
}