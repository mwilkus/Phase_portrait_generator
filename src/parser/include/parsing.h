#ifndef __PARSING
#define __PARSING

#include "exprtk.hpp"
#include <string>
#include <utility>

void parse_input(std::string *input_x, std::string *input_y);

struct equations {
  inline static double x;
  inline static double y;
  inline static exprtk::expression<double> x_expr;
  inline static exprtk::expression<double> y_expr;
  inline static bool valid = true;
};

#endif