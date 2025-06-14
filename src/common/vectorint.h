#ifndef VECTORINT_H
#define VECTORINT_H

class Vec2Int{
public:
    int x;
    int y;
    Vec2Int();
    Vec2Int(int x, int y);
    Vec2Int operator<<(const int n) const;
    Vec2Int operator&(const int n) const;
    Vec2Int operator+(const Vec2Int &other) const;
    Vec2Int operator+(const int &other) const;
    Vec2Int operator-(const Vec2Int &other) const;
    Vec2Int operator*(const int &other) const;
};

class Vec3Int: public Vec2Int{
public:
    int z;
    Vec3Int();
    Vec3Int(int x, int y, int z);
    Vec3Int toBool() const;
};

#endif