#ifndef HELPERCURRENCY_H
#define HELPERCURRENCY_H

#include <iostream>
#include <string>
#include <algorithm>
#include <thread>
#include <vector>
#include <limits>
#include "common/fixednumber.h"
#include "common/vector.h"
#include "common/vectorint.h"
#include "screenhelper.h"

class TriangleEdgeData{
public:
    int block_size;
    int block_size_minus_one;
    Vec2Int min_block, max_block;
    Vec2Int p12_q;
    Vec2Int p23_q;
    Vec2Int p31_q;
    int C12_qq;
    int C23_qq;
    int C31_qq;
    Vec2Int p12_qq;
    Vec2Int p23_qq;
    Vec2Int p31_qq;
    TriangleEdgeData(const Vec3 &p1_vp, const Vec3 &p2_vp, const Vec3 &p3_vp);
};

class DrawTriangleCurrency{
public:
    void static draw(const Vec3 &p1_vp, const Vec3 &p2_vp, const Vec3 &p3_vp);
private:
    void static loop_blocks(Vec2Int start, int block_per_thread, TriangleEdgeData ed, Vec4 color);
    void static draw_block(int x, int y, TriangleEdgeData ed, Vec4 color);
};

#endif