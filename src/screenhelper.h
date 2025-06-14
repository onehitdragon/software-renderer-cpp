#ifndef SCREENHELPER_H
#define SCREENHELPER_H

#include "global.h"
#include "common/vector.h"

class ScreenHelper{
public:
    static Vec2 viewportToCanvasCoordinate(const Vec3 &vec3);
    static void putPixel(const int &x, const int &y);
    static void putPixel(const int &x, const int &y, const Vec4 &color);
};

#endif