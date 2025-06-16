#include "helpercurrency.h"

Vec4 indexToColor(int i){
    if(i == 0){
        return {0, 0, 0, 255};
    }
    if(i == 1){
        return {255, 0, 0, 255};
    }
    if(i == 2){
        return {0, 255, 0, 255};
    }
    if(i == 3){
        return {0, 0, 255, 255};
    }
    return {255, 255, 0, 255};
}

TriangleEdgeData::TriangleEdgeData(const Vec3 &p1_vp, const Vec3 &p2_vp, const Vec3 &p3_vp){
    Vec2 p1_f = ScreenHelper::viewportToCanvasCoordinate(p1_vp);
    Vec2 p2_f = ScreenHelper::viewportToCanvasCoordinate(p2_vp);
    Vec2 p3_f = ScreenHelper::viewportToCanvasCoordinate(p3_vp);
    // Vec2 p1_f(20, 20);
    // Vec2 p2_f(100, 400);
    // Vec2 p3_f(400, 20);
    Vec2 p12_f = p2_f - p1_f;
    Vec2 p13_f = p3_f - p1_f;
    if(scalarCrossVec(p12_f, p13_f) < 0){
        swapVec(p2_f, p3_f);
    }
    Vec2Int p1_q = fixedNumber_fixedXY(p1_f);
    Vec2Int p2_q = fixedNumber_fixedXY(p2_f);
    Vec2Int p3_q = fixedNumber_fixedXY(p3_f);
    Vec2Int min, max;
    min.x = (std::min(std::min(p1_q.x, p2_q.x), p3_q.x) + fixedNumber_CEIL) >> fixedNumber_RESOLUTION;
    min.y = (std::min(std::min(p1_q.y, p2_q.y), p3_q.y) + fixedNumber_CEIL) >> fixedNumber_RESOLUTION;
    max.x = (std::max(std::max(p1_q.x, p2_q.x), p3_q.x) + fixedNumber_CEIL) >> fixedNumber_RESOLUTION;
    max.y = (std::max(std::max(p1_q.y, p2_q.y), p3_q.y) + fixedNumber_CEIL) >> fixedNumber_RESOLUTION;
    block_size = 8;
    block_size_minus_one = block_size - 1;
    int block_size_bitmask = ~(block_size_minus_one);
    min_block = min & block_size_bitmask;
    max_block = (max + block_size) & block_size_bitmask;
    p12_q = p2_q - p1_q;
    p23_q = p3_q - p2_q;
    p31_q = p1_q - p3_q;
    C12_qq = p12_q.y*p1_q.x - p12_q.x*p1_q.y;
    C23_qq = p23_q.y*p2_q.x - p23_q.x*p2_q.y;
    C31_qq = p31_q.y*p3_q.x - p31_q.x*p3_q.y;
    if(p12_q.y < 0 || (p12_q.y == 0 && p12_q.x > 0)) C12_qq++;
    if(p23_q.y < 0 || (p23_q.y == 0 && p23_q.x > 0)) C23_qq++;
    if(p31_q.y < 0 || (p31_q.y == 0 && p31_q.x > 0)) C31_qq++;
    p12_qq = p12_q << fixedNumber_RESOLUTION;
    p23_qq = p23_q << fixedNumber_RESOLUTION;
    p31_qq = p31_q << fixedNumber_RESOLUTION;
}

Vec3Int edge_value2(
    const Vec2Int &p12, const Vec2Int &p23, const Vec2Int &p31,
    const int &C12, const int &C23, const int &C31,
    const Vec2Int &p
){
    int E12 = C12 + p12.x*p.y - p12.y*p.x;
    int E23 = C23 + p23.x*p.y - p23.y*p.x;
    int E31 = C31 + p31.x*p.y - p31.y*p.x;

    return Vec3Int(E12, E23, E31);
}

