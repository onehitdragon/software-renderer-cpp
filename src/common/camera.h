#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include "matrix.h"
#include "quaternion.h"

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

class ArcballCamera: public BaseCamera{
public:
    float radius;
    Quaternion rotated;
    Quaternion rotatedInverse;
    Vec2 prevNdcMouse;
    ArcballCamera();
    void press(Vec2 ndcMouse);
    void move(Vec2 ndcMouse);
    void release();
    void getViewMatrix(M4x4 &viewMatrix) const override;
};

class TrackballCamera: public BaseCamera{
public:
    float radius;
    float yaw;
    float pitch;
    float speed;
    float panningSpeed;
    Vec3 center;
    Vec3 localPosition;
    Vec3 position;
    Quaternion rotated;
    Quaternion rotatedInverse;
    TrackballCamera();
    void move(Vec2 mouse);
    void getViewMatrix(M4x4 &viewMatrix) const override;
    void pan(Vec2 mouse);
};

#endif