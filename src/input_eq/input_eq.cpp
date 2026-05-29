#include "config.h"
#include "control.h"
#include "cordinats.h"
#include "parsing.h"
#include "input_eq.h"
#include "numeric.h"
#include <SDL.h>
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

using eq = std::pair<exprtk::expression<double>, exprtk::expression<double>>;

std::pair<bool, bool> chose_input_box(std::pair<bool, bool> *choosen,
                                      int *lines_x, int *lines_y) {
  int mouse_x, mouse_y;
  if (is_pressed_left(&mouse_x, &mouse_y)) {
    if (is_in_input_box_dx(mouse_x, mouse_y, *lines_x)) {
      choosen->first = true;
    } else {
      choosen->first = false;
    }
    if (is_in_input_box_dy(mouse_x, mouse_y, *lines_y)) {
      choosen->second = true;
    } else {
      choosen->second = false;
    }
  }
  cursor_position::choosen = *choosen; 
  return *choosen;
}

void input_logic(std::string *input, std::string *other,
                 std::pair<bool, bool> *choosen, int *lines_x, int *lines_y) {
  SDL_Event e;
  SDL_StartTextInput();
  bool is_inputing = true;
  while (is_inputing) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        is_inputing = false;
      } else if (e.type == SDL_TEXTINPUT &&
                 input->length() < MAX_INPUT_LENGTH) {
        if (choosen->first){
          input->insert(cursor_position::pos_x, e.text.text);
          cursor_position::pos_x += 1;
        }
        if (choosen->second){
          input->insert(cursor_position::pos_y, e.text.text);
          cursor_position::pos_y += 1;
        }
        is_inputing = false;
      } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_BACKSPACE) {
        int len = input->length();
        if (len > 0) {
          if (choosen->first && cursor_position::pos_x!=0){
            input->erase(cursor_position::pos_x-1,1);
            cursor_position::pos_x -= 1;
          }
          if (choosen->second && cursor_position::pos_y!=0){
            input->erase(cursor_position::pos_y-1,1);
            cursor_position::pos_y -= 1;
          }
        }
        is_inputing = false;
      }
      else{
        if (choosen->first){
          if(is_pressed_left_arrow()) cursor_position::pos_x = std::max(0, cursor_position::pos_x-1);
          if(is_pressed_right_arrow()) cursor_position::pos_x = std::min(int(input->length()), cursor_position::pos_x+1);
        }
        if (choosen->second){
          if(is_pressed_left_arrow()) cursor_position::pos_y = std::max(0, cursor_position::pos_y-1);
          if(is_pressed_right_arrow()) cursor_position::pos_y = std::min(int(input->length()), cursor_position::pos_y+1);
        }
        is_inputing = false;
      }

      std::pair<bool, bool> last_choosen = *choosen;
      *choosen = chose_input_box(choosen, lines_x, lines_y);
      if (last_choosen != *choosen) {
        is_inputing = false;
      }
    }
  }
  SDL_StopTextInput();
}

bool get_input(std::pair<bool, bool> *choosen, std::string *input_x,
               std::string *input_y, bool *is_generating, int *lines_x,
               int *lines_y) {
  bool is_changed = false;
  *choosen = chose_input_box(choosen, lines_x, lines_y);
  if (choosen->first == true) {
    *is_generating = false;
    is_changed = true;
    input_logic(input_x, input_y, choosen, lines_x, lines_y);
  } else if (choosen->second == true) {
    *is_generating = false;
    is_changed = true;
    input_logic(input_y, input_x, choosen, lines_x, lines_y);
  }
  if (is_pressed_enter()) {
    parse_input(input_x, input_y);
    clear_curves();
    *is_generating = false;
    is_changed = true;
  }
  return is_changed;
}
