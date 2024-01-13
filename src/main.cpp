#include <SDL2/SDL_error.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL.h>

#include "./settings.hpp"

Game game;
Display display;
Player player;

int main(int argc, char* argv[]) {
	while (game.running) {
		game.process_input();
		game.update();
	}

	SDL_Quit();
	SDL_DestroyWindow(display.window);
	SDL_DestroyRenderer(display.renderer);
	return 0;
}