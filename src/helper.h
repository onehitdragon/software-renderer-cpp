#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include <algorithm>
#include <unordered_set>
#include "global.h"
#include "common/camera.h"
#include "common/matrix.h"
#include "common/plane.h"
#include "common/sphere.h"
#include "common/fixednumber.h"
#include "asset.h"
#include "helpercurrency.h"

enum class ClippingStatus{
    COMPLETE,
    NO,
    PARTIAL
};

class ClippingInfo{
public:
    ClippingStatus status;
    std::vector<Triangle> triangles;
    std::vector<TextureCoor> textureCoors;
    std::unordered_set<int> ignoredVertexIndexSet;
    ClippingInfo();
};

// class LineClippingInfo{
//     ClippingStatus status;
//     Vec3 start;
//     Vec3 end;
// };

Vec2 viewportToCanvasCoordinate(const Vec3 &vec3);
void putPixel(const int &x, const int &y, const Vec4 &color);
void putPixel(const int &x, const int &y, const Vec4 &color, const float &dept);
Vec3 project(const Vec3 &worldPoint);
void clear_screen();
void render_instance(const Instance &instance, const BaseCamera &currentCamera, int idx);

void drawFilledTriangle_test(Vec3 p1_o, Vec3 p2_o, Vec3 p3_o);

#endif