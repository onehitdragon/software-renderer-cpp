#include "fixednumber.h"
#include <cmath>

unsigned int fixedNumber_RESOLUTION = 0;
unsigned int fixedNumber_MULTIPLIER = 0;
void init_fixed_number(unsigned int resolution){
    fixedNumber_RESOLUTION = resolution;
    fixedNumber_MULTIPLIER = std::pow(2, resolution);
}

Vec2Int fixedNumber_fixedXY(const Vec2 &vec){
    return {
        (int)std::round(vec.x * fixedNumber_MULTIPLIER),
        (int)std::round(vec.y * fixedNumber_MULTIPLIER)
    };
}