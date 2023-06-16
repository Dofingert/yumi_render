#ifndef SOFTWARE_RASTERIZER_BINARY_3D_H
#define SOFTWARE_RASTERIZER_BINARY_3D_H

// 定义数据结构体
// 顶点和矩阵
struct p3_vec_t {
    float x;
    float y;
    float z;
    unsigned short u; // 10 bits valid : 1024
    unsigned short v; // 10 bits valid : 1024
}; // 输入顶点 16 bytes

struct trans_vec_t {
    float d[4][4];
}; // 相机 PV 矩阵

struct frag_t {
    unsigned int last_frame_id;
    unsigned int index[3];
}; // 面索引 8 bytes

struct frag_describer_t {
    unsigned int frag_count; // 标记当前面的关键帧数量，frag_t 的数量
};

struct header_t {
    unsigned int frame_cnt;           // 标记总帧数, 有多少帧就有多少 PV 矩阵
    unsigned int frag_describer_cnt;  // 标记总面片数，frag_describer_t 的数量
    unsigned int frag_cnt;            // 总面数
    unsigned int vec3_cnt;            // 总点数
};

// 二进制中形如
//struct packed_binary_t {
//    header_t head;
//    unsigned char    tex            [1024][1024][3];               // rgb, 768 KB
//    trans_vec_t      pv             [head.frame_cnt];              // pv 矩阵数组
//    {
//      frag_describer_t frag_describer;
//      frag_t           frag           [frag_describer.frag_count]; // 面片信息，带关键帧
//    }[head.frag_describer_cnt]                                     // 面描述符数组
//    p3_vec_t         p              [head.vec3_cnt];               // 点数组
//};

#endif //SOFTWARE_RASTERIZER_BINARY_3D_H
