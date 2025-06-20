#include "vector.h"

Vec2::Vec2(): x(0), y(0){}
Vec2::Vec2(float x, float y): x(x), y(y){}
Vec2 Vec2::operator+(const Vec2 &other) const{
    return {
        x + other.x,
        y + other.y
    };
}
Vec2 Vec2::operator-(const Vec2 &other) const{
    return {
        x - other.x,
        y - other.y
    };
}
Vec2 Vec2::operator*(const float &number) const{
    return {
        x * number,
        y * number
    };
}

Vec3::Vec3(): z(0){}
Vec3::Vec3(float x, float y, float z): Vec2(x, y), z(z){}
Vec3 Vec3::operator+(const Vec3 &other) const{
    return {
        x + other.x,
        y + other.y,
        z + other.z
    };
}
Vec3 Vec3::operator-(const Vec3 &other) const{
    return {
        x - other.x,
        y - other.y,
        z - other.z
    };
}
Vec3 Vec3::operator*(const float &number) const{
    return {
        x * number,
        y * number,
        z * number
    };
}

Vec4::Vec4(): w(0){}
Vec4::Vec4(float x, float y, float z, float w): Vec3(x, y, z), w(w){};

Vec2 addVec(const Vec2 &vec1, const Vec2 &vec2){
    return {
        vec1.x + vec2.x,
        vec1.y + vec2.y
    };
}
Vec3 addVec(const Vec3 &vec1, const Vec3 &vec2){
    return {
        vec1.x + vec2.x,
        vec1.y + vec2.y,
        vec1.z + vec2.z
    };
}
Vec3 subVec(const Vec3 &vec1, const Vec3 &vec2){
    return {
        vec1.x - vec2.x,
        vec1.y - vec2.y,
        vec1.z - vec2.z
    };
}
Vec2 subVec(const Vec2 &vec1, const Vec2 &vec2){
    return {
        vec1.x - vec2.x,
        vec1.y - vec2.y
    };
}
Vec2Int subVec(const Vec2Int &vec1, const Vec2Int &vec2){
    return {
        vec1.x - vec2.x,
        vec1.y - vec2.y
    };
}
Vec3 scalarVec(const float &scalar, const Vec3 &vec){
    return {
        vec.x * scalar,
        vec.y * scalar,
        vec.z * scalar
    };
}
Vec2 scalarVec(const float &scalar, const Vec2 &vec){
    return {
        vec.x * scalar,
        vec.y * scalar
    };
}
Vec2 divineVec(const Vec2 &vec, const float &num){
    return {
        vec.x / num,
        vec.y / num
    };
}
float scalarCrossVec(const Vec2 &vec1, const Vec2 &vec2){
    return (vec1.x * vec2.y) - (vec1.y * vec2.x);
}
int scalarCrossVec(const Vec2Int &vec1, const Vec2Int &vec2){
    return (vec1.x * vec2.y) - (vec1.y * vec2.x);
}
Vec3 crossVec(const Vec3 &vec1, const Vec3 &vec2){
    return {
        vec1.y * vec2.z - vec1.z * vec2.y,
        vec1.z * vec2.x - vec1.x * vec2.z,
        vec1.x * vec2.y - vec1.y * vec2.x,
    };
}
float lengthVec(const Vec3 &vec){
    return std::sqrt(
        vec.x * vec.x +
        vec.y * vec.y +
        vec.z * vec.z
    );
}
float lengthVec(const Vec2 &vec){
    return std::sqrt(
        vec.x * vec.x +
        vec.y * vec.y
    );
}
Vec3 normalizeVec(const Vec3 &vec){
    float length = lengthVec(vec);
    if(length == 0) length = 1;
    return {
        vec.x / length,
        vec.y / length,
        vec.z / length,
    };
}
Vec2 normalizeVec(const Vec2 &vec){
    float length = lengthVec(vec);
    if(length == 0){
        return {
            vec.x,
            vec.y
        };
    }
    return {
        vec.x / length,
        vec.y / length
    };
}
float dot(const Vec3 &vec1, const Vec3 &vec2){
    return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}
Vec3 homogeneous3DToCartesian(const Vec3 &vec3){
    assert(vec3.z != 0 && "homogeneous3DToCartesian err, clipping can be wrong");
    return {
        vec3.x / vec3.z,
        vec3.y / vec3.z,
        vec3.z
    };
}
void swapVec(Vec3 &vec1, Vec3 &vec2){
    Vec3 temp = vec1;
    vec1 = vec2;
    vec2 = temp;
}
void swapVec(Vec2 &vec1, Vec2 &vec2){
    Vec2 temp = vec1;
    vec1 = vec2;
    vec2 = temp;
}
void swapVec(Vec2Int &vec1, Vec2Int &vec2){
    Vec2Int temp = vec1;
    vec1 = vec2;
    vec2 = temp;
}
std::string vecToString(const float &x, const float &y, const float &z){
    std::string s;
    s.append("(");
    s.append(std::to_string(x));
    s.append("f, ");
    s.append(std::to_string(y));
    s.append("f, ");
    s.append(std::to_string(z));
    s.append("f)");
    
    return s;
}
std::string vecToString(const Vec3 &vec){
    return vecToString(vec.x, vec.y, vec.z);
}