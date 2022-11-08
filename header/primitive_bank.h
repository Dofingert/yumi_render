//
// Created by Dofingert on 2022/11/8.
//

#ifndef SOFTWARE_RASTERIZER_HEADER_PRIMITIVE_BANK_H_
#define SOFTWARE_RASTERIZER_HEADER_PRIMITIVE_BANK_H_

struct primitive_input_t {
  unsigned char index[3]; // Sub-index, just used to index in vertex_bank
  unsigned char flag;     // reserve for further usage.
};

#endif //SOFTWARE_RASTERIZER_HEADER_PRIMITIVE_BANK_H_
