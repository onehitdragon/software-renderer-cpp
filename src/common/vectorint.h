#ifndef VECTORINT_H
#define VECTORINT_H

class Vec2Int{
public:
    int x;
    int y;
    Vec2Int();
    Vec2Int(int x, int y);
    Vec2Int operator<<(const int n) const;
    Vec2Int operator+(const Vec2Int &other) const;
    Vec2Int operator-(const Vec2Int &other) const;
};

#endif