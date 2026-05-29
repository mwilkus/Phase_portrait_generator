#include "config.h"
#include <SDL.h>
#include <SDL_mouse.h>
#include <SDL_scancode.h>

bool is_pressed_left(int *mouse_x, int *mouse_y) {
  Uint32 mouse_state = SDL_GetMouseState(mouse_x, mouse_y);
  if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT))
    return true;
  else
    return false;
}

bool is_pressed_right(int *mouse_x, int *mouse_y) {
  Uint32 mouse_state = SDL_GetMouseState(mouse_x,mouse_y);
  if (mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT))
    return true;
  else
    return false;
}

bool is_pressed_enter() {
  const uint8_t *keyboard_state = SDL_GetKeyboardState(NULL);
  if (keyboard_state[SDL_SCANCODE_RETURN] == true)
    return true;
  else
    return false;
}

bool is_pressed_left_arrow() {
  const uint8_t *keyboard_state = SDL_GetKeyboardState(NULL);
  if (keyboard_state[SDL_SCANCODE_LEFT] == true)
    return true;
  else
    return false;
}

bool is_pressed_right_arrow() {
  const uint8_t *keyboard_state = SDL_GetKeyboardState(NULL);
  if (keyboard_state[SDL_SCANCODE_RIGHT] == true)
    return true;
  else
    return false;
}

bool change_scale(double *scale) {
  bool is_changed = false;
  const uint8_t *keyboard_state = SDL_GetKeyboardState(NULL);
  if (keyboard_state[SDL_SCANCODE_UP] == true) {
    is_changed = true;
    *scale *= SCALE_CHANGE_RATE;
  } else if (keyboard_state[SDL_SCANCODE_DOWN] == true) {
    *scale /= SCALE_CHANGE_RATE;
    is_changed = true;
  } else if (keyboard_state[SDL_SCANCODE_R] == true) {
    *scale = BASE_SCALE;
    is_changed = true;
  }
  return is_changed;
}