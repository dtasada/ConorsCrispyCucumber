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
Player conor = Player("./assets/img/conor_standing.png");
Enemy jochem = Enemy("./assets/img/jochem_standing.png");
Sprite cucumber = Sprite(0, 0, 200, 60, "./assets/img/cucumber.png", 0);
Platform platform = Platform(0, display.height - 50, display.width, 50, "./assets/img/josh.jpg");
Button button = Button(50, 50, 100, 100, "ASD", (Uint32[]){255, 255, 255, 255}, [] { return 0; });

int main(int argc, char *argv[]) {
    game.update_objects = {&conor, &jochem, &platform, &button, &cucumber};
    game.platforms = {&platform};

    while (game.running) {
        game.process_input();

        cucumber.rect.x = conor.rect.x + 170;
        cucumber.rect.y = conor.rect.y + 185;

        game.update();
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
