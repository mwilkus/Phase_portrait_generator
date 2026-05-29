#include "config.h"
#include "control.h"
#include "input_eq.h"
#include "cordinats.h"
#include "numeric.h"
#include <SDL.h>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <ios>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

std::vector<int> hsv_to_rgb(double H) {
  double R, G, B;
  double S = 1, V = 1;
  int h = int(H / 60) % 6;
  double f = H / 60 - int(H / 60);
  double p = V * (1 - S), q = V * (1 - S * f), t = V * (1 - S * (1 - f));
  switch (h) {
  case 0:
    R = V, G = t, B = p;
    break;
  case 1:
    R = q, G = V, B = p;
    break;
  case 2:
    R = p, G = V, B = t;
    break;
  case 3:
    R = p, G = q, B = V;
    break;
  case 4:
    R = t, G = p, B = V;
    break;
  case 5:
    R = V, G = p, B = q;
    break;
  }
  std::vector<int> rgb = {int(R * 255), int(G * 255), int(B * 255)};
  return rgb;
}

std::string generate_numbers(std::vector<double> cords, double scale,
                             bool is_pair) {
  int order = std::ceil(std::log10(WINDOW_W / (2 * scale)));
  if ((WINDOW_W / (2 * scale)) / pow(10, order - 1) < SCALE_MIN_LINE_AMMOUNT) {
    order--;
  }
  if (is_pair) {
    std::stringstream stream_x, stream_y;
    int prec = std::max(2, -(order - 2));
    double x = cords[0];
    double y = cords[1];
    if ((x < 1e4 && x > 1e-4) || (x > -1e4 && x < -1e-4) || x == 0)
      stream_x << std::fixed << std::setprecision(prec) << x;
    else
      stream_x << std::scientific << std::setprecision(2) << x;

    if ((y < 1e4 && y > 1e-4) || (y > -1e4 && y < -1e-4) || y == 0)
      stream_y << std::fixed << std::setprecision(prec) << y;
    else
      stream_y << std::scientific << std::setprecision(2) << y;
    std::string cords_str = stream_x.str();
    cords_str += ", ";
    cords_str += stream_y.str();
    return cords_str;
  } else {
    std::stringstream stream;
    double num = cords[0];
    if ((num < 1e4 && num > 1e-4) || (num > -1e4 && num < -1e-4) || num == 0) {
      int prec = std::max(0, -(order - 1));
      if (num==0) prec =0;
      stream << std::fixed << std::setprecision(prec) << num;
    } else {
      stream << std::scientific << std::setprecision(1) << num;
    }
    std::string cords_str = stream.str();
    return cords_str;
  }
}

SDL_Texture *text_to_texture(SDL_Renderer *renderer, TTF_Font *font,
                             const char *text, SDL_Color color={250,250,250,250}) {
  SDL_Surface *text_surface = TTF_RenderText_Blended_Wrapped(
      font, text, color, INPUT_WIDTH / 2 - INPUT_BOARDER );
  SDL_Texture *text_texture =
      SDL_CreateTextureFromSurface(renderer, text_surface);
  SDL_FreeSurface(text_surface);
  return text_texture;
}

void render_input_box(SDL_Renderer *renderer, TTF_Font *font, int lines_x = 1,
                      int lines_y = 1) {
  SDL_Rect input_box_x = {INPUT_DX_X_OFFSET, INPUT_DX_Y_OFFSET, INPUT_WIDTH,
                          INPUT_HEIGHT * lines_x};
  SDL_Rect input_box_y = {INPUT_DY_X_OFFSET, INPUT_DY_Y_OFFSET, INPUT_WIDTH,
                          INPUT_HEIGHT * lines_y};
  SDL_SetRenderDrawColor(renderer, 150, 150, 150, 180);
  SDL_RenderFillRect(renderer, &input_box_x);
  SDL_RenderFillRect(renderer, &input_box_y);
}

