#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include "matrix.h"

class BaseCamera{
public:
    virtual void getViewMatrix(M4x4 &viewMatrix) const = 0;
};

class FPSCamera: public BaseCamera{
public:
    Vec3 position;
    float yaw;
    float pitch;
    float speed;
    Vec3 forward;
    Vec3 right;
    Vec3 up;
    FPSCamera();
    void update(const Vec2 &mouseDir);
    void getViewMatrix(M4x4 &viewMatrix) const override;
};

#endif