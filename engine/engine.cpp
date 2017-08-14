#include <stdexcept>
#include <string>

#include <SDL2/SDL.h>

#include "engine/engine.hpp"
#include "engine/window.hpp"

engine::engine() {
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
        throw std::runtime_error("SDL_Init failed: " + std::string(SDL_GetError()));
    }
}

engine::~engine() {
     SDL_Quit();
}

void engine::loop() {
    window main_window;
    SDL_Event event;
    bool done = false;
    while (!done) {
        main_window.swap();

        SDL_Delay(10);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = true;
            }
        }
    }
}
