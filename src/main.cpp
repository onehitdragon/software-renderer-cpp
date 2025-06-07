#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "teapotmodel.h"
#include "common/fixednumber.h"
#include "helper.h"
#include "asset.h"
#include "importer/fbximporter.h"
#include "common/mytime.h"
#include "common/camera.h"
#include "common/quaternion.h"

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;
const int WINDOW_WIDTH_HALF = 250;
const int WINDOW_HEIGHT_HALF = 250;

void initPixels(){
    for(int i = 0; i < canvasBufferLength; i += 4){
        canvasBuffer[i] = 255;
        canvasBuffer[i + 1] = 255;
        canvasBuffer[i + 2] = 255;
        canvasBuffer[i + 3] = 255;
    }
}

// fps camera
FPSCamera fpsCamera;
ArcballCamera arcballCamera;
TrackballCamera trackballCamera;
M4x4 m;

int main(){
    init_canvas_buffer(WINDOW_WIDTH, WINDOW_HEIGHT, 2, 2);
    init_fixed_number(4);
    createTeapotInstance();
    createCubeInstance();
    createTexture("assets/Test2/test.png");
    // createTexture("assets/crate-texture.jpg");
    // createTexture("assets/texture_img_1.png");
    // createTexture("assets/Stair/Stair_texture.png");
    // createTexture("assets/House/Texturelabs_Brick_163S.jpg");

    Model *model = new Model();
    Instance importIns = {model};
    importIns.transform = {{0, 0, 0}, {0, 0, 0}, 1};
    importFBX("assets/Test2/test2.fbx", model);
    // importFBX("assets/example1.fbx", model);
    // importFBX("assets/Stair/Stair.fbx", model);
    // importFBX("assets/House/House.fbx", model);

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
    // SDL_SetWindowRelativeMouseMode(window, true);
    // SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_CENTER, "0");

    // loop
    bool loop = true;
    int fps = 0;
    std::string fpsText = "FPS: ";
    Uint64 startTime = SDL_GetTicks();
    Instance inst = std::ref(importIns);
    int idx = 0;
    bool pressed = false;
    bool enableX = false, enableY = false;
    // float mouseX_old = 0, mouseY_old = 0;
    
    SDL_Event event;
    while(loop){
        MyTime::calcDeltaTime();
        while(SDL_PollEvent(&event)){
            SDL_Keymod mod = SDL_GetModState();

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
                    inst.transform.rotation.y += 1;
                }
                if(event.key.key == SDLK_BACKSPACE){
                    inst.transform.rotation.x += 1;
                }
                if(event.key.key == SDLK_Q){
                    enableX = !enableX;
                    std::cout << "enableX: " << enableX << std::endl;
                }
                if(event.key.key == SDLK_E){
                    enableY = !enableY;
                    std::cout << "enableY: " << enableY << std::endl;
                }
            }

            // Vec2 ndcMouse;
            // SDL_GetMouseState(&ndcMouse.x, &ndcMouse.y);
            // ndcMouse.x = (ndcMouse.x - WINDOW_WIDTH_HALF) / WINDOW_WIDTH_HALF;
            // ndcMouse.y = (ndcMouse.y - WINDOW_HEIGHT_HALF) / WINDOW_HEIGHT_HALF * -1;
            // if(!enableX){
            //     ndcMouse.x = 0;
            // }
            // if(!enableY){
            //     ndcMouse.y = 0;
            // }
            // if(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
            //     pressed = true;
            //     arcballCamera.press(ndcMouse);
            // }
            // if(event.type == SDL_EVENT_MOUSE_BUTTON_UP){
            //     pressed = false;
            // }
            // if(event.type == SDL_EVENT_MOUSE_MOTION && pressed){
            //     arcballCamera.move(ndcMouse);
            // }

            Vec2 mouseDir;
            SDL_GetRelativeMouseState(&mouseDir.x, &mouseDir.y);
            mouseDir.y = -mouseDir.y;
            if(!enableX){
                mouseDir.x = 0;
            }
            if(!enableY){
                mouseDir.y = 0;
            }
            if(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
                pressed = true;
            }
            if(event.type == SDL_EVENT_MOUSE_BUTTON_UP){
                pressed = false;
            }
            if(event.type == SDL_EVENT_MOUSE_MOTION && pressed){
                if((mod & SDL_KMOD_ALT) && !(mod & SDL_KMOD_LSHIFT)){
                    trackballCamera.move(mouseDir);
                }
                if((mod & SDL_KMOD_LSHIFT) && (mod & SDL_KMOD_ALT)){
                    trackballCamera.pan(mouseDir);
                }
            }
        }

        float speed = 5 * MyTime::deltaTime;
        const bool *keystates = SDL_GetKeyboardState(NULL);
        if(keystates[SDL_SCANCODE_W]){
            fpsCamera.position = fpsCamera.position + fpsCamera.forward * speed;
        }
        if(keystates[SDL_SCANCODE_S]){
            fpsCamera.position = fpsCamera.position - fpsCamera.forward * speed;
        }
        if(keystates[SDL_SCANCODE_D]){
            fpsCamera.position = fpsCamera.position + fpsCamera.right * speed;
        }
        if(keystates[SDL_SCANCODE_A]){
            fpsCamera.position = fpsCamera.position - fpsCamera.right * speed;
        }

        // main
        clear_screen();
        // inst.transform.rotation.y += 100.0f * MyTime::deltaTime;
        render_instance(inst, trackballCamera, idx);
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