#include <SDL2/SDL_error.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "settings.hpp"

Game game;
Display display;
Player player = Player("./assets/keanu.jpg");
Platform platform = Platform(0, display.height - 50, display.width, 50, "./assets/josh.jpg");

int main(int argc, char* argv[]) {
	game.update_objects = {&player, &platform};
	game.platforms = {&platform};

	while (game.running) {
		game.process_input();
		game.update();
	}

	for (Sprite* object: game.update_objects)
		SDL_DestroyTexture(object->texture);

	SDL_Quit();
	SDL_DestroyWindow(display.window);
	SDL_DestroyRenderer(display.renderer);
	return 0;
}
