#include "engine/keyboard_state.hpp"

keyboard_state::keyboard_state() {}

void keyboard_state::update_state(uint32_t event_type, SDL_Keycode keycode) {
    switch (event_type) {
        case SDL_KEYDOWN:
            switch (keycode) {
                case SDLK_UP:    this->up_pressed = 1; break;
                case SDLK_LEFT:  this->left_pressed = 1; break;
                case SDLK_DOWN:  this->down_pressed = 1; break;
                case SDLK_RIGHT: this->right_pressed = 1; break;
                case SDLK_w: this->w_pressed = 1; break;
                case SDLK_a: this->a_pressed = 1; break;
                case SDLK_s: this->s_pressed = 1; break;
                case SDLK_d: this->d_pressed = 1; break;
            }
            break;
        case SDL_KEYUP:
            switch (keycode) {
                case SDLK_UP:    this->up_pressed = 0; break;
                case SDLK_LEFT:  this->left_pressed = 0; break;
                case SDLK_DOWN:  this->down_pressed = 0; break;
                case SDLK_RIGHT: this->right_pressed = 0; break;
                case SDLK_w: this->w_pressed = 0; break;
                case SDLK_a: this->a_pressed = 0; break;
                case SDLK_s: this->s_pressed = 0; break;
                case SDLK_d: this->d_pressed = 0; break;
            }
            break;
    }
}

bool keyboard_state::get_up_pressed() {
    return this->up_pressed;
}

bool keyboard_state::get_left_pressed() {
    return this->left_pressed;
}

bool keyboard_state::get_down_pressed() {
    return this->down_pressed;
}

bool keyboard_state::get_right_pressed() {
    return this->right_pressed;
}

bool keyboard_state::get_w_pressed() {
    return this->w_pressed;
}

bool keyboard_state::get_a_pressed() {
    return this->a_pressed;
}

bool keyboard_state::get_s_pressed() {
    return this->s_pressed;
}

bool keyboard_state::get_d_pressed() {
    return this->d_pressed;
}