void fill_gap_between_points(SDL_Renderer *renderer,
                             std::pair<double, double> *last_point,
                             std::pair<double, double> point, double scale) {
  double delta_x_axis = point.first - last_point->first;
  double delta_y_axis = point.second - last_point->second;
  double delta =
      std::sqrt(std::pow(delta_x_axis, 2) + std::pow(delta_y_axis, 2));

  if (delta > MAX_POINT_DISTANCE * (WINDOW_W / scale)) {
    std::pair<double, double> start_point = *last_point;
    int how_many_times = (delta / (MAX_POINT_DISTANCE * (WINDOW_W / scale)));
    double x_axis_diff = delta_x_axis / how_many_times;
    double y_axis_diff = delta_y_axis / how_many_times;
    for (int i = 0; i < how_many_times; i++) {
      start_point.first += x_axis_diff;
      start_point.second += y_axis_diff;
      std::pair<double, double> pos =
          cords_to_pos(start_point.first, start_point.second, scale);
      SDL_Rect point_rect = {int(pos.first) - POINT_WIDTH / 2,
                             int(pos.second) - POINT_HEIGHT / 2, POINT_WIDTH,
                             POINT_HEIGHT};
      if (is_in_box(pos.first, pos.second)) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &point_rect);
      }
    }
  }
  *last_point = point;
}

void render_curves(SDL_Renderer *renderer, double scale) {
  std::pair<double, double> last_point;
  for (auto& el : simulation::curves){
    std::vector<std::pair<double, double>>* curve = &el.second;
    if (!curve->empty()) {
      last_point = *curve->begin();
      for (auto point : *curve) {
        std::pair<int, int> pos;
        pos = cords_to_pos(point.first, point.second, scale);
        SDL_Rect point_rect = {int(pos.first) - POINT_WIDTH / 2,
                              int(pos.second) - POINT_HEIGHT / 2, POINT_WIDTH,
                              POINT_HEIGHT};
        if (is_in_box(pos.first, pos.second)) {
          SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
          SDL_RenderFillRect(renderer, &point_rect);
          fill_gap_between_points(renderer, &last_point, point, scale);
        }
      }
    }
  }
}

void render_info(SDL_Renderer *renderer, double scale,
                          TTF_Font *font) {
  int mouse_x, mouse_y;

  Uint32 mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);
  SDL_Rect cords_info = {mouse_x + CORDS_INFO_X_OFFSET,
                         mouse_y + CORDS_INFO_Y_OFFSET, CORDS_INFO_BOX_WIDTH,
                         CORDS_INFO_BOX_HEIGHT};

  std::pair<double, double> cords = pos_to_cords(mouse_x, mouse_y, scale);
  double x = cords.first;
  double y = cords.second;

  if (is_in_box(mouse_x, mouse_y)) {
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 100);
    SDL_RenderFillRect(renderer, &cords_info);
    std::vector<double> cords_vact = {x, y};
    std::string text_str;
    const char *text_c;
    if (!is_pressed_right(NULL, NULL)){
      text_str = generate_numbers(cords_vact, scale, true);
      text_c = text_str.c_str();
    }
    else{
      double dx = x_equation(x,y);
      double dy = y_equation(x,y);
      double magnitudo = {std::sqrt(dx*dx + dy*dy)};
      std::stringstream stream;
      if (magnitudo>1e3 || magnitudo<2e-2) stream << std::scientific << std::setprecision(3) << magnitudo;
      else stream << std::fixed << std::setprecision(3) << magnitudo;
      std::string text_str = stream.str();
      text_c = text_str.c_str();
    }
    SDL_Texture *text_texture = text_to_texture(renderer, font, text_c);
    SDL_RenderCopy(renderer, text_texture, NULL, &cords_info);
    SDL_DestroyTexture(text_texture);
  }
}

