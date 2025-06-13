#include "global.h"
#include <SDL3/SDL.h>

Triangle::Triangle():x(0), y(0), z(0){}
Triangle::Triangle(int x, int y, int z): x(x), y(y), z(z){}
Triangle::Triangle(int x, int y, int z, Vec4 color): x(x), y(y), z(z){}

Model::Model(){}
Model::Model(int num_vertices, int num_triangles){
    vertices.reserve(num_vertices);
    triangles.reserve(num_triangles);
    textureCoors.reserve(num_triangles);
}

TextureCoor::TextureCoor(){}
TextureCoor::TextureCoor(Vec2 uv1, Vec2 uv2, Vec2 uv3): uv1(uv1), uv2(uv2), uv3(uv3){}

Transform::Transform(){}
Transform::Transform(Vec3 translation, Vec3 rotation, float scale): translation(translation), rotation(rotation), scale(scale){}

Instance::Instance(){}
Instance::Instance(Model *model): model(model){}

Camera::Camera(){}
Camera::Camera(float distanceToViewport, Transform transform): distanceToViewport(distanceToViewport), transform(transform){}
Camera camera = {1, Transform(Vec3(0, 0, -5), Vec3(0, 0, 0), 0)};

unsigned int canvas_cW = 0;
unsigned int canvas_cH = 0;
float canvas_half_cW = 0;
float canvas_half_cH = 0;
unsigned int canvas_four_mul_cW;
Uint8 *canvasBuffer = nullptr;
size_t canvasBufferLength = 0;
float viewport_vW = 0;
float viewport_vH = 0;

float *deptBuffer;
size_t deptBufferLength;

void init_canvas_buffer(unsigned int cW, unsigned int cH, float vW, float vH){
    canvas_cW = cW;
    canvas_cH = cH;
    canvasBufferLength = cW * cH * 4;
    canvasBuffer = new Uint8[canvasBufferLength];
    viewport_vW = vW;
    viewport_vH = vH;
    canvas_half_cW = cW / (float)2;
    canvas_half_cH = cH / (float)2;
    canvas_four_mul_cW = cW * 4;

    deptBufferLength = cW * cH;
    deptBuffer = new float[deptBufferLength];
}