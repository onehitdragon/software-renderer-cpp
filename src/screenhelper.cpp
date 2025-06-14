#include "screenhelper.h"

Vec2 ScreenHelper::viewportToCanvasCoordinate(const Vec3 &vec3){
    // top-left (0, 0) bottom-right (cW, cH)
    return {
        vec3.x + canvas_half_cW,
        -vec3.y + canvas_half_cH
    };
}

void ScreenHelper::putPixel(const int &x, const int &y){
    Vec4 color = {0, 0, 0, 255};
    int offset = (x * 4) + (y * canvas_four_mul_cW);
    canvasBuffer[offset + 0] = color.x;
    canvasBuffer[offset + 1] = color.y;
    canvasBuffer[offset + 2] = color.z;
    canvasBuffer[offset + 3] = color.w;
}

void ScreenHelper::putPixel(const int &x, const int &y, const Vec4 &color){
    int offset = (x * 4) + (y * canvas_four_mul_cW);
    canvasBuffer[offset + 0] = color.x;
    canvasBuffer[offset + 1] = color.y;
    canvasBuffer[offset + 2] = color.z;
    canvasBuffer[offset + 3] = color.w;
}