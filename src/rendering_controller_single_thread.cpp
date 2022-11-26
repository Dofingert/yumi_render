//
// Created by Dofingert on 2022/11/26.
//


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

matrix44f lookat(const vec3f &src, const vec3f &dst, const vec3f &up) {
  vec3f z = (dst - src).normalized();
  vec3f x = up.cross(z).normalized();
  vec3f y = z.cross(x);

  matrix44f result;
  result << x[0], x[1], x[2], -src[0],
	  y[0], y[1], y[2], -src[1],
	  z[0], z[1], z[2], -src[2],
	  0, 0, 0, 1;
//  result.transposeInPlace();
  return result;
}

matrix44f get_p(float aspect, float size, float far, float near) {
  matrix44f result;
  result << 1.f / (aspect * size), 0, 0, 0,
	  0, (1.f / size), 0, 0,
	  0, 0, (-2.f / (far - near)), (-(far + near) / (far - near)),
	  0, 0, 0, 1;
  return result;
}

int single_thread_render_main() {
  // Prepare all rendering points and primitive info.
  // TODO: add support for obj import.

  // For test, a test screen set cam at (0,0,2), a triangle at (-1,3,0) (-1,3,5) (1,3,0)
  // Normal and uv can be temporally ignored.
  /*Test start*/
  d_flag = false;
  VertexSlot test_vertex[3];
  test_vertex[0].pos << -1, 3, 0, 1;
  test_vertex[1].pos << -1, 3, 5, 1;
  test_vertex[2].pos << 1, 3, 0, 1;
  test_vertex[0].vertex_flag = {0, 0, 65535};
  test_vertex[1].vertex_flag = {0, 0, 65535};
  test_vertex[2].vertex_flag = {0, 0, 65535};
  PrimitiveInput test_primitive = {0, 1, 2, 0xff};

  // set V and P matrix.
  V = lookat(vec3f{0, 0, 2}, vec3f{0, 1, 2}, vec3f{0, 0, 1});;
  P = get_p(1, 16, 10, 1);
  // set M matrix.
  MVP[0][0][0] = P * V;
  MVP[0][0][1] = V;
  /*Test screen end*/
  // start vertex_transform.
  vertex_transform_controller(0, 3, test_vertex);
  // start primitive_assembling and kill.
  primitive_assembler_controller(0, 1, &test_primitive);
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