void render_arrows(SDL_Renderer *renderer, SDL_Texture *arrow, double scale) {
  int arrow_offset_w = WINDOW_W / (AMMOUNT_OF_ARROWS - 2);
  int arrow_offset_h = WINDOW_H / (AMMOUNT_OF_ARROWS - 2);
  int x_index = 0, y_index = 0;
  bool x_axis_made = false;
  std::vector<std::vector<int>> arrow_angles = simulation::arrow_angles;
  std::vector<std::vector<double>> arrow_colors = simulation::arrow_colors;
  for (int j = Y_WINDOW_OFFSET + Y_ARROW_RENDER_OFFSET;
       j <= WINDOW_H + Y_WINDOW_OFFSET; j += arrow_offset_h) {
    bool y_axis_made = false;
    for (int i = X_WINDOW_OFFSET + X_ARROW_RENDER_OFFSET;
         i <= WINDOW_W + X_WINDOW_OFFSET; i += arrow_offset_w) {
      if (x_index >= BOARDER && x_index < AMMOUNT_OF_ARROWS - BOARDER &&
          y_index >= BOARDER && y_index < AMMOUNT_OF_ARROWS - BOARDER) {
        SDL_Rect arrow_pos = {i, j, ARROW_WIDTH, ARROW_HEIGHT};
        SDL_Point rot_axis = {0, ARROW_HEIGHT / 2};
        int angle = arrow_angles[y_index][x_index];
        int color = arrow_colors[y_index][x_index];
        std::vector<int> rgb = hsv_to_rgb(color);
        SDL_SetTextureColorMod(arrow, rgb[0], rgb[1], rgb[2]);
        SDL_SetTextureAlphaMod(arrow, ARROW_ALFA);
        SDL_RenderCopyEx(renderer, arrow, NULL, &arrow_pos, angle, &rot_axis,
                         SDL_FLIP_NONE);
      }
      x_index++;
      if (i == X_WINDOW_OFFSET + X_ARROW_RENDER_OFFSET +
                   arrow_offset_w * (AMMOUNT_OF_ARROWS - 2) / 2 &&
          !y_axis_made) {
        y_axis_made = true;
        i -= arrow_offset_w;
      }
    }
    if (j == Y_WINDOW_OFFSET + Y_ARROW_RENDER_OFFSET +
                 arrow_offset_h * (AMMOUNT_OF_ARROWS - 2) / 2 &&
        !x_axis_made) {
      x_axis_made = true;
      j -= arrow_offset_h;
    }
    x_index = 0;
    y_index++;
  }
}

void render_scale_text(SDL_Renderer *renderer, SDL_Rect line, TTF_Font *font,
                       double number, double scale, bool vertical,
                       bool position_up, int order) {
  std::vector<double> num_vect = {number};
  if (number != 0 || !vertical) {
    SDL_Rect text_pos = line;
    std::string cords_str = generate_numbers(num_vect, scale, false);
    const char *text = cords_str.c_str();
    SDL_Texture *text_texture = text_to_texture(renderer, font, text);
    SDL_QueryTexture(text_texture, NULL, NULL, &text_pos.w, &text_pos.h);
    if (vertical) {
      text_pos.x += SCALE_TEXT_OFFSET_Y_AXIS;
    } else {
      text_pos.x -= text_pos.w / 3;
      if (position_up)
        text_pos.y += SCALE_TEXT_OFFSET_X_AXIS_UP;
      else
        text_pos.y += SCALE_TEXT_OFFSET_X_AXIS_DOWN;
    }
    text_pos.w *= FONT_SCALE;
    text_pos.h *= FONT_SCALE;
    SDL_SetTextureColorMod(text_texture, 200, 200, 200);
    SDL_RenderCopy(renderer, text_texture, NULL, &text_pos);
    SDL_DestroyTexture(text_texture);
  }
}

