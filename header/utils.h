//
// Created by Dofingert on 2022/11/26.
//

#ifndef SOFTWARE_RASTERIZER_HEADER_UTILS_H_
#define SOFTWARE_RASTERIZER_HEADER_UTILS_H_
int single_thread_render_main();
int ppm_output(uint32_t *fb,unsigned int width,unsigned int height,FILE* fp);
#endif //SOFTWARE_RASTERIZER_HEADER_UTILS_H_
