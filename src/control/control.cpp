#include <SDL.h>

bool is_pressed_left(int* mouse_x, int* mouse_y){ 
    Uint32 mouse_state = SDL_GetMouseState(mouse_x, mouse_y);
    if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) return true;
    else return false;

}