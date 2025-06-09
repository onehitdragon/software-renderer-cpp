#include "drawhelper.h"

void drawLineLow(Vec2 p0, Vec2 p1, const Vec4 &color){
    int dx = p1.x - p0.x;
    int dy = p1.y - p0.y;
    int yi = 1;
    if(dy < 0){
        dy = -dy;
        yi = -1;
    }
    int D = 2*dy - dx;
    int y = p0.y;
    for(int x = p0.x; x <= p1.x; x++){
        putPixel(x, y, color, 0);
        if(D > 0){
            y = y + yi;
            D = D - 2*dx;
        }
        D = D + 2*dy;
    }
}
void drawLineHigh(Vec2 p0, Vec2 p1, const Vec4 &color){
    int dx = p1.x - p0.x;
    int dy = p1.y - p0.y;
    int xi = 1;
    if(dx < 0){
        dx = -dx;
        xi = -1;
    }
    int D = 2*dx - dy;
    int x = p0.x;
    for(int y = p0.y; y <= p1.y; y++){
        putPixel(x, y, color, 0);
        if(D > 0){
            x = x + xi;
            D = D - 2*dy;
        }
        D = D + 2*dx;
    }
}
void DrawHelper::drawLine(const Vec3 &start, const Vec3 &end, const Vec4 &color){
    Vec3 startProj = project(start);
    Vec3 endProj = project(end);
    Vec2 p0 = viewportToCanvasCoordinate(startProj);
    Vec2 p1 = viewportToCanvasCoordinate(endProj);
    p0 = {std::round(p0.x), std::round(p0.y)};
    p1 = {std::round(p1.x), std::round(p1.y)};

    int dx = std::abs(p1.x - p0.x);
    int dy = std::abs(p1.y - p0.y);
    if(dx > dy){
        if(p0.x < p1.x){
            drawLineLow(p0, p1, color);
        }
        else{
            drawLineLow(p1, p0, color);
        }
    }
    else{
        if(p0.y < p1.y){
            drawLineHigh(p0, p1, color);
        }
        else{
            drawLineHigh(p1, p0, color);
        }
    }
}