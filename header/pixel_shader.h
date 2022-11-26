//
// Created by Dofingert on 2022/11/18.
//

#ifndef SOFTWARE_RASTERIZER_HEADER_PIXEL_SHADER_H_
#define SOFTWARE_RASTERIZER_HEADER_PIXEL_SHADER_H_

#include "render_type.h"
#include "sync_fifo.h"
#define PIXEL_SHADER_W (4)
#define PIXEL_SHADER_H (1)
#define PIXEL_SHADER_BANK_W_N (1)
#define PIXEL_SHADER_BANK_H_N (4)

#define PIXEL_SHADER_FIFO_SIZE (128)

struct PixelShaderWarp {
  unsigned int fb_ptr;
  unsigned short depth[PIXEL_SHADER_H][PIXEL_SHADER_W];
  uint16_t tex[PIXEL_SHADER_H][PIXEL_SHADER_W];
  vec3f normal[PIXEL_SHADER_H][PIXEL_SHADER_W];
};

typedef SyncFifo<PixelShaderWarp, (PIXEL_SHADER_FIFO_SIZE)> PixelShaderWarpFifo;
void pixel_shader_controller(PixelShaderWarpFifo& fifo);

#endif //SOFTWARE_RASTERIZER_HEADER_PIXEL_SHADER_H_
