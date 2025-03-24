#include "global.h"
#include "common/matrix.h"
#include <cmath>
#include "common/plane.h"
#include "common/sphere.h"
#include "common/fixednumber.h"
#include <math.h>
#include <string>
#include <iostream>
#include <immintrin.h>
#include "asset.h"
#include <algorithm>

Vec2 viewportToCanvasCoordinate(const Vec3 &vec3){
    return {
        vec3.x + canvas_half_cW,
        -vec3.y + canvas_half_cH
    };
}

void putPixel(const int &x, const int &y){
    Vec4 color = {0, 0, 0, 255};
    int offset = (x * 4) + (y * canvas_four_mul_cW);
    canvasBuffer[offset + 0] = color.x;
    canvasBuffer[offset + 1] = color.y;
    canvasBuffer[offset + 2] = color.z;
    canvasBuffer[offset + 3] = color.w;
}

void putPixel(const int &x, const int &y, const Vec4 &color, const float &dept){
    int offset;
    offset = x  + (y * canvas_cW);
    if(dept <= deptBuffer[offset]){
        return;
    }
    deptBuffer[offset] = dept;

    offset = (x * 4) + (y * canvas_four_mul_cW);
    canvasBuffer[offset + 0] = color.x;
    canvasBuffer[offset + 1] = color.y;
    canvasBuffer[offset + 2] = color.z;
    canvasBuffer[offset + 3] = color.w;
}

Vec4 getTexel(
    const Vec2 &uv1, const Vec2 &uv12, const Vec2 &uv13,
    const float &_1_over_z,
    const float &u, const float &v
){
    Vec2 uv =
    divineVec(
        addVec(
            uv1,
            addVec(
                scalarVec(u, uv12),
                scalarVec(v, uv13)
            )
        ),
        _1_over_z
    );
    

    // std::cout << "(" << textureCoor.uv3.x << ", " << textureCoor.uv3.y << " | " << uv.x << ", " << uv.y << ") ";
    uv.x = std::clamp(uv.x, 0.0f, 1.0f);
    uv.y = std::clamp(uv.y, 0.0f, 1.0f);
    int uv_w = std::floor(crateTexture_width * uv.x);
    int uv_h = std::floor(crateTexture_height * (1 - uv.y));
    int offset = uv_w * 3 + uv_h * crateTexture_pitch;
    // std::cout << uv.x << ", " << uv.y << " | " << offset << " ";
    Vec4 texel = {
        static_cast<float>(crateTexture_pixels[offset]),
        static_cast<float>(crateTexture_pixels[offset + 1]),
        static_cast<float>(crateTexture_pixels[offset + 2]),
        255
    };

    return texel;
}

float interpolate_1_over_z(
    const Vec3 &p1_o, const Vec3 &p2_o, const Vec3 &p3_o,
    const float &u, const float &v
){
    return (1/p1_o.z) + u * (1/p2_o.z - 1/p1_o.z) + v * (1/p3_o.z - 1/p1_o.z);
}

