#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <cstdio>
#include <string>
#include <sys/types.h>
#include <vector>
#include <set>

#define GAME_TITLE "game lmao"
#define DEFAULT_FRAMETIME (1000.0 / 60.0)
#define LILITA TTF_OpenFont("./assets/ttf/lilita.ttf", 12)

const SDL_Color WHITE = {255, 255, 255, 255};
const SDL_Color BLACK = {0, 0, 0, 255};

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

	Sprite(int x, int y, int w, int h, const char* path_arg, const Uint32 rgba[4]) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
		if (path_arg) {
			const char* path = path_arg;
			surface = IMG_Load(path);
		} else if (rgba) {
			surface = SDL_CreateRGBSurface(0, w, h, 4, rgba[0], rgba[1], rgba[2], rgba[3]);
		} else {
			throw "Sprite passed not path or rgba value!";
		}

		if (!surface) {
			fprintf(stderr, "SDL error: %s\n", SDL_GetError());
			exit(-1);
		}

		texture = SDL_CreateTextureFromSurface(display.renderer, surface);
		if (!texture) {
			fprintf(stderr, "SDL error: %s\n", SDL_GetError());
			exit(-1);
		}
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
	Platform(int x, int y, int w, int h, const char* path) : Sprite(x, y, w, h, path, NULL) {};
	
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
	Player(const char* path) : Sprite(0, 0, 150, 210, path, NULL) {};

	void collide() {
		// move y
		this->y_vel += 0.5 * this->y_acc * game.delta_time;
		this->y += this->y_vel * game.delta_time;
		this->y_vel += 0.5 * this->y_acc * game.delta_time;

		// update
		this->update_rect();

		// collide y
		for (Platform* platform: game.platforms) {
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

struct Button : Sprite {
	std::string body_text; 
	SDL_Surface* body_surface;
	SDL_Texture* body_texture;
	SDL_Rect body_rect;

	Button(int x, int y, int w, int h, std::string body, Uint32 rgba[4]) : Sprite(x, y, w, h, NULL, rgba) {
		body_text = body;
		body_surface = TTF_RenderText_Solid(LILITA, body.c_str(), BLACK);
		body_texture = SDL_CreateTextureFromSurface(display.renderer, body_surface);
	}

	void update() override {
		SDL_RenderCopy(display.renderer, texture, NULL, &this->rect);
		SDL_RenderCopy(display.renderer, body_texture, NULL, &this->rect);
	}
};
extern Button button;
