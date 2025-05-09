#include "camera.h"

FPSCamera::FPSCamera(){
    position = {0, 0, -10};
    forward = {0, 0, 1};
    right = {1, 0, 0};
    up = {0, 1, 0};
    speed = 0.1f;
}

void FPSCamera::update(const Vec2 &mouseDir){
    yaw += speed * mouseDir.x;
    pitch += speed * mouseDir.y;
    if(pitch > 89){
        pitch = 89;
    }
    if(pitch < -89){
        pitch = -89;
    }
    float yawRad = yaw * M_PI / 180;
    float pitchRad = pitch * M_PI / 180;
    forward.x = std::sin(yawRad) * std::cos(pitchRad);
    forward.y = std::sin(pitchRad);
    forward.z = std::cos(yawRad) * std::cos(pitchRad);
    right = normalizeVec(crossVec({0, 1, 0}, forward));
    up = normalizeVec(crossVec(forward, right));
}

void FPSCamera::getViewMatrix(M4x4 &viewMatrix) const {
    viewMatrix.init(
        right.x, right.y, right.z, -dot(right, position),
        up.x, up.y, up.z, -dot(up, position),
        forward.x, forward.y, forward.z, -dot(forward, position),
        0, 0, 0, 1
    );
}