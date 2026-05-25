#ifndef __NUMERIC
#define __NUMERIC

#include <vector>
#include "parsing.h"


void calc_curve(double scale, bool *is_generating,
                std::vector<std::pair<double, double>> *curve);

std::vector<std::vector<int>> calculate_arrow_angle(double range);
std::vector<std::vector<double>> calculate_arrow_color(double range);

struct values{
    inline static double min;
    inline static double max;
    inline static double mean;
};

#endif