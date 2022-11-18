//
// Created by Dofingert on 2022/11/8.
//

#ifndef SOFTWARE_RASTERIZER_HEADER_RENDER_TYPE_H_
#define SOFTWARE_RASTERIZER_HEADER_RENDER_TYPE_H_

#include <Eigen/Dense>

typedef Eigen::Matrix<float, 4, 4> Matrix44f;
typedef Eigen::Vector<float, 4> vec4f; // For postion and normal processing.
typedef Eigen::Vector<float, 3> vec3f; // For postion and normal
typedef Eigen::Vector<float, 2> vec2f; // Reserve
typedef Eigen::Vector<unsigned short, 2> vec2s; // For UV
typedef Eigen::Vector<unsigned char, 4> rgba; // For texture
typedef Eigen::Vector<unsigned char, 3> rgb;  // For texture

float unsigned_short_to_float(unsigned short input);

#endif //SOFTWARE_RASTERIZER_HEADER_RENDER_TYPE_H_
