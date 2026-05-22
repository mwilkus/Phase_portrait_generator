#include <SDL.h>
#include "config.h"

bool is_pressed_left(int* mouse_x, int* mouse_y){ 
    Uint32 mouse_state = SDL_GetMouseState(mouse_x, mouse_y);
    if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) return true;
    else return false;

}

void change_scale(float* scale){
    const uint8_t *keyboard_state = SDL_GetKeyboardState(NULL);
    if (keyboard_state[SDL_SCANCODE_UP] == true) {
        *scale *= SCALE_CHANGE_RATE;
    } else if (keyboard_state[SDL_SCANCODE_DOWN] == true) {
        *scale /= SCALE_CHANGE_RATE;
    }
}