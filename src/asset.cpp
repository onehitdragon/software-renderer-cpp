#include "asset.h"
#include <SDL3_image/SDL_image.h>
#include <string>
#include <iostream>

Uint8 *crateTexture_pixels;
int crateTexture_pitch;
int crateTexture_width;
int crateTexture_height;
void createCrateTexture(){
    std::string path = "assets/crate-texture.jpg";
    SDL_Surface *surface = IMG_Load(path.c_str());
    if(surface == NULL){
        std::cout << "fail IMG_Load: " << path << std::endl;
        std::cout << SDL_GetError() << std::endl;
        return;
    }
    crateTexture_pixels = (Uint8*)surface->pixels;
    crateTexture_pitch = surface->pitch;
    crateTexture_width = surface->w - 1;
    crateTexture_height = surface->h - 1;
}
