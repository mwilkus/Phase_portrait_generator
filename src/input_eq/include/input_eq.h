#ifndef __INPUT
#define __INPUT
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <utility>

void get_input(std::pair<bool, bool>* choosen, std::string* input_x, std::string* input_y, bool* is_generating ,int* lines_x, int* lines_y);

#endif