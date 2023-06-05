cmake_minimum_required(VERSION 3.16)
project(Software_rasterizer)

set(CMAKE_CXX_STANDARD 11)
include_directories(./3rd/eigen-3.4.0 3rd/fixed_point/include)
add_executable(Software_rasterizer main.cpp header/render_type.h src/primitive_assembler.cpp src/vertex_transform_engine.cpp header/vertex_bank.h src/vertex_bank.cpp header/primitive_bank.h src/short_to_float.cpp src/primitive_dispatcher.cpp header/pixel_shader.h src/pixel_shader.cpp header/sync_fifo.h src/rendering_controller_single_thread.cpp header/utils.h src/image_output.cpp)
add_executable(Tool_obj2binary3d helper_code/obj2binary_3d.cpp helper_code/binary_3d.h)
