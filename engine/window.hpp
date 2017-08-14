#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <SDL2/SDL.h>

class window {
protected:
    SDL_Window* sdl_window;
    SDL_GLContext sdl_glcontext;
public:
    window();
    ~window();
    void swap();
};

#endif // WINDOW_HPP_
