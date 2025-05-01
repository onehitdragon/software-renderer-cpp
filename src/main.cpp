#include <string>
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "teapotmodel.h"
#include "common/fixednumber.h"
#include "helper.h"
#include "asset.h"
#include "importer/fbximporter.h"
#include <cmath>
#include <math.h>
#include <algorithm>

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

// orbit camera
Vec3 target = {0, 0, 0};
float radius = 10;
float azimuthalAngle = 0;
float polarAngle = 0;

void updateCamera(const Vec2 &mouseDir, Instance &instance){
    // azimuthalAngle += 0.1 * (mouseDir.x * -1);
    polarAngle += 0.1 * (mouseDir.y * -1);
    
    Vec3 cameraPos;
    float azimuthalRad = azimuthalAngle * M_PI / 180;
    float polarRad = polarAngle * M_PI / 180;
    cameraPos.x = radius * std::cos(azimuthalRad) * std::sin(polarRad);
    cameraPos.z = radius * std::sin(azimuthalRad) * std::sin(polarRad);
    cameraPos.y = radius * std::cos(polarRad);
    std::cout << vecToString(cameraPos) << std::endl;

    Vec3 targetPos = normalizeVec(subVec(target, cameraPos));
    float lookat_AroundY = std::atan2(targetPos.x, targetPos.z) * 180 / M_PI;
    float lookat_AroundX = -std::asin(targetPos.y) * 180 / M_PI;

    camera.transform.translation = cameraPos;
    camera.transform.rotation.y = lookat_AroundY;
    camera.transform.rotation.x = lookat_AroundX;

    std::cout << lookat_AroundY << " " << lookat_AroundX << std::endl;
    // std::cout << targetPos.y << " " << targetPos.z << " " << std::atan2(targetPos.y, targetPos.z) << std::endl;
}

int main(){
    init_canvas_buffer(WINDOW_WIDTH, WINDOW_HEIGHT, 2, 2);
    init_fixed_number(4);
    createTeapotInstance();
    createCubeInstance();
    createTexture("assets/Test/Cube_texture.png");
    // createTexture("assets/crate-texture.jpg");
    // createTexture("assets/texture_img_1.png");
    // createTexture("assets/Stair/Stair_texture.png");
    // createTexture("assets/House/Texturelabs_Brick_163S.jpg");

    Model *model = new Model();
    Instance importIns = {model};
    importIns.transform = {{0, 0, 0}, {0, 0, 0}, 1};
    importFBX("assets/Test/example1.fbx", model);
    // importFBX("assets/example1.fbx", model);
    // importFBX("assets/House/House.fbx", model);
    // importFBX("assets/Stair/Stair.fbx", model);

    // return 0;

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
    Instance inst = std::ref(importIns);
    int idx = 0;
    float mouseX_old = 0, mouseY_old = 0;
    
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
                if(event.key.key == SDLK_PAGEUP){
                    inst.transform.translation.z += 0.1f;
                }
                if(event.key.key == SDLK_PAGEDOWN){
                    inst.transform.translation.z -= 0.1f;
                }
                if(event.key.key == SDLK_SPACE){
                    // polarAngle += 1;
                    // updateCamera(inst);
                    inst.transform.rotation.y += 1;
                }
                if(event.key.key == SDLK_BACKSPACE){
                    // polarAngle += 1;
                    // updateCamera(inst);
                    inst.transform.rotation.x += 1;
                }

                if(event.key.key == SDLK_A){
                    camera.transform.translation.x -= 0.1f;
                }
                if(event.key.key == SDLK_D){
                    camera.transform.translation.x += 0.1f;
                }
                if(event.key.key == SDLK_W){
                    camera.transform.translation.y += 0.1f;
                }
                if(event.key.key == SDLK_S){
                    camera.transform.translation.y -= 0.1f;
                }
                if(event.key.key == SDLK_Q){
                    camera.transform.translation.z += 0.1f;
                }
                if(event.key.key == SDLK_E){
                    camera.transform.translation.z -= 0.1f;
                }
                if(event.key.key == SDLK_F){
                    camera.transform.rotation.y += 0.1f;
                }
            }
            if(event.type == SDL_EVENT_MOUSE_WHEEL){
                radius = std::max(0.0f, radius - 0.1f * event.wheel.y);
                // std::cout << radius << std::endl;
                // updateCamera({0, 0}, inst);
            }
        }

        float mouseX_new, mouseY_new;
        SDL_MouseButtonFlags buttons = SDL_GetMouseState(&mouseX_new, &mouseY_new);
        Vec2 mouseDir = {mouseX_new - mouseX_old, mouseY_new - mouseY_old};
        mouseX_old = mouseX_new;
        mouseY_old = mouseY_new;
        const bool *keystates = SDL_GetKeyboardState(NULL);
        if(keystates[SDL_SCANCODE_LALT] && keystates[SDL_SCANCODE_LSHIFT] && buttons == SDL_BUTTON_LEFT){
            camera.transform.translation.x += 0.1 * (mouseDir.x * -1);
            camera.transform.translation.y += 0.1 * mouseDir.y;
        }
        if(keystates[SDL_SCANCODE_LALT] && !keystates[SDL_SCANCODE_LSHIFT] && buttons == SDL_BUTTON_LEFT){
            updateCamera(mouseDir, inst);
        }

        // main
        clear_screen();
        // inst.transform.rotation.y += 0.2f;
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