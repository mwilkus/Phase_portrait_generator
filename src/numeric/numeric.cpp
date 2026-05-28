#include "include/numeric.h"
#include "config.h"
#include "control.h"
#include "cordinats.h"
#include "exprtk.hpp"
#include "numeric.h"
#include "parsing.h"
#include <climits>
#include <cmath>
#include <utility>
#include <vector>

double x_equation(double x, double y) {
  equations::x = x;
  equations::y = y;
  return equations::x_expr.value();
}

double y_equation(double x, double y) {
  equations::x = x;
  equations::y = y;
  return equations::y_expr.value();
}

std::pair<double, double> RK4(double x, double y) {
  double h = TIME_INTERVAL;
  double k1 = x_equation(x, y);
  double l1 = y_equation(x, y);
  double k2 = x_equation(x + h * k1 / 2, y + h * l1 / 2);
  double l2 = y_equation(x + h * k1 / 2, y + h * l1 / 2);
  double k3 = x_equation(x + h * k2 / 2, y + h * l2 / 2);
  double l3 = y_equation(x + h * k2 / 2, y + h * l2 / 2);
  double k4 = x_equation(x + h * k3, y + h * l3);
  double l4 = y_equation(x + h * k3, y + h * l3);
  x += h / 6 * (k1 + 2 * k2 + 2 * k3 + k4);
  y += h / 6 * (l1 + 2 * l2 + 2 * l3 + l4);
  return {x, y};
}

void calc_curve(double scale, bool *is_generating,
                std::vector<std::pair<double, double>> *curve) {
  int mouse_x, mouse_y;
  std::pair<double, double> cords;
  if (is_pressed_left(&mouse_x, &mouse_y)) {
    if (is_in_box(mouse_x, mouse_y)) {
      *is_generating = true;
      curve->clear();
      cords = pos_to_cords(mouse_x, mouse_y, scale);
      curve->push_back({cords.first, cords.second});
    }
  } else if (*is_generating) {
    std::pair<double, double> last_cords = curve->back();
    std::pair<double, double> new_cords =
        RK4(last_cords.first, last_cords.second);
    curve->push_back(new_cords);
  }
}

void calculate_arrow_angle(double range) {
  std::vector<std::vector<int>> output;
  double x = -range / 2, y = range / 2;
  double dx, dy;
  double diff = range / (AMMOUNT_OF_ARROWS - 2);
  bool x_axis_made = false;
  for (int i = 0; i <= AMMOUNT_OF_ARROWS; i++) {
    std::vector<int> line;
    bool y_axis_made = false;
    if (std::abs(y) < range / 1000 && !x_axis_made)
      y = range / 2000;
    for (int j = 0; j <= AMMOUNT_OF_ARROWS; j++) {
      if (std::abs(x) < range / 1000 && !y_axis_made)
        x = -range / 2000;
      dy = y_equation(x, y);
      dx = x_equation(x, y);
      int angle = -(std::atan2(dy, dx) * 180.0 / M_PI);
      line.push_back(angle);
      if (std::abs(x) < range / 1000 && !y_axis_made) {
        x = range / 2000 - diff;
        y_axis_made = true;
      }
      x += diff;
    }
    if (std::abs(y) < range / 1000 && !x_axis_made) {
      y = -range / 2000 + diff;
      x_axis_made = true;
    }
    y -= diff;
    x = -range / 2;
    output.push_back(line);
  }
  simulation::arrow_angles = output;
}

void calculate_arrow_color(double range) {
  std::vector<std::vector<double>> output;
  std::vector<double> magnitudes;
  double x = -range / 2, y = range / 2;
  double diff = range / (AMMOUNT_OF_ARROWS - 2);
  bool x_axis_made = false;
  for (int i = 0; i <= AMMOUNT_OF_ARROWS; i++) {
    std::vector<double> line;
    bool y_axis_made = false;
    if (std::abs(y) < range / 1000 && !x_axis_made)
      y = range / 2000;
    for (int j = 0; j <= AMMOUNT_OF_ARROWS; j++) {
      if (std::abs(x) < range / 1000 && !y_axis_made)
        x = -range / 2000;
      double dy = y_equation(x, y);
      double dx = x_equation(x, y);
      double magnitude = std::sqrt(dx * dx + dy * dy);
      line.push_back(magnitude);
      magnitudes.push_back(magnitude);
      if (std::abs(x) < range / 1000 && !y_axis_made) {
        x = range / 2000 - diff;
        y_axis_made = true;
      }
      x += diff;
    }
    if (std::abs(y) < range / 1000 && !x_axis_made) {
      y = -range / 2000 + diff;
      x_axis_made = true;
    }
    y -= diff;
    x = -range / 2;
    output.push_back(line);
  }
  std::sort(magnitudes.begin(), magnitudes.end());
  values::mean = magnitudes[magnitudes.size() / 2];
  values::min = magnitudes[0];
  values::max = 2 * values::mean - values::min;
  for (auto &line : output) {
    for (auto &magnitude : line) {
      if (magnitude <= values::mean) {
        magnitude = 250 - std::max(0, int(125 * (magnitude - values::min) /
                                          (values::mean - values::min)));
      } else {
        magnitude =
            250 - std::min(250, int(125 + 125 * (magnitude - values::mean) /
                                              (values::mean - values::min)));
      }
    }
  }
  simulation::arrow_colors = output;
}