void drawFilledTriangle(Vec3 p1_o, Vec3 p2_o, Vec3 p3_o, const TextureCoor &textureCoor){
    Vec2 p1 = viewportToCanvasCoordinate(p1_o);
    Vec2 p2 = viewportToCanvasCoordinate(p2_o);
    Vec2 p3 = viewportToCanvasCoordinate(p3_o);

    Vec2 p12 = subVec(p2, p1);
    Vec2 p13 = subVec(p3, p1);
    float signedArea = scalarCrossVec(p12, p13);
    if(signedArea > 0){
        swapVec(p2, p3);
    }

    Vec2Int p1F = fixedNumber_fixedXY(p1);
    Vec2Int p2F = fixedNumber_fixedXY(p2);
    Vec2Int p3F = fixedNumber_fixedXY(p3);
    int xMin = std::min(p1F.x, std::min(p2F.x, p3F.x)) >> fixedNumber_RESOLUTION;
    int yMin = std::min(p1F.y, std::min(p2F.y, p3F.y)) >> fixedNumber_RESOLUTION;
    int xMax = (std::max(p1F.x, std::max(p2F.x, p3F.x)) + 15) >> fixedNumber_RESOLUTION;
    int yMax = (std::max(p1F.y, std::max(p2F.y, p3F.y)) + 15) >> fixedNumber_RESOLUTION;
    int dx12 = p2F.x - p1F.x;
    int dx23 = p3F.x - p2F.x;
    int dx31 = p1F.x - p3F.x;
    int dy12 = p2F.y - p1F.y;
    int dy23 = p3F.y - p2F.y;
    int dy31 = p1F.y - p3F.y;
    int dx12F = dx12 << fixedNumber_RESOLUTION;
    int dx23F = dx23 << fixedNumber_RESOLUTION;
    int dx31F = dx31 << fixedNumber_RESOLUTION;
    int dy12F = dy12 << fixedNumber_RESOLUTION;
    int dy23F = dy23 << fixedNumber_RESOLUTION;
    int dy31F = dy31 << fixedNumber_RESOLUTION;
    int C1 = dy12 * p1F.x - dx12 * p1F.y;
    int C2 = dy23 * p2F.x - dx23 * p2F.y;
    int C3 = dy31 * p3F.x - dx31 * p3F.y;
    if(dy12 > 0 || (dy12 == 0 && dx12 < 0)){
        C1--;
    }
    if(dy23 > 0 || (dy23 == 0 && dx23 < 0)){
        C2--;
    }
    if(dy31 > 0 || (dy31 == 0 && dx31 < 0)){
        C3--;
    }

    int q = 4;
    xMin = xMin & ~(q - 1);
    yMin = yMin & ~(q - 1);

    Vec2Int n12 = {-dy12, dx12};
    Vec2Int n23 = {-dy23, dx23};
    Vec2Int n31 = {-dy31, dx31};
    Vec2Int t12 = { n12.x <= 0 ? q - 1 : 0, n12.y <= 0 ? q - 1 : 0 };
    Vec2Int t23 = { n23.x <= 0 ? q - 1 : 0, n23.y <= 0 ? q - 1 : 0 };
    Vec2Int t31 = { n31.x <= 0 ? q - 1 : 0, n31.y <= 0 ? q - 1 : 0 };

    int area = scalarCrossVec(
        subVec(p2F, p1F),
        subVec(p3F, p1F)
    );
    Vec2 uv1 = {textureCoor.uv1.x, textureCoor.uv1.y};
    Vec2 uv2 = {textureCoor.uv2.x, textureCoor.uv2.y};
    Vec2 uv3 = {textureCoor.uv3.x, textureCoor.uv3.y};
    if(signedArea > 0){
        swapVec(uv2, uv3);
        swapVec(p2_o, p3_o);
    }
    uv1 = divineVec(uv1, p1_o.z);
    uv2 = divineVec(uv2, p2_o.z);
    uv3 = divineVec(uv3, p3_o.z);
    Vec2 uv12 = subVec(uv2, uv1);
    Vec2 uv13 = subVec(uv3, uv1);

    for(int y = yMin; y < yMax; y += q){
        for(int x = xMin; x < xMax; x += q){
            int x_t12 = ((x + t12.x) << fixedNumber_RESOLUTION) + 8;
            int y_t12 = ((y + t12.y) << fixedNumber_RESOLUTION) + 8;

            int x_t23 = ((x + t23.x) << fixedNumber_RESOLUTION) + 8;
            int y_t23 = ((y + t23.y) << fixedNumber_RESOLUTION) + 8;
            
            int x_t31 = ((x + t31.x) << fixedNumber_RESOLUTION) + 8;
            int y_t31 = ((y + t31.y) << fixedNumber_RESOLUTION) + 8;

            bool _00_12 = C1 + dx12 * y_t12 - dy12 * x_t12 < 0;
            bool _00_23 = C2 + dx23 * y_t23 - dy23 * x_t23 < 0;
            bool _00_31 = C3 + dx31 * y_t31 - dy31 * x_t31 < 0;

            if(!_00_12 || !_00_23 || !_00_31){
                // ignore block
                continue;
            }
            else{
                // partially block
                int x0 = (x << fixedNumber_RESOLUTION) + 8;
                int y0 = (y << fixedNumber_RESOLUTION) + 8;
                int cy12 = C1 + dx12 * y0 - dy12 * x0;
                int cy23 = C2 + dx23 * y0 - dy23 * x0;
                int cy31 = C3 + dx31 * y0 - dy31 * x0;
                for(int yi = y; yi < y + q; yi++){
                    int cx12 = cy12;
                    int cx23 = cy23;
                    int cx31 = cy31;
                    for(int xi = x; xi < x + q; xi++){
                        if((cx12 & cx23 & cx31) < 0){
                            //
                            float u = static_cast<float>(cx31) / area;
                            float v = static_cast<float>(cx12) / area;
                            float _1_over_z = interpolate_1_over_z(p1_o, p2_o, p3_o, u, v);
                            // std::cout << u << ", " << v << std::endl;
                            Vec4 color = getTexel(uv1, uv12, uv13, _1_over_z, u, v);

                            //
                            // putPixel(xi, yi);
                            putPixel(xi, yi, color, _1_over_z);
                        }
                        cx12 -= dy12F;
                        cx23 -= dy23F;
                        cx31 -= dy31F;
                    }
                    cy12 += dx12F;
                    cy23 += dx23F;
                    cy31 += dx31F;
                }
            }
        }
    }
}

