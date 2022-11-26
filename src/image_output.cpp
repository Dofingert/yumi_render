//
// Created by Dofingert on 2022/11/26.
//
#include <cstdint>
#include <cstdio>
#include "../header/utils.h"

int ppm_output(uint32_t *fb, unsigned int width, unsigned int height, FILE *fp) {
  fprintf(fp, "P6 %d %d 255\n", width, height);
  for (long i = 0; i <= width * height; i++) {
	fwrite(fb + i, 1, 3, fp);
  }
  return 0;
}