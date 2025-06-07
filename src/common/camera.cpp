#include "camera.h"

FPSCamera::FPSCamera(){
    position = {0, 0, -1};
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

ArcballCamera::ArcballCamera(){
    radius = 5;
    rotated = { 1, 0, 0, 0 };
    rotatedInverse = { 1, 0, 0, 0 };
}
Vec3 mapToSemiSphere(const Vec2 &ndcMouse){
    float r = 1;
    float x = ndcMouse.x;
    float y = ndcMouse.y;
    float z = 0;
    float length = x*x + y*y;
    if(length <= r*r){
        z = std::sqrt(r*r - length);
    }
    else{
        x = x / std::sqrt(length);
        y = y / std::sqrt(length);
    }
    return {x, y, z};
}
void ArcballCamera::press(Vec2 ndcMouse){
    prevNdcMouse = ndcMouse;
}
void ArcballCamera::move(Vec2 ndcMouse){
    Vec3 va = mapToSemiSphere(prevNdcMouse);
    Vec3 vb = mapToSemiSphere(ndcMouse);
    prevNdcMouse = ndcMouse;
    Vec3 rotateAxis = crossVec(va, vb);
    float angle = lengthVec(rotateAxis);
    // float cosValue = dot(va, vb);
    // if (cosValue > 1) cosValue = 1;
    // float angle = std::acos(cosValue);
    Quaternion q = Quaternion(angle, normalizeVec(rotateAxis));
    rotated = q * rotated;
    rotatedInverse = q.getInverse() * rotatedInverse;
}
M4x4 translateMatrix;
M4x4 rotationMatrix;
void ArcballCamera::getViewMatrix(M4x4 &viewMatrix) const {
    translateMatrix.init(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, radius,
        0, 0, 0, 1
    );
    rotatedInverse.getRotateMatrix(rotationMatrix);
    avx256_multi_matrix_4x4_4x4(translateMatrix.value, rotationMatrix.value, viewMatrix.value);
}

// TrackballCamera
TrackballCamera::TrackballCamera(){
    radius = 5;
    yaw = 0;
    pitch = 0;
    speed = 0.5f;
    panningSpeed = 0.01f;
    center = {2, 0, 0};
    rotated = Quaternion(1, 0, 0, 0);
    rotatedInverse = Quaternion(1, 0, 0, 0);
    localPosition = {0, 0, -radius};
    position = center + rotated.rotate(localPosition);
}
void TrackballCamera::move(Vec2 mouse){
    yaw += mouse.x * speed;
    pitch += mouse.y * -speed;
    Quaternion q_yaw = Quaternion(yaw * M_PI / 180, {0, 1, 0});
    Quaternion q_pitch = Quaternion(pitch * M_PI / 180, {1, 0, 0});
    rotated = q_yaw * q_pitch;
    rotatedInverse = q_pitch.getInverse() * q_yaw.getInverse();
    position = center + rotated.rotate(localPosition);
}
void TrackballCamera::getViewMatrix(M4x4 &viewMatrix) const {
    translateMatrix.init(
        1, 0, 0, -position.x,
        0, 1, 0, -position.y,
        0, 0, 1, -position.z,
        0, 0, 0, 1
    );
    rotatedInverse.getRotateMatrix(rotationMatrix);
    avx256_multi_matrix_4x4_4x4(rotationMatrix.value, translateMatrix.value, viewMatrix.value);
}
M4x4 rotationMatrix2;
void TrackballCamera::pan(Vec2 mouse){
    rotated.getRotateMatrix(rotationMatrix2);
    Vec3 right = rotationMatrix2.getColumn(0);
    Vec3 up = rotationMatrix2.getColumn(1);
    center = center + right * mouse.x * -panningSpeed;
    center = center + up * mouse.y * -panningSpeed;
    position = center + rotated.rotate(localPosition);
}
