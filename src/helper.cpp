#include "helper.h"

Vec2 viewportToCanvasCoordinate(const Vec3 &vec3){
    // top-left (0, 0) bottom-right (cW, cH)
    return {
        vec3.x + canvas_half_cW,
        -vec3.y + canvas_half_cH
    };
}

void putPixel(const int &x, const int &y){
    Vec4 color = {0, 0, 0, 255};
    int offset = (x * 4) + (y * canvas_four_mul_cW);
    canvasBuffer[offset + 0] = color.x;
    canvasBuffer[offset + 1] = color.y;
    canvasBuffer[offset + 2] = color.z;
    canvasBuffer[offset + 3] = color.w;
}

void putPixel(const int &x, const int &y, const Vec4 &color){
    int offset = (x * 4) + (y * canvas_four_mul_cW);
    canvasBuffer[offset + 0] = color.x;
    canvasBuffer[offset + 1] = color.y;
    canvasBuffer[offset + 2] = color.z;
    canvasBuffer[offset + 3] = color.w;
}

void putPixel(const int &x, const int &y, const Vec4 &color, const float &dept){
    int offset;
    offset = x + (y * canvas_cW);
    if(dept <= deptBuffer[offset]){
        return;
    }
    deptBuffer[offset] = dept;

    offset = (x * 4) + (y * canvas_four_mul_cW);
    canvasBuffer[offset + 0] = color.x;
    canvasBuffer[offset + 1] = color.y;
    canvasBuffer[offset + 2] = color.z;
    canvasBuffer[offset + 3] = color.w;
}

Vec4 getTexel(
    const Vec2 &uv1, const Vec2 &uv12, const Vec2 &uv13,
    const float &_1_over_z,
    const float &u, const float &v
){
    Vec2 uv =
    divineVec(
        addVec(
            uv1,
            addVec(
                scalarVec(u, uv12),
                scalarVec(v, uv13)
            )
        ),
        _1_over_z
    );

    // std::cout << "(" << " | " << uv.x << ", " << uv.y << ") ";
    uv.x = std::clamp(uv.x, 0.0f, 1.0f);
    uv.y = std::clamp(uv.y, 0.0f, 1.0f);
    int uv_w = std::floor((crateTexture_width - 1) * uv.x);
    int uv_h = std::floor((crateTexture_height - 1) * (1 - uv.y));
    int offset = uv_w * 4 + uv_h * crateTexture_pitch;
    // std::cout << uv.x << ", " << uv.y << " | " << offset << " ";
    Vec4 texel = {
        static_cast<float>(crateTexture_pixels[offset]),
        static_cast<float>(crateTexture_pixels[offset + 1]),
        static_cast<float>(crateTexture_pixels[offset + 2]),
        static_cast<float>(crateTexture_pixels[offset + 3]),
    };

    return texel;
}
Vec4 getTexel_repeat(
    const Vec2 &uv1, const Vec2 &uv12, const Vec2 &uv13,
    const float &_1_over_z,
    const float &u, const float &v
){
    Vec2 uv =
    divineVec(
        addVec(
            uv1,
            addVec(
                scalarVec(u, uv12),
                scalarVec(v, uv13)
            )
        ),
        _1_over_z
    );

    // std::cout << "(" << uv.x << ", " << uv.y << ") ";
    uv.x = uv.x - std::floor(uv.x);
    uv.y = uv.y - std::floor(uv.y);
    int uv_w = std::floor(crateTexture_width * uv.x);
    int uv_h = std::floor(crateTexture_height * uv.y);
    // std::cout << uv.y << ", " << crateTexture_height << " | " << " ";
    int offset = uv_w * 4 + uv_h * crateTexture_pitch;
    // std::cout << uv_w << ", " << uv_h << ", " << crateTexture_height - 1 << " | " << offset << " ";
    Vec4 texel = {
        static_cast<float>(crateTexture_pixels[offset]),
        static_cast<float>(crateTexture_pixels[offset + 1]),
        static_cast<float>(crateTexture_pixels[offset + 2]),
        static_cast<float>(crateTexture_pixels[offset + 3]),
    };

    return texel;
}

float interpolate_1_over_z(
    const Vec3 &p1_o, const Vec3 &p2_o, const Vec3 &p3_o,
    const float &u, const float &v
){
    return (1/p1_o.z) + u * (1/p2_o.z - 1/p1_o.z) + v * (1/p3_o.z - 1/p1_o.z);
}

