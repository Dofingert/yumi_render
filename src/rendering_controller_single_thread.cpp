//
// Created by Dofingert on 2022/11/26.
//

#include <cstdio>
#include <iostream>
#include "../header/primitive_bank.h"
#include "../header/vertex_bank.h"
#include "../header/pixel_shader.h"
#include "../header/utils.h"

extern matrix44f V;
extern matrix44f P;    // VP is saved in register,
// for every write to P, V is automatically mul on it.
// for every write to MVP's pos, P*V is automatically mul on it.
// for every write to MVP's normal, V is automatically mul on it.
extern matrix44f MVP[2][4][2]; // [Bank][sel][pos/uv]
extern PrimitiveFifo primitive_fifo;
extern PixelShaderWarpFifo pixel_shader_fifo[PIXEL_SHADER_BANK_W_N][PIXEL_SHADER_BANK_H_N];
extern bool d_flag;
int screen_size_w = 512;
int screen_size_h = 512;
uint32_t *frame_buffer;
uint16_t *tex;
extern uint16_t *depth_buffer;

matrix44f lookat(const vec3f &src, const vec3f &dst, const vec3f &up) {
  vec3f z = (dst - src).normalized();
  vec3f x = up.cross(z).normalized();
  vec3f y = z.cross(x);
  matrix44f result;
  result << x[0], x[1], x[2], 0,
	  y[0], y[1], y[2], 0,
	  z[0], z[1], z[2], 0,
	  0, 0, 0, 1;
  vec4f temp_src;
  temp_src << src[0], src[1], src[2], 1;
  temp_src = result * temp_src;
  result << x[0], x[1], x[2], -temp_src[0],
	  y[0], y[1], y[2], -temp_src[1],
	  z[0], z[1], z[2], -temp_src[2],
	  0, 0, 0, 1;
//  result.transposeInPlace();
  return result;
}

matrix44f get_p(float aspect, float size, float far, float near) {
  matrix44f result;
  result << 1.f / (aspect * size), 0, 0, 0,
	  0, (1.f / size), 0, 0,
	  0, 0, ((far + near) / (far - near)), (-2 * far * near / (far - near)),
	  0, 0, 1.f, 0.f;
  return result;
}

int single_thread_render_main() {
  // Prepare all rendering points and primitive info.
  // TODO: add support for obj import.

  // For test, a test screen set cam at (0,0,2), a triangle at (-1,3,0) (-1,3,5) (1,3,0)
  // Normal and uv can be temporally ignored.
  /*Test start*/
  d_flag = true;
  frame_buffer = (uint32_t *)calloc(4, screen_size_w * screen_size_h);
  depth_buffer = (uint16_t *)calloc(2, screen_size_w * screen_size_h);
  VertexSlot test_vertex[4];
  test_vertex[0].pos << -1, 4, 0, 1;
  test_vertex[1].pos << -1, 3, 3, 1;
  test_vertex[2].pos << 2, 3, 0, 1;
  test_vertex[3].pos << 2, 2, 3, 1;
  test_vertex[0].uv << 0, 0;
  test_vertex[1].uv << 0, 65535;
  test_vertex[2].uv << 65535, 0;
  test_vertex[3].uv << 65535, 65535;
  test_vertex[0].vertex_flag = {0, 0, 65535};
  test_vertex[1].vertex_flag = {0, 0, 65535};
  test_vertex[2].vertex_flag = {0, 0, 65535};
  test_vertex[3].vertex_flag = {0, 0, 65535};
  PrimitiveInput test_primitive[2] = {{0, 1, 2, 0xff}, {1, 2, 3, 0xff}};

  // set V and P matrix.
  V = lookat(vec3f{-1, -1, 8}, vec3f{test_vertex[0].pos[0] + test_vertex[3].pos[0],
									test_vertex[0].pos[1] + test_vertex[3].pos[1],
									test_vertex[0].pos[2] + test_vertex[3].pos[2]} / 2, vec3f{0, 0, 1});;
  P = get_p(1, 1, 10, 0.4);
  // set M matrix.
  MVP[0][0][0] = P * V;
  MVP[0][0][1] = V;
  std::cout << P * V * test_vertex[0].pos << std::endl;
  std::cout << P * V * test_vertex[1].pos << std::endl;
  std::cout << P * V * test_vertex[2].pos << std::endl;
  /*Test screen end*/
  // start vertex_transform.
  vertex_transform_controller(0, 4, test_vertex);
  // start primitive_assembling and kill.
  primitive_assembler_controller(0, 2, &test_primitive[0]);
  primitive_fifo.kill();
  // start primitive_dispatch. (auto killed)
  primitive_dispatcher_controller();
  // start pixel shader. (auto killed)
  for (auto &dx : pixel_shader_fifo) {
	for (auto &dy : dx) {
	  pixel_shader_controller(dy);
	}
  }
  return 0;
}