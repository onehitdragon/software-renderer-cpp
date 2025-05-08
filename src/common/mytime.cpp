#include "mytime.h"

float MyTime::deltaTime = 0;
Uint64 MyTime::deltaTime_prev = 0;
void MyTime::calcDeltaTime(){
    Uint64 cur = SDL_GetTicks();
    MyTime::deltaTime = (cur - MyTime::deltaTime_prev) / 1000.0f;
    MyTime::deltaTime_prev = cur;
};