#include "camera.h"

FPSCamera::FPSCamera(){
    position = {0, 0, -10};
    forward = {0, 0, 1};
    right = {1, 0, 0};
    up = {0, 1, 0};
    speed = 360.0/8192.0;
}

// void FPSCamera::update(const Vec2 &mouseDir, const float &deltaTime){
//     yaw += speed * mouseDir.x;
//     pitch += speed * mouseDir.y;
//     if(pitch > 89){
//         pitch = 89;
//     }
//     if(pitch < -89){
//         pitch = -89;
//     }
//     float yawRad = yaw * M_PI / 180;
//     float pitchRad = pitch * M_PI / 180;
//     forward.x = std::sin(yawRad) * std::cos(pitchRad);
//     forward.y = std::sin(pitchRad);
//     forward.z = std::cos(yawRad) * std::cos(pitchRad);
//     right = normalizeVec(crossVec({0, 1, 0}, forward));
//     up = normalizeVec(crossVec(forward, right));
// }

void FPSCamera::update(const Vec2 &mouseDir, const float &deltaTime){
    yaw   += speed * mouseDir.x;
    pitch += speed * mouseDir.y;
    if(pitch > 90.0){
        pitch = 90.0;
    }
    if(pitch < -90.0){
        pitch = -90.0;
    }
    float yawRad   = yaw   * M_PI / 180.0;
    float pitchRad = pitch * M_PI / 180.0;

    forward.x =  std::cos(pitchRad) * std::sin(yawRad);
    forward.y =  std::sin(pitchRad);
    forward.z =  std::cos(pitchRad) * std::cos(yawRad);

    right.x   =  std::cos(yawRad);
    right.y   =  0;
    right.z   = -std::sin(yawRad);

    up.x      = -std::sin(pitchRad) * std::sin(yawRad);
    up.y      =  std::cos(pitchRad);
    up.z      = -std::sin(pitchRad) * std::cos(yawRad);
}

void FPSCamera::getViewMatrix(M4x4 &viewMatrix) const {
    viewMatrix.init(
        right.x, right.y, right.z, -dot(right, position),
        up.x, up.y, up.z, -dot(up, position),
        forward.x, forward.y, forward.z, -dot(forward, position),
        0, 0, 0, 1
    );
}