void renderTriangle(
    const Triangle &triangle,
    const TextureCoor &textureCoor,
    const std::vector<Vec3> &projecteds
){
    drawFilledTriangle(
        projecteds[triangle.x],
        projecteds[triangle.y],
        projecteds[triangle.z],
        textureCoor
    );
}

M4x4 m_scale;
M4x4 m_rotation;
M4x4 m_translation;
M4x4 m_modeltransform;
void makeModelTransform(const Transform &transform){
    m_scale.init(
        transform.scale, 0, 0, 0,
        0, transform.scale, 0, 0,
        0, 0, transform.scale, 0,
        0, 0, 0, 1
    );
    float rotationRad = transform.rotation * M_PI / 180;
    m_rotation.init(
        std::cos(rotationRad), 0, std::sin(rotationRad), 0,
        0, 1, 0, 0,
        -(std::sin(rotationRad)), 0, std::cos(rotationRad), 0,
        0, 0, 0, 1
    );
    m_translation.init(
        1, 0, 0, transform.translation.x,
        0, 1, 0, transform.translation.y,
        0, 0, 1, transform.translation.z,
        0, 0, 0, 1
    );

    avx256_multi_matrix_4x4_4x4(m_translation.value, m_rotation.value, m_modeltransform.value);
    avx256_multi_matrix_4x4_4x4(m_modeltransform.value, m_scale.value, m_modeltransform.value);
}

M4x4 m_camera;
void makeCameraTransform(){
    m_translation.init(
        1, 0, 0, -camera.transform.translation.x,
        0, 1, 0, -camera.transform.translation.y,
        0, 0, 1, -camera.transform.translation.z,
        0, 0, 0, 1
    );
    float rotationRad = -camera.transform.rotation * M_PI / 180;
    m_rotation.init(
        std::cos(rotationRad), 0, std::sin(rotationRad), 0,
        0, 1, 0, 0,
        -(std::sin(rotationRad)), 0, std::cos(rotationRad), 0,
        0, 0, 0, 1
    );

    avx256_multi_matrix_4x4_4x4(m_rotation.value, m_translation.value, m_camera.value);
}

unsigned int clippingPlaneLength = 5;
Plane *clippingPlanes = new Plane[clippingPlaneLength];
bool clippingPlanesInited = false;

void initClippingPlanes(){
    if(!clippingPlanesInited){
        // nearPlane
        clippingPlanes[0].normal = {0, 0, 1};
        clippingPlanes[0].D = -camera.distanceToViewport;
        //leftPlane
        clippingPlanes[1].normal = {1 / std::sqrt((float)2), 0, 1 / std::sqrt((float)2)};
        clippingPlanes[1].D = 0;
        //rightPlane
        clippingPlanes[2].normal = {-1 / std::sqrt((float)2), 0, 1 / std::sqrt((float)2)};
        clippingPlanes[2].D = 0;
        //bottomPlane
        clippingPlanes[3].normal = {0, 1 / std::sqrt((float)2), 1 / std::sqrt((float)2)};
        clippingPlanes[3].D = 0;
        //topPlane
        clippingPlanes[4].normal = {0, -1 / std::sqrt((float)2), 1 / std::sqrt((float)2)};
        clippingPlanes[4].D = 0;

        clippingPlanesInited = true;
    }
}

Sphere getBoudingSphere(const std::vector<Vec3> &verticies){
    Sphere result;

    for(int i = 0, n = verticies.size(); i < n; i++){
        Vec3 vertex = verticies[i];
        result.center.x += vertex.x;
        result.center.y += vertex.y;
        result.center.z += vertex.z;
    }
    result.center.x /= verticies.size();
    result.center.y /= verticies.size();
    result.center.z /= verticies.size();

    for(int i = 0, n = verticies.size(); i < n; i++){
        Vec3 vertex = verticies[i];
        float distanceVertexAndCenter = lengthVec(subVec(vertex, result.center));
        if(result.radius < distanceVertexAndCenter){
            result.radius = distanceVertexAndCenter;
        }
    }

    return result;
}

ClippingPlaneStatus clipWholeObject(const Sphere &boundingSphere){
    ClippingPlaneStatus status;

    for(int i = 0; i < clippingPlaneLength; i++){
        Plane plane = clippingPlanes[i];
        float d = distancePointToPlane(plane, boundingSphere.center);
        if(d < -(boundingSphere.radius)){
            status.rears.push_back(plane);
            break;
        }
        if(d > boundingSphere.radius){
            status.fronts.push_back(plane);
        }
        if(std::abs(d) < boundingSphere.radius){
            status.intersects.push_back(plane);
        }
    }

    return status;
}