void drawFilledTriangle(Vec3 p1_o, Vec3 p2_o, Vec3 p3_o){
    Vec2 p1 = viewportToCanvasCoordinate(p1_o);
    Vec2 p2 = viewportToCanvasCoordinate(p2_o);
    Vec2 p3 = viewportToCanvasCoordinate(p3_o);

    Vec2 p12 = subVec(p2, p1);
    Vec2 p13 = subVec(p3, p1);
    float signedArea = scalarCrossVec(p12, p13);
    if(signedArea > 0){
        swapVec(p2, p3);
    }

    Vec2Int p1F = fixedNumber_fixedXY(p1);
    Vec2Int p2F = fixedNumber_fixedXY(p2);
    Vec2Int p3F = fixedNumber_fixedXY(p3);
    int xMin = std::min(p1F.x, std::min(p2F.x, p3F.x)) >> fixedNumber_RESOLUTION;
    int yMin = std::min(p1F.y, std::min(p2F.y, p3F.y)) >> fixedNumber_RESOLUTION;
    int xMax = (std::max(p1F.x, std::max(p2F.x, p3F.x)) + 15) >> fixedNumber_RESOLUTION;
    int yMax = (std::max(p1F.y, std::max(p2F.y, p3F.y)) + 15) >> fixedNumber_RESOLUTION;
    int dx12 = p2F.x - p1F.x;
    int dx23 = p3F.x - p2F.x;
    int dx31 = p1F.x - p3F.x;
    int dy12 = p2F.y - p1F.y;
    int dy23 = p3F.y - p2F.y;
    int dy31 = p1F.y - p3F.y;
    int dx12F = dx12 << fixedNumber_RESOLUTION;
    int dx23F = dx23 << fixedNumber_RESOLUTION;
    int dx31F = dx31 << fixedNumber_RESOLUTION;
    int dy12F = dy12 << fixedNumber_RESOLUTION;
    int dy23F = dy23 << fixedNumber_RESOLUTION;
    int dy31F = dy31 << fixedNumber_RESOLUTION;
    int C1 = dy12 * p1F.x - dx12 * p1F.y;
    int C2 = dy23 * p2F.x - dx23 * p2F.y;
    int C3 = dy31 * p3F.x - dx31 * p3F.y;
    if(dy12 > 0 || (dy12 == 0 && dx12 < 0)){
        C1--;
    }
    if(dy23 > 0 || (dy23 == 0 && dx23 < 0)){
        C2--;
    }
    if(dy31 > 0 || (dy31 == 0 && dx31 < 0)){
        C3--;
    }

    int q = 4;
    xMin = xMin & ~(q - 1);
    yMin = yMin & ~(q - 1);

    Vec2Int n12 = {-dy12, dx12};
    Vec2Int n23 = {-dy23, dx23};
    Vec2Int n31 = {-dy31, dx31};
    Vec2Int t12 = { n12.x <= 0 ? q - 1 : 0, n12.y <= 0 ? q - 1 : 0 };
    Vec2Int t23 = { n23.x <= 0 ? q - 1 : 0, n23.y <= 0 ? q - 1 : 0 };
    Vec2Int t31 = { n31.x <= 0 ? q - 1 : 0, n31.y <= 0 ? q - 1 : 0 };

    for(int y = yMin; y < yMax; y += q){
        for(int x = xMin; x < xMax; x += q){
            int x_t12 = ((x + t12.x) << fixedNumber_RESOLUTION) + 8;
            int y_t12 = ((y + t12.y) << fixedNumber_RESOLUTION) + 8;

            int x_t23 = ((x + t23.x) << fixedNumber_RESOLUTION) + 8;
            int y_t23 = ((y + t23.y) << fixedNumber_RESOLUTION) + 8;
            
            int x_t31 = ((x + t31.x) << fixedNumber_RESOLUTION) + 8;
            int y_t31 = ((y + t31.y) << fixedNumber_RESOLUTION) + 8;

            bool _00_12 = C1 + dx12 * y_t12 - dy12 * x_t12 < 0;
            bool _00_23 = C2 + dx23 * y_t23 - dy23 * x_t23 < 0;
            bool _00_31 = C3 + dx31 * y_t31 - dy31 * x_t31 < 0;

            if(!_00_12 || !_00_23 || !_00_31){
                // ignore block
                continue;
            }
            else{
                // partially block
                int x0 = (x << fixedNumber_RESOLUTION) + 8;
                int y0 = (y << fixedNumber_RESOLUTION) + 8;
                int cy12 = C1 + dx12 * y0 - dy12 * x0;
                int cy23 = C2 + dx23 * y0 - dy23 * x0;
                int cy31 = C3 + dx31 * y0 - dy31 * x0;
                for(int yi = y; yi < y + q; yi++){
                    int cx12 = cy12;
                    int cx23 = cy23;
                    int cx31 = cy31;
                    for(int xi = x; xi < x + q; xi++){
                        if((cx12 & cx23 & cx31) < 0){
                            putPixel(xi, yi);
                        }
                        cx12 -= dy12F;
                        cx23 -= dy23F;
                        cx31 -= dy31F;
                    }
                    cy12 += dx12F;
                    cy23 += dx23F;
                    cy31 += dx31F;
                }
            }
        }
    }
}
void drawFilledTriangle(Vec3 p1_o, Vec3 p2_o, Vec3 p3_o, const TextureCoor &textureCoor){
    Vec2 p1 = viewportToCanvasCoordinate(p1_o);
    Vec2 p2 = viewportToCanvasCoordinate(p2_o);
    Vec2 p3 = viewportToCanvasCoordinate(p3_o);

    Vec2 p12 = subVec(p2, p1);
    Vec2 p13 = subVec(p3, p1);
    float signedArea = scalarCrossVec(p12, p13);
    if(signedArea > 0){
        swapVec(p2, p3);
    }

    Vec2Int p1F = fixedNumber_fixedXY(p1);
    Vec2Int p2F = fixedNumber_fixedXY(p2);
    Vec2Int p3F = fixedNumber_fixedXY(p3);
    int xMin = std::min(p1F.x, std::min(p2F.x, p3F.x)) >> fixedNumber_RESOLUTION;
    int yMin = std::min(p1F.y, std::min(p2F.y, p3F.y)) >> fixedNumber_RESOLUTION;
    int xMax = (std::max(p1F.x, std::max(p2F.x, p3F.x)) + 15) >> fixedNumber_RESOLUTION;
    int yMax = (std::max(p1F.y, std::max(p2F.y, p3F.y)) + 15) >> fixedNumber_RESOLUTION;
    int dx12 = p2F.x - p1F.x;
    int dx23 = p3F.x - p2F.x;
    int dx31 = p1F.x - p3F.x;
    int dy12 = p2F.y - p1F.y;
    int dy23 = p3F.y - p2F.y;
    int dy31 = p1F.y - p3F.y;
    int dx12F = dx12 << fixedNumber_RESOLUTION;
    int dx23F = dx23 << fixedNumber_RESOLUTION;
    int dx31F = dx31 << fixedNumber_RESOLUTION;
    int dy12F = dy12 << fixedNumber_RESOLUTION;
    int dy23F = dy23 << fixedNumber_RESOLUTION;
    int dy31F = dy31 << fixedNumber_RESOLUTION;
    int C1 = dy12 * p1F.x - dx12 * p1F.y;
    int C2 = dy23 * p2F.x - dx23 * p2F.y;
    int C3 = dy31 * p3F.x - dx31 * p3F.y;
    if(dy12 > 0 || (dy12 == 0 && dx12 < 0)){
        C1--;
    }
    if(dy23 > 0 || (dy23 == 0 && dx23 < 0)){
        C2--;
    }
    if(dy31 > 0 || (dy31 == 0 && dx31 < 0)){
        C3--;
    }

    int q = 4;
    xMin = xMin & ~(q - 1);
    yMin = yMin & ~(q - 1);

    Vec2Int n12 = {-dy12, dx12};
    Vec2Int n23 = {-dy23, dx23};
    Vec2Int n31 = {-dy31, dx31};
    Vec2Int t12 = { n12.x <= 0 ? q - 1 : 0, n12.y <= 0 ? q - 1 : 0 };
    Vec2Int t23 = { n23.x <= 0 ? q - 1 : 0, n23.y <= 0 ? q - 1 : 0 };
    Vec2Int t31 = { n31.x <= 0 ? q - 1 : 0, n31.y <= 0 ? q - 1 : 0 };

    int area = scalarCrossVec(
        subVec(p2F, p1F),
        subVec(p3F, p1F)
    );
    Vec2 uv1 = {textureCoor.uv1.x, textureCoor.uv1.y};
    Vec2 uv2 = {textureCoor.uv2.x, textureCoor.uv2.y};
    Vec2 uv3 = {textureCoor.uv3.x, textureCoor.uv3.y};
    if(signedArea > 0){
        swapVec(uv2, uv3);
        swapVec(p2_o, p3_o);
    }
    uv1 = divineVec(uv1, p1_o.z);
    uv2 = divineVec(uv2, p2_o.z);
    uv3 = divineVec(uv3, p3_o.z);
    Vec2 uv12 = subVec(uv2, uv1);
    Vec2 uv13 = subVec(uv3, uv1);

    for(int y = yMin; y < yMax; y += q){
        for(int x = xMin; x < xMax; x += q){
            int x_t12 = ((x + t12.x) << fixedNumber_RESOLUTION) + 8;
            int y_t12 = ((y + t12.y) << fixedNumber_RESOLUTION) + 8;

            int x_t23 = ((x + t23.x) << fixedNumber_RESOLUTION) + 8;
            int y_t23 = ((y + t23.y) << fixedNumber_RESOLUTION) + 8;
            
            int x_t31 = ((x + t31.x) << fixedNumber_RESOLUTION) + 8;
            int y_t31 = ((y + t31.y) << fixedNumber_RESOLUTION) + 8;

            bool _00_12 = C1 + dx12 * y_t12 - dy12 * x_t12 < 0;
            bool _00_23 = C2 + dx23 * y_t23 - dy23 * x_t23 < 0;
            bool _00_31 = C3 + dx31 * y_t31 - dy31 * x_t31 < 0;

            if(!_00_12 || !_00_23 || !_00_31){
                // ignore block
                continue;
            }
            else{
                // partially block
                int x0 = (x << fixedNumber_RESOLUTION) + 8;
                int y0 = (y << fixedNumber_RESOLUTION) + 8;
                int cy12 = C1 + dx12 * y0 - dy12 * x0;
                int cy23 = C2 + dx23 * y0 - dy23 * x0;
                int cy31 = C3 + dx31 * y0 - dy31 * x0;
                for(int yi = y; yi < y + q; yi++){
                    int cx12 = cy12;
                    int cx23 = cy23;
                    int cx31 = cy31;
                    for(int xi = x; xi < x + q; xi++){
                        if((cx12 & cx23 & cx31) < 0){
                            //
                            float u = static_cast<float>(cx31) / area;
                            float v = static_cast<float>(cx12) / area;
                            float _1_over_z = interpolate_1_over_z(p1_o, p2_o, p3_o, u, v);
                            // std::cout << u << ", " << v << std::endl;
                            Vec4 color = getTexel(uv1, uv12, uv13, _1_over_z, u, v);
                            // require fixing
                            // Vec4 color = getTexel_repeat(uv1, uv12, uv13, _1_over_z, u, v);

                            //
                            // putPixel(xi, yi);
                            putPixel(xi, yi, color, _1_over_z);
                        }
                        cx12 -= dy12F;
                        cx23 -= dy23F;
                        cx31 -= dy31F;
                    }
                    cy12 += dx12F;
                    cy23 += dx23F;
                    cy31 += dx31F;
                }
            }
        }
    }
}
Vec3Int edge_value(
    const Vec2Int &p12, const Vec2Int &p23, const Vec2Int &p31,
    const int &C12, const int &C23, const int &C31,
    const Vec2Int &p
){
    int E12 = C12 + p12.x*p.y - p12.y*p.x;
    int E23 = C23 + p23.x*p.y - p23.y*p.x;
    int E31 = C31 + p31.x*p.y - p31.y*p.x;

    return Vec3Int(E12, E23, E31);
}
Vec3Int edge_value(
    const Vec2Int &p12, const Vec2Int &p23, const Vec2Int &p31,
    const int &C12, const int &C23, const int &C31,
    const Vec2Int &s12, const Vec2Int &s23, const Vec2Int &s31
){
    int E12 = C12 + p12.x*s12.y - p12.y*s12.x;
    int E23 = C23 + p23.x*s23.y - p23.y*s23.x;
    int E31 = C31 + p31.x*s31.y - p31.y*s31.x;

    return Vec3Int(E12, E23, E31);
}
void drawFilledTriangle_test1(Vec3 p1_o, Vec3 p2_o, Vec3 p3_o){
    Vec2 p1_f = viewportToCanvasCoordinate(p1_o);
    Vec2 p2_f = viewportToCanvasCoordinate(p2_o);
    Vec2 p3_f = viewportToCanvasCoordinate(p3_o);
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
    Vec2Int p12_q = p2_q - p1_q;
    Vec2Int p23_q = p3_q - p2_q;
    Vec2Int p31_q = p1_q - p3_q;
    int C12_qq = p12_q.y*p1_q.x - p12_q.x*p1_q.y;
    int C23_qq = p23_q.y*p2_q.x - p23_q.x*p2_q.y;
    int C31_qq = p31_q.y*p3_q.x - p31_q.x*p3_q.y;
    if(p12_q.y < 0 || (p12_q.y == 0 && p12_q.x > 0)) C12_qq++;
    if(p23_q.y < 0 || (p23_q.y == 0 && p23_q.x > 0)) C23_qq++;
    if(p31_q.y < 0 || (p31_q.y == 0 && p31_q.x > 0)) C31_qq++;
    Vec2Int p12_qq = p12_q << fixedNumber_RESOLUTION;
    Vec2Int p23_qq = p23_q << fixedNumber_RESOLUTION;
    Vec2Int p31_qq = p31_q << fixedNumber_RESOLUTION;
    int block_size = 8;
    int block_size_minus_one = block_size - 1;
    int block_size_bitmask = ~(block_size_minus_one);
    min.x &= block_size_bitmask;
    min.y &= block_size_bitmask;
    for(int y = min.y; y <= max.y; y += block_size){
        for(int x = min.x; x <= max.x; x += block_size){
            int x0_q = x << fixedNumber_RESOLUTION;
            int y0_q = y << fixedNumber_RESOLUTION;
            int x1_q = (x + block_size_minus_one) << fixedNumber_RESOLUTION;
            int y1_q = (y + block_size_minus_one) << fixedNumber_RESOLUTION;
            Vec2Int corner1_q(x0_q, y0_q);
            Vec2Int corner2_q(x1_q, y0_q);
            Vec2Int corner3_q(x1_q, y1_q);
            Vec2Int corner4_q(x0_q, y1_q);
            Vec3Int corner1_E_qq = edge_value(p12_q, p23_q, p31_q, C12_qq, C23_qq, C31_qq, corner1_q);
            Vec3Int corner2_E_qq = edge_value(p12_q, p23_q, p31_q, C12_qq, C23_qq, C31_qq, corner2_q);
            Vec3Int corner3_E_qq = edge_value(p12_q, p23_q, p31_q, C12_qq, C23_qq, C31_qq, corner3_q);
            Vec3Int corner4_E_qq = edge_value(p12_q, p23_q, p31_q, C12_qq, C23_qq, C31_qq, corner4_q);
            Vec3Int corner1_E_bool = corner1_E_qq.toBool();
            Vec3Int corner2_E_bool = corner2_E_qq.toBool();
            Vec3Int corner3_E_bool = corner3_E_qq.toBool();
            Vec3Int corner4_E_bool = corner4_E_qq.toBool();
            int E1 = corner1_E_bool.x | (corner2_E_bool.x << 1) | (corner3_E_bool.x << 2) | (corner4_E_bool.x << 3);
            int E2 = corner1_E_bool.y | (corner2_E_bool.y << 1) | (corner3_E_bool.y << 2) | (corner4_E_bool.y << 3);
            int E3 = corner1_E_bool.z | (corner2_E_bool.z << 1) | (corner3_E_bool.z << 2) | (corner4_E_bool.z << 3);
            if(E1 == 0x0 || E2 == 0x0 || E3 == 0x0){
                continue;
            }
            if(E1 == 0xF && E2 == 0xF && E3 == 0xF){
                for(int yi = y, yi_max = y + block_size; yi < yi_max; yi++){
                    for(int xi = x, xi_max = x + block_size; xi < xi_max; xi++){
                        putPixel(xi, yi);
                    }
                }
            }
            else{
                for(int yi = y, yi_max = y + block_size; yi < yi_max; yi++){
                    int E12_qq = corner1_E_qq.x;
                    int E23_qq = corner1_E_qq.y;
                    int E31_qq = corner1_E_qq.z;
                    for(int xi = x, xi_max = x + block_size; xi < xi_max; xi++){
                        if(E12_qq > 0 && E23_qq > 0 && E31_qq > 0){
                            putPixel(xi, yi);
                        }
                        E12_qq -= p12_qq.y;
                        E23_qq -= p23_qq.y;
                        E31_qq -= p31_qq.y;
                    }
                    corner1_E_qq.x += p12_qq.x;
                    corner1_E_qq.y += p23_qq.x;
                    corner1_E_qq.z += p31_qq.x;
                }
            }
        }
    }
}
void drawFilledTriangle_test(Vec3 p1_o, Vec3 p2_o, Vec3 p3_o){
    Vec2 p1_f = viewportToCanvasCoordinate(p1_o);
    Vec2 p2_f = viewportToCanvasCoordinate(p2_o);
    Vec2 p3_f = viewportToCanvasCoordinate(p3_o);
    // Vec2 p1_f(20, 20);
    // Vec2 p2_f(100, 400);
    // Vec2 p3_f(400, 20);
    Vec2 p12_f = p2_f - p1_f;
    Vec2 p13_f = p3_f - p1_f;
    // if(scalarCrossVec(p12_f, p13_f) < 0) return;
    if(scalarCrossVec(p12_f, p13_f) < 0){
        swapVec(p2_f, p3_f); // to cw to calc
    }
    Vec2Int p1_q = fixedNumber_fixedXY(p1_f);
    Vec2Int p2_q = fixedNumber_fixedXY(p2_f);
    Vec2Int p3_q = fixedNumber_fixedXY(p3_f);
    Vec2Int min, max;
    min.x = (std::min(std::min(p1_q.x, p2_q.x), p3_q.x) + fixedNumber_CEIL) >> fixedNumber_RESOLUTION;
    min.y = (std::min(std::min(p1_q.y, p2_q.y), p3_q.y) + fixedNumber_CEIL) >> fixedNumber_RESOLUTION;
    max.x = (std::max(std::max(p1_q.x, p2_q.x), p3_q.x) + fixedNumber_CEIL) >> fixedNumber_RESOLUTION;
    max.y = (std::max(std::max(p1_q.y, p2_q.y), p3_q.y) + fixedNumber_CEIL) >> fixedNumber_RESOLUTION;
    Vec2Int p12_q = p2_q - p1_q;
    Vec2Int p23_q = p3_q - p2_q;
    Vec2Int p31_q = p1_q - p3_q;
    int C12_qq = p12_q.y*p1_q.x - p12_q.x*p1_q.y;
    int C23_qq = p23_q.y*p2_q.x - p23_q.x*p2_q.y;
    int C31_qq = p31_q.y*p3_q.x - p31_q.x*p3_q.y;
    if(p12_q.y < 0 || (p12_q.y == 0 && p12_q.x > 0)) C12_qq++;
    if(p23_q.y < 0 || (p23_q.y == 0 && p23_q.x > 0)) C23_qq++;
    if(p31_q.y < 0 || (p31_q.y == 0 && p31_q.x > 0)) C31_qq++;
    Vec2Int p12_qq = p12_q << fixedNumber_RESOLUTION;
    Vec2Int p23_qq = p23_q << fixedNumber_RESOLUTION;
    Vec2Int p31_qq = p31_q << fixedNumber_RESOLUTION;
    int block_size = 4;
    int block_size_minus_one = block_size - 1;
    int block_size_bitmask = ~(block_size_minus_one);
    min.x &= block_size_bitmask;
    min.y &= block_size_bitmask;
    Vec2Int n12_q(-p12_q.y, p12_q.x);
    Vec2Int n23_q(-p23_q.y, p23_q.x);
    Vec2Int n31_q(-p31_q.y, p31_q.x);
    Vec2Int t12(n12_q.x >= 0 ? block_size_minus_one : 0, n12_q.y >= 0 ? block_size_minus_one : 0);
    Vec2Int t23(n23_q.x >= 0 ? block_size_minus_one : 0, n23_q.y >= 0 ? block_size_minus_one : 0);
    Vec2Int t31(n31_q.x >= 0 ? block_size_minus_one : 0, n31_q.y >= 0 ? block_size_minus_one : 0);
    for(int y = min.y; y <= max.y; y += block_size){
        for(int x = min.x; x <= max.x; x += block_size){
            Vec2Int s12_q = (Vec2Int(x, y) + t12) << fixedNumber_RESOLUTION;
            Vec2Int s23_q = (Vec2Int(x, y) + t23) << fixedNumber_RESOLUTION;
            Vec2Int s31_q = (Vec2Int(x, y) + t31) << fixedNumber_RESOLUTION;
            Vec3Int E_qq = edge_value(p12_q, p23_q, p31_q, C12_qq, C23_qq, C31_qq, s12_q, s23_q, s31_q);
            Vec3Int E_qq_bool = E_qq.toBool();
            if(!E_qq_bool.x || !E_qq_bool.y || !E_qq_bool.z){
                continue;
            }
            else{
                Vec2Int p_q = Vec2Int(x, y) << fixedNumber_RESOLUTION;
                E_qq = edge_value(p12_q, p23_q, p31_q, C12_qq, C23_qq, C31_qq, p_q);
                for(int yi = y, yi_max = y + block_size; yi < yi_max; yi++){
                    int E12_qq = E_qq.x;
                    int E23_qq = E_qq.y;
                    int E31_qq = E_qq.z;
                    for(int xi = x, xi_max = x + block_size; xi < xi_max; xi++){
                        if(E12_qq > 0 && E23_qq > 0 && E31_qq > 0){
                            putPixel(xi, yi);
                        }
                        E12_qq -= p12_qq.y;
                        E23_qq -= p23_qq.y;
                        E31_qq -= p31_qq.y;
                    }
                    E_qq.x += p12_qq.x;
                    E_qq.y += p23_qq.x;
                    E_qq.z += p31_qq.x;
                }
            }
        }
    }
}

