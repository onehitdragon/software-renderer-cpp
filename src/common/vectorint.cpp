#include "vectorint.h"

Vec2Int::Vec2Int(): x(0), y(0){}
Vec2Int::Vec2Int(int x, int y): x(x), y(y){}
Vec2Int Vec2Int::operator<<(const int n) const{
    return {
        x << n,
        y << n
    };
}
Vec2Int Vec2Int::operator+(const Vec2Int &other) const{
    return {
        x + other.x,
        y + other.y
    };
}
Vec2Int Vec2Int::operator-(const Vec2Int &other) const{
    return {
        x - other.x,
        y - other.y
    };
}