void render_scale(SDL_Renderer *renderer, SDL_Texture *line, double scale,
                  TTF_Font *font) {
  int order = std::ceil(std::log10(WINDOW_W / (2 * scale)));
  if ((WINDOW_W / (2 * scale)) / pow(10, order - 1) < SCALE_MIN_LINE_AMMOUNT) {
    order--;
  }
  double diff = pow(10, order - 1);
  bool position_up = false;
  for (double i = 0; i <= WINDOW_W / (2 * scale); i += diff) {
    SDL_Rect line_r = {int(X_WINDOW_OFFSET + WINDOW_W / 2 +
                           i / (WINDOW_W / (scale)) * WINDOW_W) +
                           SCALE_LINE_X_X_OFFSET,
                       Y_WINDOW_OFFSET + WINDOW_H / 2 + SCALE_LINE_X_Y_OFFSET,
                       SCALE_LINE_X_WIDTH, SCALE_LINE_X_HEIGHT};
    if (line_r.x <= X_WINDOW_OFFSET + WINDOW_W - SCALE_BOARDER) {
      SDL_SetTextureColorMod(line, 255,255, 255);
      SDL_RenderCopy(renderer, line, NULL, &line_r);
      render_scale_text(renderer, line_r, font, i, scale, false, position_up,
                        order);
    }
    position_up = !position_up;
  }
  position_up = false;
  for (double i = 0; i >= -WINDOW_W / (2 * scale); i -= diff) {
    SDL_Rect line_r = {int(X_WINDOW_OFFSET + WINDOW_W / 2 +
                           i / (WINDOW_W / (scale)) * WINDOW_W) +
                           SCALE_LINE_X_X_OFFSET,
                       Y_WINDOW_OFFSET + WINDOW_H / 2 + SCALE_LINE_X_Y_OFFSET,
                       SCALE_LINE_X_WIDTH, SCALE_LINE_X_HEIGHT};
    if (line_r.x >= X_WINDOW_OFFSET + SCALE_BOARDER) {
      SDL_SetTextureColorMod(line, 255,255, 255);
      SDL_RenderCopy(renderer, line, NULL, &line_r);
      render_scale_text(renderer, line_r, font, i, scale, false, position_up,
                        order);
    }
    position_up = !position_up;
  }
  position_up = false;
  for (double i = 0; i <= WINDOW_H / (2 * scale); i += diff) {
    SDL_Rect line_r = {X_WINDOW_OFFSET + WINDOW_W / 2 + SCALE_LINE_Y_X_OFFSET,
                       int(Y_WINDOW_OFFSET + WINDOW_H / 2 +
                           i / (WINDOW_H / (scale)) * WINDOW_H) +
                           SCALE_LINE_Y_Y_OFFSET,
                       SCALE_LINE_Y_WIDTH, SCALE_LINE_Y_HEIGHT};
    if (line_r.y <= Y_WINDOW_OFFSET + WINDOW_H - SCALE_BOARDER) {
      SDL_SetTextureColorMod(line, 255,255, 255);
      SDL_RenderCopy(renderer, line, NULL, &line_r);
      render_scale_text(renderer, line_r, font, -i, scale, true, position_up,
                        order);
    }
  }
  for (double i = 0; i >= -WINDOW_H / (2 * scale); i -= diff) {
    SDL_Rect line_r = {X_WINDOW_OFFSET + WINDOW_W / 2 + SCALE_LINE_Y_X_OFFSET,
                       int(Y_WINDOW_OFFSET + WINDOW_H / 2 +
                           i / (WINDOW_H / (scale)) * WINDOW_H) +
                           SCALE_LINE_Y_Y_OFFSET,
                       SCALE_LINE_Y_WIDTH, SCALE_LINE_Y_HEIGHT};
    if (line_r.y >= Y_WINDOW_OFFSET + SCALE_BOARDER) {
      SDL_SetTextureColorMod(line, 255,255, 255);
      SDL_RenderCopy(renderer, line, NULL, &line_r);
      render_scale_text(renderer, line_r, font, -i, scale, true, position_up,
                        order);
    }
  }
}

