#ifndef MYTIME_H
#define MYTIME_H

#include <SDL3/SDL.h>

class MyTime{
public:
    static float deltaTime;
    static Uint64 deltaTime_prev;
    static void calcDeltaTime();
};

#endif