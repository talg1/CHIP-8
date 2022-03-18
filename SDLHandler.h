#pragma once
#include <SDL.h>

class SDLHandler {
private:
	SDL_Event event;
	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Texture* texture;
	int i;
public:
	void initialize(const char* title, int SCREEN_WIDTH, int SCREEN_HEIGHT, int SCALE_FACTOR);
	~SDLHandler() {
		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	void update(void const* buffer, int pitch);
};