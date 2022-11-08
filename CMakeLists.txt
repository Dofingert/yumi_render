cmake_minimum_required(VERSION 3.16)
project(Software_rasterizer)

set(CMAKE_CXX_STANDARD 11)
include_directories(./3rd/eigen-3.4.0)
add_executable(Software_rasterizer main.cpp header/render_type.h)
