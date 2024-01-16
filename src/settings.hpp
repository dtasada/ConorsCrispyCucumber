#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
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
#include <set>

#define GAME_TITLE "game lmao"
#define DEFAULT_FRAMETIME (1000.0 / 60.0)

struct Display {
	SDL_Window* window;
	SDL_Renderer* renderer;
	unsigned int width = 1280;
	unsigned int height = 720;

	Display() {
		this->window = SDL_CreateWindow(GAME_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->width, this->height, (Uint32)NULL);
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
	int w, h;
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect rect;

	Sprite(int x_arg, int y_arg, int w_arg, int h_arg, const char* path_arg) {
		x = x_arg;
		y = y_arg;
		w = w_arg;
		h = h_arg;
		const char* path = path_arg;
		surface = IMG_Load(path);
		if (!surface){
			fprintf(stderr, "SDL error: %s\n", SDL_GetError());
		}
		texture = SDL_CreateTextureFromSurface(display.renderer, surface);
		if (!texture)
			fprintf(stderr, "SDL errosr: %s\n", SDL_GetError());
		rect = SDL_Rect{x, y, w, h};

	}

	void update_rect() {
		this->rect.x = this->x;
		this->rect.y = this->y;
	}

	virtual void update() {
		this->update_rect();
		SDL_RenderCopy(display.renderer, texture, NULL, &rect);
	}

	void process_input(SDL_Event* event) {}
};

struct Platform : Sprite {
	Platform(int x, int y, int w, int h, const char* path) : Sprite(x, y, w, h, path) {};
	
	void update() override {
		Sprite::update();
	}
};

struct Game {
	bool running = true;
	unsigned int last_frame_time;
	unsigned int fps;
	float delta_time;

	std::vector<Sprite*> update_objects;
	std::vector<Platform*> platforms;
	std::set<SDL_Keycode> keys;

	void process_input() {
		SDL_Event event;
		SDL_PollEvent(&event);

		switch (event.type) {
			case SDL_QUIT:
				running = false;
			break;
			case SDL_KEYDOWN:
				keys.insert(event.key.keysym.sym);
			break;
			case SDL_KEYUP:
				keys.erase(event.key.keysym.sym);
		}
		for (Sprite* object: this->update_objects) {
			object->process_input(&event);
		}
	}

	void update() {
		SDL_RenderClear(display.renderer);
		SDL_Delay(1000.0 / 60);

		int frametime = SDL_GetTicks() - last_frame_time;
		fps = 1000.0 / frametime;
	
		last_frame_time = SDL_GetTicks();
		delta_time = frametime / DEFAULT_FRAMETIME;
		delta_time = 1;

		for (Sprite* object: update_objects) {
			object->update();
		}

		SDL_SetRenderDrawColor(display.renderer, 120, 120, 120, 255);
		SDL_RenderPresent(display.renderer);
	}
};
extern Game game;

struct Player : Sprite {
	float x_vel = 10;
	float y_vel = 0;
	float y_acc = 0.06;
	Player(const char* path) : Sprite(0, 0, 50, 50, path) {};

	void collide() {
		// move y
		this->y_vel += 0.5 * this->y_acc * game.delta_time;
		this->y += this->y_vel * game.delta_time;
		this->y_vel += 0.5 * this->y_acc * game.delta_time;
		// update
		this->update_rect();
		// collide y
		for (Platform* platform: game.platforms) {
			// printf("%f   %f\n", this->frect.y, platform->frect.y);
 			bool col = SDL_HasIntersection(&this->rect, &platform->rect);
			if (col) {
				this->y = platform->rect.y - this->rect.h;
				this->y_vel = -5;
			}
		}
		// update
		this->update_rect();
	}

	void update() override {
		if (game.keys.count(SDLK_s))
			this->y += this->y_vel * game.delta_time;
		if (game.keys.count(SDLK_d))
			this->x += this->x_vel * game.delta_time;
		if (game.keys.count(SDLK_a))
			this->x -= this->x_vel * game.delta_time;
		this->collide();
		Sprite::update();
	}

	void process_input(SDL_Event* event) {}
};
