#include <SDL.h>
#include <stdio.h>
#include "Chip8.h"

Chip8 chip8;
int main(int argc, char* args[]) {
    SDL_Event event;
    SDL_Renderer* renderer;
    SDL_Window* window;
    int i;

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 64*20, 32*20, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderSetScale(renderer, 20, 20);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (i = 0; i < 32; ++i) {
        SDL_RenderDrawPoint(renderer, i, i);
        SDL_RenderDrawPoint(renderer, 63-i, i);
    }
    SDL_RenderPresent(renderer);
    while (1) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
    if (argc < 3) {
        return 0;
    }
    else {
        char* filename = args[1];
        char* SCALE_FACTOR = args[2];
        chip8.initialize();
        chip8.LoadROM(filename);
        chip8.cycle();
    }
}