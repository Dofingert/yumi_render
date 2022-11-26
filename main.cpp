#include <iostream>
#include <sg14/fixed_point>
#include "header/render_type.h"

int screen_size_w = 512;
int screen_size_h = 512;
uint32_t frame_buffer[512][512];
uint16_t tex[512][512];

void test_eigen() {
  matrix44f m1;
  m1 << 1, 0, 0, 1,
	  0, 1, 0, 2,
	  0, 0, 1, 3,
	  0, 0, 0, 1;
  vec4f v1;
  v1 << 1, 2, 3, 1;
  std::cout << m1 * v1 << std::endl;
}

void test_fixpoint() {
  fixed_s4f11 a;
  fixed_s11f4 b;
  fixed_s0f15 c;
  a = 1.21;
  b = 1.21;
  c = 0.75;
  std::cout << a << std::endl;
  std::cout << b << std::endl;
  std::cout << c << std::endl;
  c = -c;
  std::cout << c << std::endl;
}

int main() {
  test_eigen();
  test_fixpoint();
  return 0;
}