void renderTriangle(
    const Triangle &triangle,
    const TextureCoor &textureCoor,
    const std::vector<Vec3> &projecteds
){
    drawFilledTriangle(
        projecteds[triangle.x],
        projecteds[triangle.y],
        projecteds[triangle.z],
        textureCoor
    );
}
void renderTriangle(
    const Triangle &triangle,
    const std::vector<Vec3> &projecteds
){
    drawFilledTriangle(
        projecteds[triangle.x],
        projecteds[triangle.y],
        projecteds[triangle.z]
    );
    // drawFilledTriangle_test1(
    //     projecteds[triangle.x],
    //     projecteds[triangle.y],
    //     projecteds[triangle.z]
    // );
    // DrawTriangleCurrency::draw(
    //     projecteds[triangle.x],
    //     projecteds[triangle.y],
    //     projecteds[triangle.z]
    // );
}

M4x4 m_scale;
M4x4 m_rotation;
M4x4 m_rotation_X;
M4x4 m_rotation_Y;
M4x4 m_rotation_Z;
M4x4 m_translation;
M4x4 m_modeltransform;
void makeModelTransform(const Transform &transform){
    m_scale.init(
        transform.scale, 0, 0, 0,
        0, transform.scale, 0, 0,
        0, 0, transform.scale, 0,
        0, 0, 0, 1
    );
    float rotationXRad = transform.rotation.x * M_PI / 180;
    float rotationYRad = transform.rotation.y * M_PI / 180;
    float rotationZRad = transform.rotation.z * M_PI / 180;
    m_rotation_X.init(
        1, 0, 0, 0,
        0, std::cos(rotationXRad), -std::sin(rotationXRad), 0,
        0, std::sin(rotationXRad), std::cos(rotationXRad), 0,
        0, 0, 0, 1
    );
    m_rotation_Y.init(
        std::cos(rotationYRad), 0, std::sin(rotationYRad), 0,
        0, 1, 0, 0,
        -std::sin(rotationYRad), 0, std::cos(rotationYRad), 0,
        0, 0, 0, 1
    );
    m_rotation_Z.init(
        std::cos(rotationZRad), -std::sin(rotationZRad), 0, 0,
        std::sin(rotationZRad), std::cos(rotationZRad), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
    avx256_multi_matrix_4x4_4x4(m_rotation_Z.value, m_rotation_Y.value, m_rotation.value);
    avx256_multi_matrix_4x4_4x4(m_rotation.value, m_rotation_X.value, m_rotation.value);
    m_translation.init(
        1, 0, 0, transform.translation.x,
        0, 1, 0, transform.translation.y,
        0, 0, 1, transform.translation.z,
        0, 0, 0, 1
    );

    avx256_multi_matrix_4x4_4x4(m_translation.value, m_rotation.value, m_modeltransform.value);
    avx256_multi_matrix_4x4_4x4(m_modeltransform.value, m_scale.value, m_modeltransform.value);
}

M4x4 m_camera;
void makeCameraTransform(){
    m_translation.init(
        1, 0, 0, -camera.transform.translation.x,
        0, 1, 0, -camera.transform.translation.y,
        0, 0, 1, -camera.transform.translation.z,
        0, 0, 0, 1
    );
    float rotationXRad = -camera.transform.rotation.x * M_PI / 180;
    float rotationYRad = -camera.transform.rotation.y * M_PI / 180;
    float rotationZRad = -camera.transform.rotation.z * M_PI / 180;
    m_rotation_X.init(
        1, 0, 0, 0,
        0, std::cos(rotationXRad), -std::sin(rotationXRad), 0,
        0, std::sin(rotationXRad), std::cos(rotationXRad), 0,
        0, 0, 0, 1
    );
    m_rotation_Y.init(
        std::cos(rotationYRad), 0, std::sin(rotationYRad), 0,
        0, 1, 0, 0,
        -std::sin(rotationYRad), 0, std::cos(rotationYRad), 0,
        0, 0, 0, 1
    );
    m_rotation_Z.init(
        std::cos(rotationZRad), -std::sin(rotationZRad), 0, 0,
        std::sin(rotationZRad), std::cos(rotationZRad), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
    avx256_multi_matrix_4x4_4x4(m_rotation_Z.value, m_rotation_Y.value, m_rotation.value);
    avx256_multi_matrix_4x4_4x4(m_rotation.value, m_rotation_X.value, m_rotation.value);

    avx256_multi_matrix_4x4_4x4(m_rotation.value, m_translation.value, m_camera.value);
}
void makeCameraTransform2(const BaseCamera &currentCamera){
    currentCamera.getViewMatrix(m_camera);
}

unsigned int clippingPlaneLength = 5;
Plane *clippingPlanes = new Plane[clippingPlaneLength];
bool clippingPlanesInited = false;

void initClippingPlanes(){
    if(!clippingPlanesInited){
        // nearPlane
        clippingPlanes[0].normal = {0, 0, 1};
        clippingPlanes[0].D = -camera.distanceToViewport;
        //leftPlane
        clippingPlanes[1].normal = {1 / std::sqrt((float)2), 0, 1 / std::sqrt((float)2)};
        clippingPlanes[1].D = 0;
        //rightPlane
        clippingPlanes[2].normal = {-1 / std::sqrt((float)2), 0, 1 / std::sqrt((float)2)};
        clippingPlanes[2].D = 0;
        //bottomPlane
        clippingPlanes[3].normal = {0, 1 / std::sqrt((float)2), 1 / std::sqrt((float)2)};
        clippingPlanes[3].D = 0;
        //topPlane
        clippingPlanes[4].normal = {0, -1 / std::sqrt((float)2), 1 / std::sqrt((float)2)};
        clippingPlanes[4].D = 0;

        clippingPlanesInited = true;
    }
}

Sphere getBoudingSphere(const std::vector<Vec3> &verticies){
    Sphere result;

    for(int i = 0, n = verticies.size(); i < n; i++){
        Vec3 vertex = verticies[i];
        result.center.x += vertex.x;
        result.center.y += vertex.y;
        result.center.z += vertex.z;
    }
    result.center.x /= verticies.size();
    result.center.y /= verticies.size();
    result.center.z /= verticies.size();

    for(int i = 0, n = verticies.size(); i < n; i++){
        Vec3 vertex = verticies[i];
        float distanceVertexAndCenter = lengthVec(subVec(vertex, result.center));
        if(result.radius < distanceVertexAndCenter){
            result.radius = distanceVertexAndCenter;
        }
    }

    return result;
}

ClippingPlaneStatus clipWholeObject(const Sphere &boundingSphere){
    ClippingPlaneStatus status;

    for(int i = 0; i < clippingPlaneLength; i++){
        Plane plane = clippingPlanes[i];
        float d = distancePointToPlane(plane, boundingSphere.center);
        if(d < -(boundingSphere.radius)){
            status.rears.push_back(plane);
            break;
        }
        if(d > boundingSphere.radius){
            status.fronts.push_back(plane);
            continue;
        }
        if(std::abs(d) < boundingSphere.radius){
            status.intersects.push_back(plane);
            continue;
        }
        std::cout << "clipWholeObject err" << std::endl;
    }

    return status;
}

void clipTriangle(
    const std::vector<Plane> &intersectPlanes,
    std::vector<Vec3> &verticies,
    const Instance &instance,
    ClippingInfo &clippingInfo
){
    std::vector<Triangle> trianglesWaitingProcess = instance.model->triangles;
    std::vector<TextureCoor> textureCoorsWaitingProcess = instance.model->textureCoors;
    std::vector<int> trianglesWaitingProcess_plane(trianglesWaitingProcess.size(), 0);
    clippingInfo.triangles.reserve(trianglesWaitingProcess.size());
    clippingInfo.textureCoors.reserve(trianglesWaitingProcess.size());
    
    while(!trianglesWaitingProcess.empty()){
        Triangle triangle = trianglesWaitingProcess.back();
        trianglesWaitingProcess.pop_back();
        TextureCoor textureCoor = textureCoorsWaitingProcess.back();
        textureCoorsWaitingProcess.pop_back();
        int startPlane = trianglesWaitingProcess_plane.back();
        trianglesWaitingProcess_plane.pop_back();
        int front = 0;
        for(int i = startPlane, n = intersectPlanes.size(); i < n; i++){
            Plane plane = intersectPlanes[i];
            Vec3 vertexA = verticies[triangle.x];
            Vec3 vertexB = verticies[triangle.y];
            Vec3 vertexC = verticies[triangle.z];
            float dA = distancePointToPlane(plane, vertexA);
            float dB = distancePointToPlane(plane, vertexB);
            float dC = distancePointToPlane(plane, vertexC);

            if(dA < 0 && dB < 0 && dC < 0){
                break;
            }
            else if(dA >= 0 && dB >= 0 && dC >= 0){
                front++;
                continue;
            }
            else{
                if(oneVertexFront(
                    plane, dA, dB, dC, vertexA, vertexB, vertexC, triangle, textureCoor,
                    verticies, trianglesWaitingProcess, textureCoorsWaitingProcess, trianglesWaitingProcess_plane, i,
                    clippingInfo.ignoredVertexIndexSet
                )){
                    break;
                }
                else if(twoVertexFront(
                    plane, dA, dB, dC, vertexA, vertexB, vertexC, triangle, textureCoor,
                    verticies, trianglesWaitingProcess, textureCoorsWaitingProcess, trianglesWaitingProcess_plane, i,
                    clippingInfo.ignoredVertexIndexSet
                )){
                    break;
                }
            }
        }
        if(front == intersectPlanes.size() - startPlane){
            clippingInfo.triangles.push_back(triangle);
            clippingInfo.textureCoors.push_back(textureCoor);
        }
    }
}

ClippingInfo::ClippingInfo(){
    status = ClippingStatus::COMPLETE;
};

ClippingInfo clipping(
    std::vector<Vec3> &applieds,
    const Instance &instance
){
    ClippingInfo clippingInfo;

    initClippingPlanes();
    ClippingPlaneStatus status = clipWholeObject(getBoudingSphere(applieds));
    if(status.rears.size() > 0){
        clippingInfo.status = ClippingStatus::COMPLETE;
        
        return clippingInfo;
    }
    if(status.fronts.size() == clippingPlaneLength){
        clippingInfo.status = ClippingStatus::NO;
        clippingInfo.triangles = instance.model->triangles; // copy
        clippingInfo.textureCoors = instance.model->textureCoors; // copy

        return clippingInfo;
    }

    clippingInfo.status = ClippingStatus::PARTIAL;
    clipTriangle(status.intersects, applieds, instance, clippingInfo);

    return clippingInfo;
}

M4x4 m_cameramodel;
M4x1 m4x1_cache_0;
M4x1 m4x1_cache_1;
std::vector<Vec3> apply(const Instance &instance, const BaseCamera &currentCamera){
    makeModelTransform(instance.transform);
    makeCameraTransform();
    // makeCameraTransform2(currentCamera);
    avx256_multi_matrix_4x4_4x4(
        m_camera.value,
        m_modeltransform.value,
        m_cameramodel.value
    );

    std::vector<Vec3> applieds;
    applieds.reserve(instance.model->vertices.size());

    for(int i = 0, n = instance.model->vertices.size(); i < n; i++){
        vec3_to_M4x1(instance.model->vertices[i], m4x1_cache_0);
        avx256_multi_matrix_4x4_4x1(
            m_cameramodel.value,
            m4x1_cache_0.value,
            m4x1_cache_1.value
        );
        Vec3 applied = matrix_to_vec3(m4x1_cache_1);
        applieds.push_back(applied);
    }

    return applieds;
}

M3x4 m_projection;
void makeProjectionTransform(){
    m_projection.init(
        camera.distanceToViewport * canvas_cW / viewport_vW, 0, 0, 0,
        0, camera.distanceToViewport * canvas_cH / viewport_vH, 0, 0,
        0, 0, 1, 0
    );
}

M3x1 m3x1_cache_0;
void project(std::vector<Vec3> &applieds, const ClippingInfo &clippingInfo){
    makeProjectionTransform();

    for(int i = 0, n = applieds.size(); i < n; i++){
        if(clippingInfo.ignoredVertexIndexSet.count(i)) continue;
        Vec3 applied = applieds[i];
        vec3_to_M4x1(applied, m4x1_cache_0);
        avx256_multi_matrix_3x4_4x1(
            m_projection.value,
            m4x1_cache_0.value,
            m3x1_cache_0.value
        );
        
        Vec3 projected = homogeneous3DToCartesian(
            matrix_to_vec3(m3x1_cache_0)
        );
        applieds[i] = projected;
    }
}
Vec3 project(const Vec3 &worldPoint){
    makeProjectionTransform();

    vec3_to_M4x1(worldPoint, m4x1_cache_0);
    avx256_multi_matrix_3x4_4x1(
        m_projection.value,
        m4x1_cache_0.value,
        m3x1_cache_0.value
    );
    Vec3 projected = homogeneous3DToCartesian(
        matrix_to_vec3(m3x1_cache_0)
    );

    return projected;
}

void backface_culling(ClippingInfo &clippingInfo, const std::vector<Vec3> &applieds){
    std::vector<Triangle> triangles_result;
    std::vector<TextureCoor> textureCoors_result;
    for(int i = 0, n = clippingInfo.triangles.size(); i < n; i++){
        Triangle trig = clippingInfo.triangles[i];
        Vec3 v0 = applieds[trig.x];
        Vec3 v1 = applieds[trig.y];
        Vec3 v2 = applieds[trig.z];
        Vec3 v01 = v1 - v0;
        Vec3 v02 = v2 - v0;
        Vec3 to_cam = normalizeVec(Vec3(0, 0, 0) - v0);
        Vec3 normal = normalizeVec(crossVec(v01, v02));
        float cos = dot(normal, to_cam);
        // std::cout << vecToString(to_cam) << " ";
        // std::cout << cos << " ";
        if(cos >= 0){
            triangles_result.push_back(trig);
            textureCoors_result.push_back(clippingInfo.textureCoors[i]);
        }
    }
    // std::cout << std::endl;
    clippingInfo.triangles = triangles_result;
    clippingInfo.textureCoors = textureCoors_result;
}

// void check_trig_not_cw(ClippingInfo &clippingInfo, const std::vector<Vec3> &applieds){
//     for(int i = 0, n = clippingInfo.triangles.size(); i < n; i++){
//         Triangle trig = clippingInfo.triangles[i];
//         Vec3 v0 = applieds[trig.x];
//         Vec3 v1 = applieds[trig.y];
//         Vec3 v2 = applieds[trig.z];
//     }
// }

void clear_screen(){
    std::fill_n(canvasBuffer, canvasBufferLength, 255);
    std::fill_n(deptBuffer, deptBufferLength, -std::numeric_limits<float>::max());
}

void render_instance(const Instance &instance, const BaseCamera &currentCamera, int idx){
    std::vector<Vec3> applieds = apply(instance, currentCamera);

    ClippingInfo clippingInfo = clipping(applieds, instance);
    if(clippingInfo.status == ClippingStatus::COMPLETE){
        return;
    }

    // backface_culling(clippingInfo, applieds);
    // std::cout << clippingInfo.triangles.size() << std::endl;

    project(applieds, clippingInfo);
    
    // DrawTriangleCurrency::renderTriangles(clippingInfo.triangles, applieds);
    for(int i = 0, n = clippingInfo.triangles.size(); i < n; i++){
        Triangle triangle = clippingInfo.triangles.at(i);
        // TextureCoor textureCoor = clippingInfo.textureCoors->at(i);
        // if(i == idx){
            renderTriangle(triangle, applieds);
            // renderTriangle(triangle, textureCoor, applieds);
        // }
    }
}
