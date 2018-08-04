#ifndef KEYBOARD_STATE_HPP_
#define KEYBOARD_STATE_HPP_

#include <stdint.h>

#include <SDL2/SDL.h>

class keyboard_state {
protected:
    bool up_pressed = 0;
    bool left_pressed = 0;
    bool down_pressed = 0;
    bool right_pressed = 0;
    bool w_pressed = 0;
    bool a_pressed = 0;
    bool s_pressed = 0;
    bool d_pressed = 0;
public:
    keyboard_state();
    void update_state(uint32_t event_type, SDL_Keycode keycode);
    bool get_up_pressed();
    bool get_left_pressed();
    bool get_down_pressed();
    bool get_right_pressed();
    bool get_w_pressed();
    bool get_a_pressed();
    bool get_s_pressed();
    bool get_d_pressed();
};

#endif // KEYBOARD_STATE_HPP_
