#include <SDL2/SDL_error.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <cstdio>
#include <sys/types.h>
#include <vector>

#define GAME_TITLE "game lmao"

struct Display {
	SDL_Window* window;
	SDL_Renderer* renderer;
	unsigned int width = 1280;
	unsigned int height = 720;

	Display() {
		this->window = SDL_CreateWindow(GAME_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->width, this->height, SDL_WINDOW_BORDERLESS);
		if (!this->window)
			fprintf(stderr, "Error creating SDL_Window%s\n", SDL_GetError());

		this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
		if (!this->renderer)
			fprintf(stderr, "Error creating SDL_Renderer%s\n", SDL_GetError());

		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
			SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
	}
};
extern Display display;

struct Sprite {
	int x, y;
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect rectangle;

	Sprite(int x_arg, int y_arg, int width, int height, const char* path) {
		x = x_arg;
		y = y_arg;
		surface = IMG_Load(path);
		if (!surface) fprintf(stderr, SDL_GetError());
		texture = SDL_CreateTextureFromSurface(display.renderer, surface);
		if (!texture) fprintf(stderr, SDL_GetError());
		rectangle = SDL_Rect{ x_arg, y_arg, width, height };

	}

	virtual void update() {
		SDL_RenderCopy(display.renderer, texture, NULL, &rectangle);
	}
};

struct Player : Sprite {
	Player() : Sprite(display.width / 2, display.height / 2, 50, 50, "./assets/keanu.jpg") {};
};
extern Player player;

extern std::vector<Sprite*> update_objects = { &player };

struct Game {
	bool running = true;
	void process_input() {
		SDL_Event event;
		SDL_PollEvent(&event);

		switch (event.type) {
			case SDL_QUIT:
				running = false;
			break;
			case SDL_KEYDOWN:
			break;
		}
	}

	void update() {
		SDL_RenderClear(display.renderer);

		for (Sprite* object: update_objects) {
			object->update();
		}

		SDL_SetRenderDrawColor(display.renderer, 0, 0 ,0, 255);
		SDL_RenderPresent(display.renderer);

		for (Sprite* object: update_objects) {
			SDL_DestroyTexture(object->texture);
		}
	}
};
extern Game game;
