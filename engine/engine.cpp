#include <stdexcept>
#include <string>

#include <SDL2/SDL.h>

#include "engine/engine.hpp"

engine::engine() {
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
        throw std::runtime_error("SDL_Init failed: " + std::string(SDL_GetError()));
    }
}

engine::~engine() {
     SDL_Quit();
}

