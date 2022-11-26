//
// Created by Dofingert on 2022/11/8.
//

#ifndef SOFTWARE_RASTERIZER_HEADER_RENDER_TYPE_H_
#define SOFTWARE_RASTERIZER_HEADER_RENDER_TYPE_H_

#include <Eigen/Dense>
#include <sg14/fixed_point>

typedef sg14::fixed_point<short, -4> fixed_s11f4;
typedef sg14::fixed_point<short, -11> fixed_s4f11;
typedef sg14::fixed_point<int,   -21> fixed_s10f21;
typedef sg14::fixed_point<short, -15> fixed_s0f15;

typedef Eigen::Matrix<float, 4, 4> matrix44f;
typedef Eigen::Matrix<float, 2, 2> matrix22f; // triangle matrix
typedef Eigen::Vector<float, 4> vec4f; // For position and normal processing.
typedef Eigen::Vector<float, 3> vec3f; // For position and normal
typedef Eigen::Vector<float, 2> vec2f; // Reserve
typedef Eigen::Vector<unsigned short, 2> vec2s; // For UV and cordination
typedef Eigen::Vector<unsigned char, 4> rgba; // For texture
typedef Eigen::Vector<unsigned char, 3> rgb;  // For texture

typedef Eigen::Vector<fixed_s11f4, 2> vec2s11f4; // fixed point
typedef Eigen::Vector<fixed_s10f21, 2> vec2s10f21; // fixed point
typedef Eigen::Vector<fixed_s10f21, 3> vec3s10f21; // fixed point
typedef Eigen::Vector<fixed_s10f21, 3> vec4s10f21; // fixed point
typedef Eigen::Matrix<fixed_s0f15, 2, 2> matrix22s0f15; // fixed point for triangle matrix

float unsigned_short_to_float(unsigned short input);

#endif //SOFTWARE_RASTERIZER_HEADER_RENDER_TYPE_H_
