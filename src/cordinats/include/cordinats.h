#ifndef __CORDINATS
#define __CORDINATS
#include <utility>

bool is_in_box(int x, int y);
std::pair<float, float> pos_to_cords(int x, int y, float scale);
std::pair<int, int> cords_to_pos(float x, float y, float scale);

#endif