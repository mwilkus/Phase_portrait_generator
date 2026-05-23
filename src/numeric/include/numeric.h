#ifndef __NUMERIC
#define __NUMERIC

#include <vector>
#include "parsing.h"


void calc_curve(float scale, bool *is_generating,
                std::vector<std::pair<float, float>> *curve);

std::vector<std::vector<int>> calculate_arrow_angle(float range);

#endif