#include "quaternion.h"

Vec3 Quaternion::getV() const{
    return {x, y, z};
}
void Quaternion::setV(Vec3 v){
    x = v.x;
    y = v.y;
    z = v.z;
}
std::string Quaternion::toString() const{
    std::string s;
    s.append("(");
    s.append(std::to_string(w));
    s.append(", ");
    s.append(std::to_string(x));
    s.append(", ");
    s.append(std::to_string(y));
    s.append(", ");
    s.append(std::to_string(z));
    s.append(")");
    
    return s;
}
Quaternion::Quaternion(){};
Quaternion::Quaternion(float w, float x, float y, float z): w(w), x(x), y(y), z(z){};
Quaternion::Quaternion(float angle, Vec3 unit){
    float half_angle = angle / 2;
    w = cos(half_angle);
    x = sin(half_angle) * unit.x;
    y = sin(half_angle) * unit.y;
    z = sin(half_angle) * unit.z;
};
Quaternion Quaternion::operator*(const Quaternion &other) const{
    Quaternion result;
    result.w = w * other.w - dot(getV(), other.getV());
    result.setV(
        other.getV() * w + getV() * other.w + crossVec(getV(), other.getV())
    );

    return result;
}
Quaternion Quaternion::operator*(const Vec3 &p) const{
    Quaternion pure;
    pure.w = 0;
    pure.setV(p);

    return *(this) * pure;
}
Quaternion Quaternion::rotate(const Vec3 &p) const{
    Quaternion inv;
    inv.w = w;
    inv.setV(getV() * -1);
    Quaternion q = *(this) * p * inv;

    return q;
}
float Quaternion::calcNorm() const{
    return sqrt(w*w + x*x + y*y + z*z);
}
Quaternion Quaternion::getConjugate() const{
    Quaternion conjugate;
    conjugate.w = w;
    conjugate.setV(getV() * -1);

    return conjugate;
}
Quaternion Quaternion::getInverse2() const{
    Quaternion inv;
    Quaternion conjugate = getConjugate();
    float norm = calcNorm();
    inv.w = conjugate.w / (norm * norm);
    inv.x = conjugate.x / (norm * norm);
    inv.y = conjugate.y / (norm * norm);
    inv.z = conjugate.z / (norm * norm);

    return inv;
}
Quaternion Quaternion::getInverse() const{
    Quaternion inv;
    inv.w = w;
    inv.setV(getV() * -1);

    return inv;
}
void Quaternion::getRotateMatrix(M4x4 &matrix) const{
    matrix.init(
        1 - 2*(y*y + z*z), 2*(x*y - w*z), 2*(x*z + w*y), 0,
        2*(x*y + w*z), 1 - 2*(x*x + z*z), 2*(y*z - w*x), 0,
        2*(x*z - w*y), 2*(y*z + w*x), 1 - 2*(x*x + y*y), 0,
        0, 0, 0, 1
    );
}
