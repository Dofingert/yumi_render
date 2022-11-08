#include <iostream>

void test_eigen() {
  Matrix44i m1;
  m1 << 1, 0, 0, 1,
	  0, 1, 0, 2,
	  0, 0, 1, 3,
	  0, 0, 0, 1;
  vec4f v1;
  v1 << 1, 2, 3, 1;
  std::cout << m1 * v1;
}

int main() {
  test_eigen();
  return 0;
}