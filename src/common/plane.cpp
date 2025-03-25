#include "plane.h"
#include <string>
#include <iostream>

Plane::Plane(): D(0){}

float distancePointToPlane(const Plane &plane, const Vec3 &point){
    return dot(plane.normal, point) + plane.D;
}

ClippingPlaneStatus::ClippingPlaneStatus(){}

Vec3 getPointInLineAndThroughPlane(
    const Vec3 &pointA,
    const Vec3 &pointB,
    const Plane &plane
){
    Vec3 AB = subVec(pointB, pointA);
    float t = (-plane.D - dot(plane.normal, pointA)) / dot(plane.normal, AB);

    return addVec(pointA, scalarVec(t, AB));
}

float get_u(
    const Vec3 &start,
    const Vec3 &end,
    const Vec3 &middle
){
    if(start.x != end.x){
        return (middle.x - start.x) / (end.x - start.x);
    }
    if(start.y != end.y){
        return (middle.y - start.y) / (end.y - start.y);
    }
    if(start.z != end.z){
        return (middle.z - start.z) / (end.z - start.z);
    }
    return 0;
}

bool oneVertexFront(
    const Plane &plane,
    const float &dA,
    const float &dB,
    const float &dC,
    const Vec3 &vertexA,
    const Vec3 &vertexB,
    const Vec3 &vertexC,
    const Triangle &triangle,
    const TextureCoor &textureCoor,
    std::vector<Vec3> &verticies,
    std::vector<Triangle> &trianglesWaitingProcess,
    std::vector<TextureCoor> &textureCoorsWaitingProcess,
    std::vector<int> &trianglesWaitingProcess_plane,
    const int &i
){
    bool isOneVertexFront = false;
    Vec3 frontVertex, rearVertex1, rearVertex2;
    int frontIndex;
    Vec2 frontTextureCoor, rearTextureCoor1, rearTextureCoor2;
    if(dA >= 0 && dB < 0 && dC < 0){
        frontVertex = vertexA;
        rearVertex1 = vertexB;
        rearVertex2 = vertexC;
        frontIndex = triangle.x;
        frontTextureCoor = textureCoor.uv1;
        rearTextureCoor1 = textureCoor.uv2;
        rearTextureCoor2 = textureCoor.uv3;
        isOneVertexFront = true;
    }
    if(dB >= 0 && dA < 0 && dC < 0){
        frontVertex = vertexB;
        rearVertex1 = vertexA;
        rearVertex2 = vertexC;
        frontIndex = triangle.y;
        frontTextureCoor = textureCoor.uv2;
        rearTextureCoor1 = textureCoor.uv1;
        rearTextureCoor2 = textureCoor.uv3;
        isOneVertexFront = true;
    }
    if(dC >= 0 && dA < 0 && dB < 0){
        frontVertex = vertexC;
        rearVertex1 = vertexA;
        rearVertex2 = vertexB;
        frontIndex = triangle.z;
        frontTextureCoor = textureCoor.uv3;
        rearTextureCoor1 = textureCoor.uv1;
        rearTextureCoor2 = textureCoor.uv2;
        isOneVertexFront = true;
    }
    if(!isOneVertexFront){
        return false;
    }

    Vec3 intersectVertex1 = getPointInLineAndThroughPlane(frontVertex, rearVertex1, plane);
    Vec3 intersectVertex2 = getPointInLineAndThroughPlane(frontVertex, rearVertex2, plane);
    verticies.push_back(intersectVertex1);
    int num_newVertex1 = verticies.size() - 1;
    verticies.push_back(intersectVertex2);
    int num_newVertex2 = verticies.size() - 1;
    Triangle newTriangle = {
        frontIndex,
        num_newVertex1,
        num_newVertex2
    };
    trianglesWaitingProcess.push_back(newTriangle);
    trianglesWaitingProcess_plane.push_back(i + 1);

    float u1 = get_u(frontVertex, rearVertex1, intersectVertex1);
    float u2 = get_u(frontVertex, rearVertex2, intersectVertex2);
    TextureCoor newTextureCoor = {
        frontTextureCoor,
        addVec(frontTextureCoor, scalarVec(u1, subVec(rearTextureCoor1, frontTextureCoor))),
        addVec(frontTextureCoor, scalarVec(u2, subVec(rearTextureCoor2, frontTextureCoor)))
    };
    textureCoorsWaitingProcess.push_back(newTextureCoor);

    return true;
}