void render_cursor(SDL_Renderer *renderer, TTF_Font *font, SDL_Texture * line, std::string *equation_x, std::string *equation_y, int *lines_x, int *lines_y){
  std::string before_cursor;
  int w,h,base_h,dummy;
  SDL_Rect cursor;
  if (cursor_position::choosen.first) before_cursor = equation_x->substr(0,6+cursor_position::pos_x);  
  else if (cursor_position::choosen.second) before_cursor = equation_y->substr(0,6+cursor_position::pos_y);
  TTF_SizeUTF8(font, before_cursor.c_str(), &w, &h);
  std::string current_line = "";
  int count_of_lines = 0;
  for (auto c : before_cursor){
    std::string test_line = current_line + c;
    int test_w;
    TTF_SizeUTF8(font, test_line.c_str(), &test_w, &dummy);
    if (test_w>(INPUT_WIDTH / 2 - INPUT_BOARDER)){
      current_line=c;
      count_of_lines++;
    }
    else current_line=test_line;
  }
  TTF_SizeUTF8(font, current_line.c_str(), &w, &dummy);
  h*=count_of_lines;
  h*=INPUT_FONT_SCALE;
  w*=INPUT_FONT_SCALE;
  if (cursor_position::choosen.first) cursor = {INPUT_DX_X_OFFSET+CURSOR_X_OFFSET+w, INPUT_DX_Y_OFFSET+h+CURSOR_Y_OFFSET, CURSOR_WIDTH, CURSOR_HEIGHT};
  else if (cursor_position::choosen.second) cursor = {INPUT_DY_X_OFFSET+CURSOR_X_OFFSET+w, INPUT_DY_Y_OFFSET+h+CURSOR_Y_OFFSET, CURSOR_WIDTH, CURSOR_HEIGHT};
  SDL_SetTextureColorMod(line, 0,0, 0);
  SDL_RenderCopy(renderer, line, NULL, &cursor);
}

void render_equation(SDL_Renderer *renderer, TTF_Font *font,
                     std::string *equation_x, std::string *equation_y,
                     int *lines_x, int *lines_y, SDL_Texture* line) {
  std::string eq_x_str = "dx/dt=" + *equation_x;
  std::string eq_y_str = "dy/dt=" + *equation_y;
  const char *equation_x_c = eq_x_str.c_str();
  const char *equation_y_c = eq_y_str.c_str();
  SDL_Color black = {0,0,0,255};
  SDL_Texture *text_x_texture = text_to_texture(renderer, font, equation_x_c, black);
  SDL_Texture *text_y_texture = text_to_texture(renderer, font, equation_y_c, black);
  SDL_Rect text_pos_x = {INPUT_DX_X_OFFSET, INPUT_DX_Y_OFFSET, 0, 0};
  SDL_Rect text_pos_y = {INPUT_DY_X_OFFSET, INPUT_DY_Y_OFFSET, 0, 0};
  SDL_QueryTexture(text_x_texture, NULL, NULL, &text_pos_x.w, &text_pos_x.h);
  SDL_QueryTexture(text_y_texture, NULL, NULL, &text_pos_y.w, &text_pos_y.h);
  text_pos_x.w *= INPUT_FONT_SCALE;
  text_pos_x.h *= INPUT_FONT_SCALE;
  text_pos_y.w *= INPUT_FONT_SCALE;
  text_pos_y.h *= INPUT_FONT_SCALE;
  int w_x, h_x, w_y, h_y;
  TTF_SizeUTF8(font, equation_x_c, &w_x, &h_x);
  TTF_SizeUTF8(font, equation_y_c, &w_y, &h_y);
  *lines_x = w_x * 2.2 / INPUT_WIDTH + 1;
  *lines_y = w_y * 2.2 / INPUT_WIDTH + 1;
  render_input_box(renderer, font, *lines_x, *lines_y);
  SDL_RenderCopy(renderer, text_x_texture, NULL, &text_pos_x);
  SDL_RenderCopy(renderer, text_y_texture, NULL, &text_pos_y);
  SDL_DestroyTexture(text_x_texture);
  SDL_DestroyTexture(text_y_texture);
  render_cursor(renderer, font, line, &eq_x_str, &eq_y_str, lines_x, lines_y);
}

