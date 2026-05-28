#ifndef __NUMERIC
#define __NUMERIC

#include "parsing.h"
#include <vector>

void calc_curve(double scale, bool *is_generating,
                std::vector<std::pair<double, double>> *curve);

void calculate_arrow_angle(double range);
void calculate_arrow_color(double range);

struct values {
  inline static double min;
  inline static double max;
  inline static double mean;
};

struct simulation {
  inline static std::vector<std::vector<int>> arrow_angles;
  inline static std::vector<std::vector<double>> arrow_colors;
};

#endif