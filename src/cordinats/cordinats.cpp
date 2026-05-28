#include "config.h"
#include <utility>

bool is_in_box(int x, int y) {
  if (x >= X_WINDOW_OFFSET + CORDS_INFO_BOARDER &&
      x <= X_WINDOW_OFFSET + WINDOW_W - CORDS_INFO_BOARDER &&
      y >= Y_WINDOW_OFFSET + CORDS_INFO_BOARDER &&
      y <= Y_WINDOW_OFFSET + WINDOW_H - CORDS_INFO_BOARDER) {
    return true;
  } else {
    return false;
  }
}

bool is_in_input_box_dx(int x, int y, int lines) {
  if (x > INPUT_DX_X_OFFSET && x < INPUT_DX_X_OFFSET + INPUT_WIDTH &&
      y > INPUT_DX_Y_OFFSET && y < INPUT_DX_Y_OFFSET + INPUT_HEIGHT * lines) {
    return true;
  }
  return false;
}

bool is_in_input_box_dy(int x, int y, int lines) {
  if (x > INPUT_DY_X_OFFSET && x < INPUT_DY_X_OFFSET + INPUT_WIDTH &&
      y > INPUT_DY_Y_OFFSET && y < INPUT_DY_Y_OFFSET + INPUT_HEIGHT * lines) {
    return true;
  }
  return false;
}

std::pair<double, double> pos_to_cords(int x, int y, double scale) {
  std::pair<double, double> cords;
  cords.first =
      (x - (X_WINDOW_OFFSET + WINDOW_W / 2 + SCALE_LINE_X_X_OFFSET)) / scale;
  cords.second =
      (Y_WINDOW_OFFSET + WINDOW_H / 2 + SCALE_LINE_Y_Y_OFFSET - y) / scale;
  return cords;
}

std::pair<int, int> cords_to_pos(double x, double y, double scale) {
  std::pair<int, int> position;
  position.first =
      x * scale + X_WINDOW_OFFSET + WINDOW_W / 2 + SCALE_LINE_X_X_OFFSET;
  position.second =
      Y_WINDOW_OFFSET + WINDOW_H / 2 + SCALE_LINE_Y_Y_OFFSET - y * scale;
  return position;
}