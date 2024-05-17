#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

#include "settings.hpp"

Game game;
Display display;
Player player = Player("./assets/conor_standing.png");
Sprite cucumber = Sprite(0, 0, 200, 60, "./assets/cucumber.png", 0);
Platform platform =
    Platform(0, display.height - 50, display.width, 50, "./assets/josh.jpg");

Button button = Button(50, 50, 100, 100, "ASD", (Uint32[]){255, 255, 255, 255});

int main(int argc, char *argv[]) {
    game.update_objects = {&player, &platform, &button, &cucumber};
    game.platforms = {&platform};

    while (game.running) {
        game.process_input();
        game.update();
        cucumber.rect.x = player.rect.x + 170;
        cucumber.rect.y = player.rect.y + 185;
    }

    for (Sprite *object : game.update_objects) {
        SDL_DestroyTexture(object->texture);
        SDL_FreeSurface(object->surface);
    }

    SDL_Quit();
    SDL_DestroyWindow(display.window);
    SDL_DestroyRenderer(display.renderer);
    return 0;
}