void DrawTriangleCurrency::draw_block(int x, int y, const TriangleEdgeData &ed, Vec4 color){
    int x0_q = x << fixedNumber_RESOLUTION;
    int y0_q = y << fixedNumber_RESOLUTION;
    int x1_q = (x + ed.block_size_minus_one) << fixedNumber_RESOLUTION;
    int y1_q = (y + ed.block_size_minus_one) << fixedNumber_RESOLUTION;
    Vec2Int corner1_q(x0_q, y0_q);
    Vec2Int corner2_q(x1_q, y0_q);
    Vec2Int corner3_q(x1_q, y1_q);
    Vec2Int corner4_q(x0_q, y1_q);
    Vec3Int corner1_E_qq = edge_value2(ed.p12_q, ed.p23_q, ed.p31_q, ed.C12_qq, ed.C23_qq, ed.C31_qq, corner1_q);
    Vec3Int corner2_E_qq = edge_value2(ed.p12_q, ed.p23_q, ed.p31_q, ed.C12_qq, ed.C23_qq, ed.C31_qq, corner2_q);
    Vec3Int corner3_E_qq = edge_value2(ed.p12_q, ed.p23_q, ed.p31_q, ed.C12_qq, ed.C23_qq, ed.C31_qq, corner3_q);
    Vec3Int corner4_E_qq = edge_value2(ed.p12_q, ed.p23_q, ed.p31_q, ed.C12_qq, ed.C23_qq, ed.C31_qq, corner4_q);
    Vec3Int corner1_E_bool = corner1_E_qq.toBool();
    Vec3Int corner2_E_bool = corner2_E_qq.toBool();
    Vec3Int corner3_E_bool = corner3_E_qq.toBool();
    Vec3Int corner4_E_bool = corner4_E_qq.toBool();
    int E1 = corner1_E_bool.x | (corner2_E_bool.x << 1) | (corner3_E_bool.x << 2) | (corner4_E_bool.x << 3);
    int E2 = corner1_E_bool.y | (corner2_E_bool.y << 1) | (corner3_E_bool.y << 2) | (corner4_E_bool.y << 3);
    int E3 = corner1_E_bool.z | (corner2_E_bool.z << 1) | (corner3_E_bool.z << 2) | (corner4_E_bool.z << 3);
    if(E1 == 0x0 || E2 == 0x0 || E3 == 0x0){
        return;
    }
    if(E1 == 0xF && E2 == 0xF && E3 == 0xF){
        for(int yi = y, yi_max = y + ed.block_size; yi < yi_max; yi++){
            for(int xi = x, xi_max = x + ed.block_size; xi < xi_max; xi++){
                ScreenHelper::putPixel(xi, yi, color);
            }
        }
    }
    else{
        for(int yi = y, yi_max = y + ed.block_size; yi < yi_max; yi++){
            int E12_qq = corner1_E_qq.x;
            int E23_qq = corner1_E_qq.y;
            int E31_qq = corner1_E_qq.z;
            for(int xi = x, xi_max = x + ed.block_size; xi < xi_max; xi++){
                if(E12_qq > 0 && E23_qq > 0 && E31_qq > 0){
                    ScreenHelper::putPixel(xi, yi, color);
                }
                E12_qq -= ed.p12_qq.y;
                E23_qq -= ed.p23_qq.y;
                E31_qq -= ed.p31_qq.y;
            }
            corner1_E_qq.x += ed.p12_qq.x;
            corner1_E_qq.y += ed.p23_qq.x;
            corner1_E_qq.z += ed.p31_qq.x;
        }
    }
}

void DrawTriangleCurrency::loop_blocks(
    Vec2Int start, int block_per_thread, TriangleEdgeData ed, Vec4 color
){
    return;
    int num_drawn_block = 0;
    for(int y = start.y; y < ed.max_block.y; y += ed.block_size){
        for(int x = start.x; x < ed.max_block.x; x += ed.block_size){
            DrawTriangleCurrency::draw_block(x, y, ed, color);
            if(++num_drawn_block >= block_per_thread) return;
        }
        start.x = ed.min_block.x;
    }
}

unsigned int hardware_thread = std::thread::hardware_concurrency();
unsigned int minimum_thread = 2;
int total_thread = std::max(hardware_thread, minimum_thread);
std::vector<std::thread> threads(total_thread - 1);
void DrawTriangleCurrency::draw(const Vec3 &p1_vp, const Vec3 &p2_vp, const Vec3 &p3_vp){
    TriangleEdgeData ed(p1_vp, p2_vp, p3_vp);
    int R = (ed.max_block.x - ed.min_block.x) / ed.block_size; // num of block each row
    int C = (ed.max_block.y - ed.min_block.y) / ed.block_size; // num of block each col
    int total_block = R * C;
    int block_per_thread = total_block / total_thread;
    Vec2Int T_block(0, 0);
    Vec2Int T_value(ed.min_block.x, ed.min_block.y);
    Vec2Int T0_value(ed.min_block.x, ed.min_block.y);
    for(int i = 0; i < total_thread - 1; i++){
        // std::cout << "block: " << i << std::endl;
        // std::cout << T_block.x << ", " << T_block.y << std::endl;
        // std::cout << T_value.x << ", " << T_value.y << std::endl;
        threads[i] = std::thread(
            &DrawTriangleCurrency::loop_blocks,
            T_value, block_per_thread, ed, indexToColor(i)
        );

        int T_block_x = T_block.x + block_per_thread;
        T_block.x = T_block_x % R;
        T_block.y = T_block.y + (T_block_x / R);
        T_value = T0_value + T_block * ed.block_size;
    }
    // std::cout << "block: " << total_thread - 1 << std::endl;
    // std::cout << T_block.x << ", " << T_block.y << std::endl;
    // std::cout << T_value.x << ", " << T_value.y << std::endl;
    DrawTriangleCurrency::loop_blocks(
        T_value, std::numeric_limits<int>::max(), ed, indexToColor(total_thread - 1)
    );
    
    for(int i = 0; i < total_thread - 1; i++){
        threads[i].join();
    }
}

// ThreadPool threadPool(total_thread - 1);
void DrawTriangleCurrency::draw2(const Vec3 &p1_vp, const Vec3 &p2_vp, const Vec3 &p3_vp){
    TriangleEdgeData ed(p1_vp, p2_vp, p3_vp);
    // for(int y = ed.min_block.y; y < ed.max_block.y; y += ed.block_size){
    //     for(int x = ed.min_block.x; x < ed.max_block.x; x += ed.block_size){
    //         // threadPool.enqueue_task([x, y, ed]{
    //             DrawTriangleCurrency::draw_block(x, y, ed, {0, 0, 0, 255});
    //         // });
    //     }
    // }
}

void DrawTriangleCurrency::renderTriangles(
    const std::vector<Triangle> *triangles,
    const std::vector<Vec3> &projecteds
){
    for(int i = 0, n = triangles->size(); i < n; i++){
        Triangle triangle = triangles->at(i);
        draw2(
            projecteds[triangle.x],
            projecteds[triangle.y],
            projecteds[triangle.z]
        );
    }
    // threadPool.main_thread();
}
