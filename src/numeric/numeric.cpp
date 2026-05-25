#include "include/numeric.h"
#include "config.h"
#include "cordinats.h"
#include "control.h"
#include <climits>
#include <cmath>
#include <utility>
#include <vector>
#include "exprtk.hpp"
#include "parsing.h"

float x_equation(float x, float y) {
  equations::x = x;
  equations::y = y;
  return equations::x_expr.value();
 }

float y_equation(float x, float y) {
  equations::x = x;
  equations::y = y;
  return equations::y_expr.value();
}

std::pair<float, float> RK4(float x, float y) {
  float h = TIME_INTERVAL;
  float k1 = x_equation(x, y);
  float l1 = y_equation(x, y);
  float k2 = x_equation(x + h * k1 / 2, y + h * l1 / 2);
  float l2 = y_equation(x + h * k1 / 2, y + h * l1 / 2);
  float k3 = x_equation(x + h * k2 / 2, y + h * l2 / 2);
  float l3 = y_equation(x + h * k2 / 2, y + h * l2 / 2);
  float k4 = x_equation(x + h * k3, y + h * l3);
  float l4 = y_equation(x + h * k3, y + h * l3);
  x += h / 6 * (k1 + 2 * k2 + 2 * k3 + k4);
  y += h / 6 * (l1 + 2 * l2 + 2 * l3 + l4);
  return {x, y};
}

void calc_curve(float scale, bool *is_generating,
                std::vector<std::pair<float, float>> *curve) {
  int mouse_x, mouse_y;
  std::pair<float, float> cords;
  if (is_pressed_left(&mouse_x, &mouse_y)) {
    if (is_in_box(mouse_x, mouse_y)) {
      *is_generating = true;
      curve->clear();
      cords = pos_to_cords(mouse_x, mouse_y, scale);
      curve->push_back({cords.first, cords.second});
    }
  } else if (*is_generating) {
    std::pair<float, float> last_cords = curve->back();
    std::pair<float, float> new_cords =
        RK4(last_cords.first, last_cords.second);
    curve->push_back(new_cords);
  }
}

std::vector<std::vector<int>> calculate_arrow_angle(float range) {
  std::vector<std::vector<int>> output;
  float x = -range / 2, y = range / 2;
  float dx,dy;
  float diff = range / (AMMOUNT_OF_ARROWS - 1);
  for (int i = 0; i <= AMMOUNT_OF_ARROWS; i++) {
    std::vector<int> line;
    for (int j = 0; j <= AMMOUNT_OF_ARROWS; j++) {
      dy = y_equation(x, y);
      dx = x_equation(x, y);
      int angle = -(std::atan2(dy, dx) * 180.0 / M_PI);
      line.push_back(angle);
      x += diff;
    }
    y -= diff;
    x = -range / 2;
    output.push_back(line);
  }
  return output;
}

std::vector<std::vector<float>> calculate_arrow_color(float range) {
  std::vector<std::vector<float>> output;
  std::vector<float> magnitudes;
  float x = -range / 2, y = range / 2;
  float diff = range / (AMMOUNT_OF_ARROWS - 1);
  for (int i = 0; i <= AMMOUNT_OF_ARROWS; i++) {
    std::vector<float> line;
    for (int j = 0; j <= AMMOUNT_OF_ARROWS; j++) {
      if (std::abs(x)<range/1000) x = -range/1000;
      if (std::abs(y)<range/1000) y = -range/1000;
      float dy = y_equation(x, y);
      float dx = x_equation(x, y);
      float magnitude = std::sqrt(dx * dx + dy * dy);
      line.push_back(magnitude);
      magnitudes.push_back(magnitude);
      x += diff;
    }
    y -= diff;
    x = -range / 2;
    output.push_back(line);
  }
  std::sort(magnitudes.begin(), magnitudes.end());
  float mediana = magnitudes[magnitudes.size() / 2];
  float min_magnitude = magnitudes[0];
  for(auto &line : output) {
    for(auto &magnitude : line) {
      if (magnitude <= mediana){
        magnitude = 250 - std::max(0,int(125 * (magnitude - min_magnitude) / (mediana - min_magnitude)));
      }
      else {
        magnitude = 250 - std::min(250, int(125 + 125 * (magnitude - mediana) / (mediana - min_magnitude)));
      }
    }
  }
  return output;
}