#ifndef __NUMERIC
#define __NUMERIC

#include <vector>
#include "parsing.h"


void calc_curve(float scale, bool *is_generating,
                std::vector<std::pair<float, float>> *curve);

std::vector<std::vector<int>> calculate_arrow_angle(float range);
std::vector<std::vector<float>> calculate_arrow_color(float range);

#endif