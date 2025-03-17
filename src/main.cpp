#include <string>
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "teapotmodel.h"
#include "common/fixednumber.h"
#include "helper.h"

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;

void initPixels(){
    for(int i = 0; i < canvasBufferLength; i += 4){
        canvasBuffer[i] = 255;
        canvasBuffer[i + 1] = 255;
        canvasBuffer[i + 2] = 255;
        canvasBuffer[i + 3] = 255;
    }
}

int main(){
    init_canvas_buffer(WINDOW_WIDTH, WINDOW_HEIGHT, 2, 2);
    init_fixed_number(4);
    createTeapotInstance();
    createCubeInstance();

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
    Instance inst = std::ref(teapotInstance);
    int idx = 0;
    while(loop){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_EVENT_QUIT){
                loop = false;
            }
            if(event.type == SDL_EVENT_KEY_DOWN){
                idx++;
            }
            if(event.type == SDL_EVENT_KEY_DOWN){
                if(event.key.key == SDLK_LEFT){
                    inst.transform.translation.x -= 0.1f;
                }
                if(event.key.key == SDLK_RIGHT){
                    inst.transform.translation.x += 0.1f;
                }
                if(event.key.key == SDLK_UP){
                    inst.transform.translation.y += 0.1f;
                }
                if(event.key.key == SDLK_DOWN){
                    inst.transform.translation.y -= 0.1f;
                }
            }
        }

        // main
        // inst.transform.rotation += 0.1f;
        render_instance(inst, idx);
        SDL_UpdateTexture(texture, NULL, canvasBuffer, WINDOW_WIDTH * 4);
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
        // break;
    }

    return 0;
}