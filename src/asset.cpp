#include "asset.h"

uint8_t *crateTexture_pixels = nullptr;
int crateTexture_pitch;
int crateTexture_width;
int crateTexture_height;

void createTexture(std::string path){
    SDL_Surface *surface = IMG_Load(path.c_str());
    if(surface == NULL){
        std::cout << "fail IMG_Load: " << path << std::endl;
        throw std::runtime_error(SDL_GetError());
    }
    // std::cout << SDL_GetPixelFormatName(surface->format) << std::endl;
    // std::cout << surface->format << std::endl;
    // std::cout << SDL_PIXELFORMAT_RGB24 << std::endl;

    if(surface->format == SDL_PIXELFORMAT_RGB24){
        delete[] crateTexture_pixels;
        crateTexture_pixels = new uint8_t[surface->w * surface->h * 4];
        uint8_t *value = reinterpret_cast<uint8_t*>(surface->pixels);
        for(int i = 0, j = 0, n = surface->w * surface->h * 3; i < n; i += 3, j += 4){
            crateTexture_pixels[j + 0] = value[i + 0];
            crateTexture_pixels[j + 1] = value[i + 1];
            crateTexture_pixels[j + 2] = value[i + 2];
            crateTexture_pixels[j + 3] = 255;
        }
        crateTexture_pitch = surface->pitch + surface->w;
        crateTexture_width = surface->w - 1;
        crateTexture_height = surface->h - 1;
    }
    else if(surface->format == SDL_PIXELFORMAT_ABGR8888){
        // std::ofstream outfile("assets/abc.out");
        // if(!outfile.is_open()){
        //     std::cout << "cant open to write: " << path << std::endl;
        //     return;
        // }
        // uint8_t *value = reinterpret_cast<uint8_t*>(surface->pixels);
        // for(int i = 0; i < 500000; i+=4){
        //     outfile << static_cast<int>(value[i]) << " ";
        //     outfile << static_cast<int>(value[i + 1]) << " ";
        //     outfile << static_cast<int>(value[i + 2]) << " ";
        //     outfile << static_cast<int>(value[i + 3]) << " ";
        //     outfile << std::endl;
        // }

        delete[] crateTexture_pixels;
        crateTexture_pixels = new uint8_t[surface->w * surface->h * 4];
        uint8_t *value = reinterpret_cast<uint8_t*>(surface->pixels);
        for(int i = 0, n = surface->w * surface->h * 4; i < n; i++){
            crateTexture_pixels[i] = value[i];
        }
        crateTexture_pitch = surface->pitch;
        crateTexture_width = surface->w - 1;
        crateTexture_height = surface->h - 1;
    }
    else{
        std::string error = "Unsupported pixel format: ";
        error.append(SDL_GetPixelFormatName(surface->format));
        SDL_DestroySurface(surface);
        throw std::runtime_error(error);
    }

    SDL_DestroySurface(surface);
}
