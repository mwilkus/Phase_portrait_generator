#ifndef __INPUT
#define __INPUT
#include "parsing.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <sys/types.h>
#include <utility>

using eq = std::pair<exprtk::expression<double>, exprtk::expression<double>>;

bool get_input(std::pair<bool, bool> *choosen, std::string *input_x,
               std::string *input_y, bool *is_generating, int *lines_x,
               int *lines_y, std::vector<std::pair<double, double>> *curve);

struct cursor_position{
    inline static std::pair<bool, bool> choosen = {false, false};
    inline static int pos_x = 1;
    inline static int pos_y = 1;
};

#endif