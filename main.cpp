#include <SDL.h>
#include <stdio.h>
#include "Chip8.h"
#include "SDLHandler.h"
#include <string>
#include <chrono>
#include <thread>

Chip8 chip8;
SDLHandler graphics;
int main(int argc, char* args[]) {
    if (argc < 3) {
        return 0;
    }
    char* filename = args[1];
    int SCALE_FACTOR = std::stoi(args[2]);
    int CYCLE_DELAY = 1;
    int SCREEN_WIDTH = 64;
    int SCREEN_HEIGHT = 32;
    chip8.initialize(SCREEN_WIDTH, SCREEN_HEIGHT);
    chip8.LoadROM(filename);
    graphics.initialize("My Emulator", SCREEN_WIDTH, SCREEN_HEIGHT, SCALE_FACTOR);
    bool quit = false;
    auto last_cycle_time = std::chrono::high_resolution_clock::now();
    int pitch = sizeof(chip8.screen[0]) * SCREEN_WIDTH;
    while (!quit) {
        graphics.process_input(chip8.keypad);
        auto current_time = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(current_time - last_cycle_time).count();
        if (dt > CYCLE_DELAY) {
            last_cycle_time = current_time;
            chip8.cycle();
            graphics.update(chip8.screen, pitch);
        }
    }
    return 0;

}