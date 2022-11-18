//
// Created by Dofingert on 2022/11/18.
//

#ifndef SOFTWARE_RASTERIZER_HEADER_PIXEL_SHADER_H_
#define SOFTWARE_RASTERIZER_HEADER_PIXEL_SHADER_H_

#include "render_type.h"
#define PIXEL_SHADER_W (4)
#define PIXEL_SHADER_H (1)

struct shader_warps{
  	unsigned int fb_ptr;
	rgb tex[PIXEL_SHADER_H][PIXEL_SHADER_W];
	vec3f normal[PIXEL_SHADER_H][PIXEL_SHADER_W];
};

#endif //SOFTWARE_RASTERIZER_HEADER_PIXEL_SHADER_H_