bool twoVertexFront(
    const Plane &plane,
    const float &dA,
    const float &dB,
    const float &dC,
    const Vec3 &vertexA,
    const Vec3 &vertexB,
    const Vec3 &vertexC,
    const Triangle &triangle,
    const TextureCoor &textureCoor,
    std::vector<Vec3> &verticies,
    std::vector<Triangle> &trianglesWaitingProcess,
    std::vector<TextureCoor> &textureCoorsWaitingProcess,
    std::vector<int> &trianglesWaitingProcess_plane,
    const int &i
){
    bool isTwoVertexFront = false;
    Vec3 frontVertex1, frontVertex2, rearVertex;
    int frontIndex1, frontIndex2;
    Vec2 frontTextureCoor1, frontTextureCoor2, rearTextureCoor;
    if(dA >= 0 && dB >= 0 && dC < 0){
        frontVertex1 = vertexA;
        frontVertex2 = vertexB;
        rearVertex = vertexC;
        frontIndex1 = triangle.x;
        frontIndex2 = triangle.y;
        frontTextureCoor1 = textureCoor.uv1;
        frontTextureCoor2 = textureCoor.uv2;
        rearTextureCoor = textureCoor.uv3;
        isTwoVertexFront = true;
    }
    if(dA >= 0 && dC >= 0 && dB < 0){
        frontVertex1 = vertexA;
        frontVertex2 = vertexC;
        rearVertex = vertexB;
        frontIndex1 = triangle.x;
        frontIndex2 = triangle.z;
        frontTextureCoor1 = textureCoor.uv1;
        frontTextureCoor2 = textureCoor.uv3;
        rearTextureCoor = textureCoor.uv2;
        isTwoVertexFront = true;
    }
    if(dB >= 0 && dC >= 0 && dA < 0){
        frontVertex1 = vertexB;
        frontVertex2 = vertexC;
        rearVertex = vertexA;
        frontIndex1 = triangle.y;
        frontIndex2 = triangle.z;
        frontTextureCoor1 = textureCoor.uv2;
        frontTextureCoor2 = textureCoor.uv3;
        rearTextureCoor = textureCoor.uv1;
        isTwoVertexFront = true;
    }
    if(!isTwoVertexFront){
        return false;
    }

    Vec3 intersectVertex1 = getPointInLineAndThroughPlane(frontVertex1, rearVertex, plane);
    Vec3 intersectVertex2 = getPointInLineAndThroughPlane(frontVertex2, rearVertex, plane);
    verticies.push_back(intersectVertex1);
    int num_newVertex1 = verticies.size() - 1;
    verticies.push_back(intersectVertex2);
    int num_newVertex2 = verticies.size() - 1;
    Triangle newTriangle1 = {
        frontIndex1,
        frontIndex2,
        num_newVertex1
    };
    Triangle newTriangle2 = {
        frontIndex2,
        num_newVertex1,
        num_newVertex2
    };
    trianglesWaitingProcess.push_back(newTriangle1);
    trianglesWaitingProcess_plane.push_back(i + 1);
    trianglesWaitingProcess.push_back(newTriangle2);
    trianglesWaitingProcess_plane.push_back(i + 1);

    float u1 = get_u(frontVertex1, rearVertex, intersectVertex1);
    float u2 = get_u(frontVertex2, rearVertex, intersectVertex2);
    TextureCoor newTextureCoor1 = {
        frontTextureCoor1,
        frontTextureCoor2,
        addVec(frontTextureCoor1, scalarVec(u1, subVec(rearTextureCoor, frontTextureCoor1)))
    };
    TextureCoor newTextureCoor2 = {
        frontTextureCoor2,
        addVec(frontTextureCoor1, scalarVec(u1, subVec(rearTextureCoor, frontTextureCoor1))),
        addVec(frontTextureCoor2, scalarVec(u2, subVec(rearTextureCoor, frontTextureCoor2)))
    };
    textureCoorsWaitingProcess.push_back(newTextureCoor1);
    textureCoorsWaitingProcess.push_back(newTextureCoor2);

    return true;
}