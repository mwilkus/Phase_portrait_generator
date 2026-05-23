#ifndef __PARSING
#define __PARSING

#include <string>
#include "exprtk.hpp"
#include <utility>

void parse_input(std::string* input_x, std::string* input_y);

struct equations {
    inline static float x;
    inline static float y;
    inline static exprtk::expression<float> x_expr;
    inline static exprtk::expression<float> y_expr;
    inline static bool valid = true;
};

#endif