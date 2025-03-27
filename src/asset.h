#ifndef ASSET_H
#define ASSET_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <iostream>
#include <fstream>

extern Uint8 *crateTexture_pixels;
extern int crateTexture_pitch;
extern int crateTexture_width;
extern int crateTexture_height;
void createTexture(std::string path);

#endif