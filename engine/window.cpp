#include <stdexcept>
#include <string>

#include "engine/window.hpp"

window::window() {
    // TODO: make this configurable
    this->sdl_window = SDL_CreateWindow(
        "SDL2/OpenGL Demo",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        700,
        700,
        SDL_WINDOW_OPENGL
    );
    if (this->sdl_window == NULL) {
        throw std::runtime_error("SDL_CreateWindow failed: " + std::string(SDL_GetError()));
    }

    this->sdl_glcontext = SDL_GL_CreateContext(this->sdl_window);
    if (this->sdl_glcontext == NULL) {
        SDL_DestroyWindow(this->sdl_window);
        throw std::runtime_error("SDL_GL_CreateContext failed: " + std::string(SDL_GetError()));
    }
}

window::~window() {
    SDL_GL_DeleteContext(this->sdl_glcontext);
    SDL_DestroyWindow(this->sdl_window);
}

void window::swap() {
    SDL_GL_SwapWindow(this->sdl_window);
}
