#ifndef __CORDINATS
#define __CORDINATS
#include <utility>

bool is_in_box(int x, int y);
bool is_in_input_box_dx(int x, int y, int lines);
bool is_in_input_box_dy(int x, int y, int lines);
std::pair<double, double> pos_to_cords(int x, int y, double scale);
std::pair<int, int> cords_to_pos(double x, double y, double scale);

#endif