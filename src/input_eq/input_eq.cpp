#include "config.h"
#include "control.h"
#include "cordinats.h"
#include <SDL.h>
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <string>
#include <utility>
#include <vector>
#include "parsing.h"

using eq = std::pair<exprtk::expression<float>, exprtk::expression<float>>;

std::pair<bool, bool> chose_input_box(std::pair<bool, bool>* choosen, int* lines_x, int* lines_y) {
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
  return *choosen;
}

void input_logic(std::string* input, std::string* other, std::pair<bool, bool>* choosen, int* lines_x, int* lines_y) {
    SDL_Event e;
    SDL_StartTextInput();
    bool is_inputing = true;
    while (is_inputing) {
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT) {
                is_inputing = false;
            }
            else if (e.type == SDL_TEXTINPUT) {
                *input += e.text.text;
                is_inputing = false;
            }
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_BACKSPACE) {
                int len = input->length();
                if (len > 0) {
                    input->pop_back();
                }
                is_inputing = false;
            }
            
            std::pair<bool, bool> last_choosen = *choosen;
            *choosen = chose_input_box(choosen,lines_x,lines_y);
            if (last_choosen != *choosen) {
                is_inputing = false;
            }
        }
    }
    SDL_StopTextInput();
}

void get_input(std::pair<bool, bool>* choosen, std::string* input_x, std::string* input_y, bool* is_generating, int* lines_x, int* lines_y, std::vector<std::pair<float, float>>* curve) {
    *choosen = chose_input_box(choosen,lines_x,lines_y);
    if (choosen->first == true) {
        *is_generating = false;
        input_logic(input_x, input_y, choosen, lines_x, lines_y);
    }
    else if (choosen->second == true) {
        *is_generating = false;
        input_logic(input_y, input_x, choosen, lines_x, lines_y);
    }
    if (is_pressed_enter()) {
        parse_input(input_x, input_y);
        curve->clear();
        *is_generating = false;
    }
}

