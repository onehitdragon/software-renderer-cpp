#ifndef GLOBAL_H
#define GLOBAL_H

#include "common/vector.h"
#include <vector>

class Triangle{
public:
    int x;
    int y;
    int z;
    Triangle();
    Triangle(int x, int y, int z);
    Triangle(int x, int y, int z, Vec4 color); // will be removed
};

class TextureCoor{
public:
    Vec2 uv1;
    Vec2 uv2;
    Vec2 uv3;
    TextureCoor();
    TextureCoor(Vec2 uv1, Vec2 uv2, Vec2 uv3);
};

class Model{
public:
    std::vector<Vec3> vertices;
    std::vector<Triangle> triangles;
    std::vector<Vec2> uv;
    std::vector<TextureCoor> textureCoors;
    Model();
    Model(int num_vertices, int num_triangles);
};

class Transform{
public:
    Vec3 translation;
    Vec3 rotation;
    float scale;
    Transform();
    Transform(Vec3 translation, Vec3 rotation, float scale);
};

class Instance{
public:
    Model *model;
    Transform transform;
    Instance();
    Instance(Model *model);
};

class Camera{
public:
    float distanceToViewport;
    Transform transform;
    Camera();
    Camera(float distanceToViewport, Transform transform);
};
extern Camera camera;

extern unsigned int canvas_cW;
extern unsigned int canvas_cH;
extern float canvas_half_cW;
extern float canvas_half_cH;
extern unsigned int canvas_four_mul_cW;
extern unsigned char *canvasBuffer;
extern size_t canvasBufferLength;
extern float viewport_vW;
extern float viewport_vH;
extern float *deptBuffer;
extern size_t deptBufferLength;
void init_canvas_buffer(unsigned int cW, unsigned int cH, float vW, float vH);

#endif