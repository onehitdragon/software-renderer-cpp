#include <string>
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;
Uint8 *pixels;
const int pixelsLength = WINDOW_WIDTH * WINDOW_HEIGHT * 4;

void initPixels(){
    pixels = new Uint8[pixelsLength];
    for(int i = 0; i < pixelsLength; i += 4){
        pixels[i] = 255;
        pixels[i + 1] = 255;
        pixels[i + 2] = 255;
        pixels[i + 3] = 255;
    }
}

void updatePixels(){
    for(int i = 0; i < pixelsLength; i += 4){
        pixels[i] = 0;
        pixels[i + 1] = 255;
        pixels[i + 2] = 0;
        pixels[i + 3] = 255;
    }
}

int main(){
    initPixels();

    std::cout << "hello w" << std::endl;
    if(!SDL_Init(SDL_INIT_VIDEO)){
        std::cout << "fail SDL_INIT_VIDEO" << std::endl;
        std::cout << SDL_GetError() << std::endl;
        return 1;
    }
    if(!TTF_Init()){
        std::cout << "fail TTF_Init" << std::endl;
        std::cout << SDL_GetError() << std::endl;
        return 1;
    }

    
    SDL_Window *window = SDL_CreateWindow("Software Renderer", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if(window == NULL){
        std::cout << "fail SDL_CreateWindow" << std::endl;
        std::cout << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if(renderer == NULL){
        std::cout << "fail SDL_CreateRenderer" << std::endl;
        std::cout << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
    if(texture == NULL){
        std::cout << "fail SDL_CreateTexture" << std::endl;
        std::cout << SDL_GetError() << std::endl;
        return 1;
    }

    // text config
    TTF_Font *font = TTF_OpenFont("font/TimesNewRoman.ttf", 18);
    if(font == NULL){
        std::cout << "fail TTF_OpenFont" << std::endl;
        std::cout << SDL_GetError() << std::endl;
        return 1;
    }

    // loop
    bool loop = true;
    int fps = 0;
    std::string fpsText = "FPS: ";
    Uint64 startTime = SDL_GetTicks();
    while(loop){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_EVENT_QUIT){
                loop = false;
            }
            if(event.type == SDL_EVENT_KEY_DOWN){
            }
        }

        // main
        SDL_UpdateTexture(texture, NULL, pixels, WINDOW_WIDTH * 4);
        SDL_RenderTexture(renderer, texture, NULL, NULL);

        // fps
        if(SDL_GetTicks() - startTime >= 1000){
            fpsText = "FPS: " + std::to_string(fps);
            fps = 0;
            startTime = SDL_GetTicks();
        }
        else{
            fps++;
        }
        SDL_Surface *textSurface = TTF_RenderText_Solid(font, fpsText.c_str(), fpsText.length(), {0, 255, 0, 255});
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FRect textFRect = {0, 0, (float)textSurface->w, (float)textSurface->h};
        SDL_RenderTexture(renderer, textTexture, NULL, &textFRect);
        SDL_DestroySurface(textSurface);
        SDL_DestroyTexture(textTexture);

        // final
        SDL_RenderPresent(renderer);
    }

    return 0;
}