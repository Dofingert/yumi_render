#ifndef SOFTWARE_RASTERIZER_BINARY_3D_H
#define SOFTWARE_RASTERIZER_BINARY_3D_H

// 定义数据结构体
// 顶点和矩阵
struct p3_vec_t{
    float x;
    float y;
    float z;
    unsigned short u; // 10 bits valid : 1024
    unsigned short v; // 10 bits valid : 1024
}; // 输入顶点 16 bytes

struct trans_vec_t{
    float d[4][4];
}; // 相机 PV 矩阵

struct frag_t {
    unsigned short last_frame_id;
    unsigned short index[3];
}; // 面索引 8 bytes

struct frag_describer_t {
    unsigned int frag_count;
};

struct header_t {
    unsigned int frame_cnt; // 标记总帧数, 有多少帧就有多少 PV 矩阵
    unsigned int frag_describer_cnt;  // 标记总面片数，frag_describer_t 的数量
    unsigned int frag_cnt;
    unsigned int vec3_cnt; // 总点数
};

// 二进制中形如
//struct packed_binary_t {
//    header_t head;
//    char             tex            [1024][1024][3]; // rgb
//    trans_vec_t      pv             [head.frame_cnt];
//    frag_describer_t frag_describer [head.frag_describer_cnt];
//    frag_t           frag           [head.frag_cnt];
//    p3_vec_t         p              [head.vec3_cnt];
//};

#endif //SOFTWARE_RASTERIZER_BINARY_3D_H
