#ifndef VECTOR_H
#define VECTOR_H

#include "vectorint.h"
#include <string>
#include <cmath>

class Vec2{
public:
    float x;
    float y;
    Vec2();
    Vec2(float x, float y);
};

class Vec3: public Vec2{
public:
    float z;
    Vec3();
    Vec3(float x, float y, float z);
};

class Vec4: public Vec3{
public:
    float w;
    Vec4();
    Vec4(float x, float y, float z, float w);
};

Vec2 addVec(const Vec2 &vec1, const Vec2 &vec2);
Vec3 addVec(const Vec3 &vec1, const Vec3 &vec2);
Vec3 subVec(const Vec3 &vec1, const Vec3 &vec2);
Vec2 subVec(const Vec2 &vec1, const Vec2 &vec2);
Vec2Int subVec(const Vec2Int &vec1, const Vec2Int &vec2);
Vec3 scalarVec(const float &scalar, const Vec3 &vec);
Vec2 scalarVec(const float &scalar, const Vec2 &vec);
Vec2 divineVec(const Vec2 &vec, const float &num);
float scalarCrossVec(const Vec2 &vec1, const Vec2 &vec2);
int scalarCrossVec(const Vec2Int &vec1, const Vec2Int &vec2);
float lengthVec(const Vec3 &vec);
float dot(const Vec3 &vec1, const Vec3 &vec2);
Vec3 homogeneous3DToCartesian(const Vec3 &vec3);
void swapVec(Vec3 &vec1, Vec3 &vec2);
void swapVec(Vec2 &vec1, Vec2 &vec2);
std::string vecToString(const float &x, const float &y, const float &z);
std::string vecToString(const Vec3 &vec);

#endif