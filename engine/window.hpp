#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <SDL2/SDL.h>

class window {
protected:
    SDL_Window* sdl_window;
    SDL_GLContext sdl_glcontext;
public:
    window();
    window(window const &) = delete;
    ~window();
    void operator=(window const &) = delete;
    void swap();
};

#endif // WINDOW_HPP_
