#ifndef DRAWHELPER_H
#define DRAWHELPER_H

#include <iostream>
#include "common/vector.h"
#include "helper.h"

class DrawHelper{
public:
    static void drawLine(const Vec3 &start, const Vec3 &end, const Vec4 &color);
};

#endif