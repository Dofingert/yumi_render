//
// Created by Dofingert on 2022/11/10.
//
#include "../header/primitive_bank.h"
#include "../header/pixel_shader.h"

extern PrimitiveFifo primitive_fifo;
extern int screen_size_w;
extern int screen_size_h;
extern PixelShaderWarpFifo pixel_shader_fifo[PIXEL_SHADER_BANK_W_N][PIXEL_SHADER_BANK_H_N];

bool is_in_triangle(const vec2f &ap,
					const vec2f &ab,
					const vec2f &ac,
					const matrix22f &triangle_matrix,
					vec2f *result) {
  vec2f cal_uv{ap.dot(ab), ap.dot(ac)};
  cal_uv = triangle_matrix * cal_uv;
  *result = cal_uv;
  if (cal_uv[0] >= 0 && cal_uv[1] >= 0 && cal_uv[0] + cal_uv[1] <= 1) {
	return true;
  }
  return false;
}

float interpolation(const vec2f &uv, float a, float b, float c) {
  return uv[0] * b + uv[1] * c + (1 - uv[0] - uv[1]) * a;
}

uint16_t uv2tex(const vec2f &uv) {
  int u = (int)(uv[0] * 32);
  int v = (int)(uv[1] * 32);
  return ((u ^ v) & 1) ? 0xffff : 0x0000;
}

void primitive_dispatch(const Primitive &input) {
  vec2s p[3];
  vec2s min{screen_size_w, screen_size_h};
  vec2s max{0, 0};
  for (int i = 0; i < 3; i++) {
	p[i] << (int)((input.pos[i][0] * (float)screen_size_w / 2) + (float)screen_size_w / 2), (int)(
		(input.pos[i][1] * (float)screen_size_h / 2) + (float)screen_size_h / 2);
	for (int j = 0; j < 2; j++) {
	  if (p[i][j] > max[j]) {
		max[j] = p[i][j];
	  }
	  if (p[i][j] < min[j]) {
		min[j] = p[i][j];
	  }
	}
  }
  vec2f ap;
  vec2f ab;
  vec2f ac;
  ab << p[1][0] - p[0][0], p[1][1] - p[0][1];
  ac << p[2][0] - p[0][0], p[2][1] - p[0][1];
  vec2f uv;
  float a, b, c;
  a = ab.dot(ab);
  b = ab.dot(ac);
  c = ac.dot(ac);
  matrix22f triangle_m;
  triangle_m << c, -b, -b, a;
  triangle_m /= (float)(a * c - b * b);
  PixelShaderWarp pixel_shader_task;
  for (int x = (min[0] / PIXEL_SHADER_W) * PIXEL_SHADER_W; x <= max[0]; x += PIXEL_SHADER_W) {
	for (int y = (min[1] / PIXEL_SHADER_H) * PIXEL_SHADER_H; y <= max[1]; y += PIXEL_SHADER_H) {
	  bool valid_shader_blocks = false;
	  pixel_shader_task.fb_ptr = y * screen_size_w + x;
	  for (int dx = 0; dx < PIXEL_SHADER_W; dx += 1) {
		for (int dy = 0; dy < PIXEL_SHADER_H; dy += 1) {
		  ap << (float)(x + dx) - (float)p[0][0], (float)(y + dy) - (float)p[0][1];
		  bool in_triangle = is_in_triangle(ap, ab, ac, triangle_m, &uv);
		  // uv is needed to be adjusted here.
		  float u, v, w;
		  u = uv[0];
		  v = uv[1];
		  w = 1 - u - v;
		  float alpha, beta, sigma;
		  alpha = w / (input.pos[0][3]);
		  beta = u / (input.pos[1][3]);
		  sigma = v / (input.pos[2][3]);
		  u = beta / (alpha + beta + sigma);
		  v = sigma / (alpha + beta + sigma);
		  uv[0] = u;
		  uv[1] = v;
		  u = interpolation(uv, input.uv[0][0], input.uv[1][0], input.uv[2][0]) / 65535;
		  v = interpolation(uv, input.uv[0][1], input.uv[1][1], input.uv[2][1]) / 65535;
		  valid_shader_blocks |= in_triangle;
		  pixel_shader_task.depth[dy][dx] =
			  (unsigned short)(32767
				  * interpolation(uv, input.pos[0][2] + 1, input.pos[1][2] + 1, input.pos[2][2] + 1));
		  if (!in_triangle) {
			pixel_shader_task.depth[dy][dx] = 0xffff;
		  }
		  pixel_shader_task.normal[dy][dx] <<
										   interpolation(uv,
														 input.normal[0][0],
														 input.normal[1][0],
														 input.normal[2][0]),
			  interpolation(uv, input.normal[0][1], input.normal[1][1], input.normal[2][1]),
			  interpolation(uv, input.normal[0][2], input.normal[1][2], input.normal[2][2]);
		  pixel_shader_task.tex[dy][dx] = uv2tex(vec2f{u, v});
		}
	  }
	  if (valid_shader_blocks) {
		pixel_shader_fifo[(x / PIXEL_SHADER_W) % PIXEL_SHADER_BANK_W_N][(y / PIXEL_SHADER_H) % PIXEL_SHADER_BANK_H_N]
			.push(pixel_shader_task);
	  }
	}
  }
}

void primitive_dispatcher_controller() {
  Primitive handling_primitive;
  while (true) {
	int ret = primitive_fifo.pop(&handling_primitive);
	if (ret != 0) {
	  for (auto &dx : pixel_shader_fifo) {
		for (auto &dy : dx) {
		  dy.kill();
		}
	  }
	  return;
	}
	primitive_dispatch(handling_primitive);
  }
}