void render_color_bar(SDL_Renderer *renderer) {
  SDL_Rect color_bar_frame = {COLOR_BAR_FRAME_X_OFFSET,
                              COLOR_BAR_FRAME_Y_OFFSET, COLOR_BAR_FRAME_WIDTH,
                              COLOR_BAR_FRAME_HEIGHT};
  SDL_SetRenderDrawColor(renderer, 170, 170, 170, 50);
  SDL_RenderFillRect(renderer, &color_bar_frame);
  for (int i = 0; i < COLOR_BAR_HEIGHT; i++) {
    double color_value = (double)i / COLOR_BAR_HEIGHT * 250;
    std::vector<int> rgb = hsv_to_rgb(color_value);
    SDL_SetRenderDrawColor(renderer, rgb[0], rgb[1], rgb[2], ARROW_ALFA);
    SDL_RenderDrawLine(renderer, COLOR_BAR_X_OFFSET, COLOR_BAR_Y_OFFSET + i,
                       COLOR_BAR_X_OFFSET + COLOR_BAR_WIDTH,
                       COLOR_BAR_Y_OFFSET + i);
  }
}

void render_color_box_scale(SDL_Renderer *renderer, SDL_Texture *line, TTF_Font *font) {
  int index = 0;
  for (int i = COLOR_BAR_Y_OFFSET + COLOR_BAR_MAX_VALUE_Y_OFFSET;
       i <= COLOR_BAR_Y_OFFSET + COLOR_BAR_HEIGHT + COLOR_BAR_MIN_VALUE_Y_OFFSET;
       i += (COLOR_BAR_HEIGHT+2*COLOR_BAR_MIN_VALUE_Y_OFFSET) / (AMMOUNT_OF_LINES-1)) {
    SDL_Rect line_1 = {COLOR_BAR_X_OFFSET+COLOR_BAR_SCALE_X_OFFSET, i, COLOR_BAR_SCALE_WIDTH,COLOR_BAR_SCALE_HEIGHT};
    SDL_Rect line_2 = line_1;
    line_2.x+=COLOR_BAR_SCALE_SPACE+COLOR_BAR_SCALE_WIDTH;
    SDL_Rect text_rect = line_1;
    text_rect.y-=COLOR_BAR_SCALE_TEXT_HEIGHT;
    text_rect.w = 2*COLOR_BAR_SCALE_WIDTH+COLOR_BAR_SCALE_SPACE;
    text_rect.h = COLOR_BAR_SCALE_TEXT_HEIGHT;
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2);
    float value;
    switch (index) {
    case 0:
      value = values::max;
      break;
    case 1:
      value = (values::mean + values::max) / 2;
      break;
    case 2:
      value = values::mean;
      break;
    case 3:
      value = (values::mean + values::min) / 2;
      break;
    case 4:
      value = values::min;
      break;
    }
    if (value < 1e3 && value > 1e-1) {
      if (index!=0) stream << std::fixed << std::setprecision(2) << value;
      else stream << std::fixed << std::setprecision(2) << "> " << value;
    } else {
      if (index!=0) stream << std::scientific << std::setprecision(2) << value;
      else stream << std::scientific << std::setprecision(2) << "> " << value;
    }
    std::string text_str = stream.str();
    const char* text = text_str.c_str();
    SDL_Color black = {0,0,0,255};
    SDL_Texture* text_texture = text_to_texture(renderer, font, text, black);
    SDL_SetTextureColorMod(line, 0,0, 0);
    SDL_RenderCopy(renderer, line, NULL, &line_1);
    SDL_RenderCopy(renderer, line, NULL, &line_2);
    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
    index++;
  }
}

void render_tittle(SDL_Renderer *renderer, SDL_Texture * title_texture) {
  SDL_Rect tittle_rect = {TITLE_X_OFFSET, TITLE_Y_OFFSET,
                           TITLE_WIDTH, TITLE_HEIGHT};
  SDL_RenderCopy(renderer, title_texture, NULL, &tittle_rect);
}

void render_background(SDL_Renderer *renderer, SDL_Rect phase_poitrat, SDL_Texture* backround){
  SDL_Rect back = {0,0, WIDTH,HAIGHT};
  SDL_RenderCopy(renderer, backround, NULL, &back);
  SDL_SetRenderDrawColor(renderer, 15, 35, 50, 150);
  SDL_RenderFillRect(renderer, &phase_poitrat);
  SDL_RenderFillRect(renderer, &phase_poitrat);
}
