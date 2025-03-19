#ifndef FIXEDNUMBER_H
#define FIXEDNUMBER_H

#include "vector.h"
#include "vectorint.h"

extern unsigned int fixedNumber_RESOLUTION;
extern unsigned int fixedNumber_MULTIPLIER;
void init_fixed_number(unsigned int resolution);

Vec2Int fixedNumber_fixedXY(const Vec2 &vec);

#endif