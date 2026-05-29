#ifndef __NUMERIC
#define __NUMERIC

#include "parsing.h"
#include <vector>
#include <map>

void clear_curves();

void calc_curves(double scale, bool *is_generating);

void calculate_arrow_angle(double range);
void calculate_arrow_color(double range);

struct values {
  inline static double min;
  inline static double max;
  inline static double mean;
};

struct simulation {
  inline static bool stil_pressed = false;
  inline static std::vector<std::vector<int>> arrow_angles;
  inline static std::vector<std::vector<double>> arrow_colors;
  inline static std::map<std::pair<double, double>,std::vector<std::pair<double, double>>> curves;
};

#endif