#ifndef QUATERNION_H
#define QUATERNION_H

#include <iostream>
#include "vector.h"
#include "matrix.h"

class Quaternion{
public:
    float w;
    float x;
    float y;
    float z;
    Vec3 getV() const;
    void setV(Vec3 v);
    std::string toString() const;
    Quaternion();
    Quaternion(float w, float x, float y, float z);
    Quaternion(float angle, Vec3 unit);
    Quaternion operator*(const Quaternion &other) const;
    Quaternion operator*(const Vec3 &p) const;
    Vec3 rotate(const Vec3 &p) const;
    float calcNorm() const;
    Quaternion getConjugate() const;
    Quaternion getInverse() const;
    Quaternion getInverse2() const;
    void getRotateMatrix(M4x4 &matrix) const;
};

#endif