std::vector<Triangle> clipTriangle(
    const std::vector<Plane> &intersectPlanes,
    std::vector<Vec3> &verticies,
    const std::vector<Triangle> &triangles
){
    std::vector<Triangle> trianglesWaitingProcess = triangles;
    std::vector<int> trianglesWaitingProcess_plane(trianglesWaitingProcess.size(), 0);
    std::vector<Triangle> trianglesProcessed;
    trianglesProcessed.reserve(trianglesWaitingProcess.size());
    
    while(!trianglesWaitingProcess.empty()){
        Triangle triangle = trianglesWaitingProcess.back();
        trianglesWaitingProcess.pop_back();
        int startPlane = trianglesWaitingProcess_plane.back();
        trianglesWaitingProcess_plane.pop_back();
        int front = 0;
        for(int i = startPlane, n = intersectPlanes.size(); i < n; i++){
            Plane plane = intersectPlanes[i];
            Vec3 vertexA = verticies[triangle.x];
            Vec3 vertexB = verticies[triangle.y];
            Vec3 vertexC = verticies[triangle.z];
            float dA = distancePointToPlane(plane, vertexA);
            float dB = distancePointToPlane(plane, vertexB);
            float dC = distancePointToPlane(plane, vertexC);

            if(dA < 0 && dB < 0 && dC < 0){
                break;
            }
            else if(dA >= 0 && dB >= 0 && dC >= 0){
                front++;
                continue;
            }
            else{
                if(oneVertexFront(plane, dA, dB, dC, vertexA, vertexB, vertexC, triangle, verticies, trianglesWaitingProcess, trianglesWaitingProcess_plane, i)){
                    break;
                }
                else if(twoVertexFront(plane, dA, dB, dC, vertexA, vertexB, vertexC, triangle, verticies, trianglesWaitingProcess, trianglesWaitingProcess_plane, i)){
                    break;
                }
            }
        }
        if(front == intersectPlanes.size() - startPlane){
            trianglesProcessed.push_back(triangle);
        }
    }

    return trianglesProcessed;
}

std::vector<Triangle> clipping(std::vector<Vec3> &applieds, const std::vector<Triangle> &triangles){
    initClippingPlanes();
    ClippingPlaneStatus status = clipWholeObject(getBoudingSphere(applieds));
    if(status.rears.size() > 0){
        return {};
    }
    if(status.fronts.size() == clippingPlaneLength){
        return triangles;
    }

    return clipTriangle(status.intersects, applieds, triangles);
}

M4x4 m_cameramodel;
M4x1 m4x1_cache_0;
M4x1 m4x1_cache_1;
std::vector<Vec3> apply(const Instance &instance){
    makeModelTransform(instance.transform);
    makeCameraTransform();
    avx256_multi_matrix_4x4_4x4(
        m_camera.value,
        m_modeltransform.value,
        m_cameramodel.value
    );

    std::vector<Vec3> applieds;
    applieds.reserve(instance.model->vertices.size());

    for(int i = 0, n = instance.model->vertices.size(); i < n; i++){
        vec3_to_M4x1(instance.model->vertices[i], m4x1_cache_0);
        avx256_multi_matrix_4x4_4x1(
            m_cameramodel.value,
            m4x1_cache_0.value,
            m4x1_cache_1.value
        );
        Vec3 applied = matrix_to_vec3(m4x1_cache_1);
        applieds.push_back(applied);
    }

    return applieds;
}

M3x4 m_projection;
void makeProjectionTransform(){
    m_projection.init(
        camera.distanceToViewport * canvas_cW / viewport_vW, 0, 0, 0,
        0, camera.distanceToViewport * canvas_cH / viewport_vH, 0, 0,
        0, 0, 1, 0
    );
}

M3x1 m3x1_cache_0;
void project(std::vector<Vec3> &applieds){
    makeProjectionTransform();

    for(int i = 0, n = applieds.size(); i < n; i++){
        Vec3 applied = applieds[i];
        vec3_to_M4x1(applied, m4x1_cache_0);
        avx256_multi_matrix_3x4_4x1(
            m_projection.value,
            m4x1_cache_0.value,
            m3x1_cache_0.value
        );
        Vec3 projected = homogeneous3DToCartesian(
            matrix_to_vec3(m3x1_cache_0)
        );
        applieds[i] = projected;
    }
}

void render_instance(const Instance &instance, int idx){
    std::vector<Vec3> applieds = apply(instance);

    std::vector<Triangle> clippingTriangles = clipping(applieds, instance.model->triangles);
    if(clippingTriangles.empty()) return;

    project(applieds);

    std::fill_n(canvasBuffer, canvasBufferLength, 255);
    std::fill_n(deptBuffer, deptBufferLength, -std::numeric_limits<float>::max());
    for(int i = 0, n = clippingTriangles.size(); i < n; i++){
        Triangle triangle = clippingTriangles[i];
        TextureCoor textureCoor = instance.model->textureCoors[i];
        // if(i == 0){
            renderTriangle(triangle, textureCoor, applieds);
        // }
    }
}
