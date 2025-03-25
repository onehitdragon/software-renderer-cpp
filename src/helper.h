#ifndef HELPER_H
#define HELPER_H

#include "global.h"

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

void render_instance(const Instance &instance, int idx);

#endif