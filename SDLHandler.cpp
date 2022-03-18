#include "SDLHandler.h"
#include <SDL.h>

void SDLHandler::initialize(const char* title, int SCREEN_WIDTH, int SCREEN_HEIGHT, int SCALE_FACTOR) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH * SCALE_FACTOR, SCREEN_HEIGHT * SCALE_FACTOR, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void SDLHandler::update(void const* buffer, int pitch)
{
    SDL_UpdateTexture(texture, nullptr, buffer, pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

bool SDLHandler::process_input(uint8_t* keys) {
    bool quit = false;
    /* Poll for events */
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            /* Keyboard event */
            /* Pass the event data onto PrintKeyInfo() */
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_x:
                        keys[0] = 1;
                        break;
                    case SDLK_1:
                        keys[1] = 1;
                        break;
                    case SDLK_2:
                        keys[2] = 1;
                        break;
                    case SDLK_3:
                        keys[3] = 1;
                        break;
                    case SDLK_q:
                        keys[4] = 1;
                        break;
                    case SDLK_w:
                        keys[5] = 1;
                        break;
                    case SDLK_e:
                        keys[6] = 1;
                        break;
                    case SDLK_a:
                        keys[7] = 1;
                        break;
                    case SDLK_s:
                        keys[8] = 1;
                        break;
                    case SDLK_d:
                        keys[9] = 1;
                        break;
                    case SDLK_z:
                        keys[10] = 1;
                        break;
                    case SDLK_c:
                        keys[11] = 1;
                        break;
                    case SDLK_4:
                        keys[12] = 1;
                        break;
                    case SDLK_r:
                        keys[13] = 1;
                        break;
                    case SDLK_f:
                        keys[14] = 1;
                        break;
                    case SDLK_v:
                        keys[15] = 1;
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                case SDLK_x:
                    keys[0] = 0;
                    break;
                case SDLK_1:
                    keys[1] = 0;
                    break;
                case SDLK_2:
                    keys[2] = 0;
                    break;
                case SDLK_3:
                    keys[3] = 0;
                    break;
                case SDLK_q:
                    keys[4] = 0;
                    break;
                case SDLK_w:
                    keys[5] = 0;
                    break;
                case SDLK_e:
                    keys[6] = 0;
                    break;
                case SDLK_a:
                    keys[7] = 0;
                    break;
                case SDLK_s:
                    keys[8] = 0;
                    break;
                case SDLK_d:
                    keys[9] = 0;
                    break;
                case SDLK_z:
                    keys[10] = 0;
                    break;
                case SDLK_c:
                    keys[11] = 0;
                    break;
                case SDLK_4:
                    keys[12] = 0;
                    break;
                case SDLK_r:
                    keys[13] = 0;
                    break;
                case SDLK_f:
                    keys[14] = 0;
                    break;
                case SDLK_v:
                    keys[15] = 0;
                    break;
                }
                break;
            /* SDL_QUIT event (window close) */
            case SDL_QUIT:
                quit = true;
                break;
        }
    }
    /* Clean up */
    if(quit)
        SDL_Quit();
    return quit;
}