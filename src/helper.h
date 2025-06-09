#ifndef HELPER_H
#define HELPER_H

#include "global.h"
#include "common/camera.h"
#include <unordered_set>

enum class ClippingStatus{
    COMPLETE,
    NO,
    PARTIAL
};

class ClippingInfo{
public:
    ClippingStatus status;
    std::vector<Triangle> *triangles;
    std::vector<TextureCoor> *textureCoors;
    std::unordered_set<int> ignoredVertexIndexSet;
    ClippingInfo();
    ~ClippingInfo();
};

Vec2 viewportToCanvasCoordinate(const Vec3 &vec3);
void putPixel(const int &x, const int &y, const Vec4 &color, const float &dept);
Vec3 project(const Vec3 &worldPoint);
void clear_screen();
void render_instance(const Instance &instance, const BaseCamera &currentCamera, int idx);

#endif