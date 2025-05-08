#ifndef HELPER_H
#define HELPER_H

#include "global.h"
#include "common/camera.h"

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
    ClippingInfo();
    ~ClippingInfo();
};

void clear_screen();
void render_instance(const Instance &instance, const BaseCamera &currentCamera, int idx);

#endif