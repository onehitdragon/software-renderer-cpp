#include "vectorint.h"

Vec2Int::Vec2Int(): x(0), y(0){}
Vec2Int::Vec2Int(int x, int y): x(x), y(y){}
Vec2Int Vec2Int::operator<<(const int n) const{
    return {
        x << n,
        y << n
    };
}
Vec2Int Vec2Int::operator&(const int n) const{
    return {
        x & n,
        y & n
    };
}
Vec2Int Vec2Int::operator+(const Vec2Int &other) const{
    return {
        x + other.x,
        y + other.y
    };
}
Vec2Int Vec2Int::operator+(const int &other) const{
    return {
        x + other,
        y + other
    };
}
Vec2Int Vec2Int::operator-(const Vec2Int &other) const{
    return {
        x - other.x,
        y - other.y
    };
}
Vec2Int Vec2Int::operator*(const int &other) const{
    return {
        x * other,
        y * other
    };
}

Vec3Int::Vec3Int(): Vec2Int(0, 0), z(0){}
Vec3Int::Vec3Int(int x, int y, int z): Vec2Int(x, y), z(z){}
Vec3Int Vec3Int::toBool() const{
    return {
        x > 0,
        y > 0,
        z > 0
    };
};