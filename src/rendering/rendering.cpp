#include <iomanip>
#include <vector>
#include "config.h"
#include "cordinats.h"
#include "control.h"
#include "numeric.h"
#include <cmath>
#include <sstream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

std::string generate_numbers(std::vector<float> cords, float scale, bool is_pair) {
  int order = std::ceil(std::log10(WINDOW_W / (2 * scale)));
  if ((WINDOW_W / (2 * scale)) / pow(10, order - 1) <
      SCALE_MIN_LINE_AMMOUNT) {
    order--;
  }
  if (is_pair) {
    std::stringstream stream_x, stream_y;
    int prec = std::max(2, -(order - 2));
    float x = cords[0];
    float y = cords[1];
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
  } 
  else {
    std::stringstream stream;
    float num = cords[0];
    if ((num < 1e4 && num > 1e-4) || (num > -1e4 && num < -1e-4) ||
        num == 0) {
      int prec = std::max(0, -(order - 1));
      stream << std::fixed << std::setprecision(prec) << num;
    } else {
      stream << std::scientific << std::setprecision(1) << num;
    }
    std::string cords_str = stream.str();
    return cords_str;
  }
}

SDL_Texture* text_to_texture(SDL_Renderer* renderer, TTF_Font* font, const char* text){
  SDL_Color black = {0, 0, 0, 255};
  SDL_Surface *text_surface = TTF_RenderText_Blended(font, text, black);
  SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
  SDL_FreeSurface(text_surface);
  return text_texture;
}

void render_curve(SDL_Renderer *renderer,std::vector<std::pair<float, float>> *curve, float scale) {
  for (auto point : *curve) {
    std::pair<int, int> pos;
    pos = cords_to_pos(point.first, point.second, scale);
    SDL_Rect point_rect = {int(pos.first) - POINT_WIDTH / 2,
                           int(pos.second) - POINT_HEIGHT / 2, POINT_WIDTH,
                           POINT_HEIGHT};
    if (is_in_box(pos.first, pos.second)) {
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
      SDL_RenderFillRect(renderer, &point_rect);
    }
  }
}

void render_codinats_info(SDL_Renderer *renderer, float scale, TTF_Font *font) {
  int mouse_x, mouse_y;

  Uint32 mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);
  SDL_Rect cords_info = {mouse_x + CORDS_INFO_X_OFFSET,
                         mouse_y + CORDS_INFO_Y_OFFSET, CORDS_INFO_BOX_WIDTH,
                         CORDS_INFO_BOX_HEIGHT};

  std::pair<float, float> cords = pos_to_cords(mouse_x, mouse_y, scale);
  float x = cords.first;
  float y = cords.second;

  if (is_in_box(mouse_x, mouse_y)) {
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 100);
    SDL_RenderFillRect(renderer, &cords_info);
    std::vector<float> cords_vact = {x,y};
    std::string cords_str = generate_numbers(cords_vact, scale, true);
    const char *cords_c = cords_str.c_str();
    SDL_Texture* text_texture = text_to_texture(renderer, font, cords_c);
    SDL_RenderCopy(renderer, text_texture, NULL, &cords_info);
    SDL_DestroyTexture(text_texture);
  }
}

void render_arrows(SDL_Renderer *renderer, SDL_Texture *arrow, float scale) {
  int arrow_offset_w = WINDOW_W / (AMMOUNT_OF_ARROWS - 1);
  int arrow_offset_h = WINDOW_H / (AMMOUNT_OF_ARROWS - 1);
  int x_index = 0, y_index = 0;
  std::vector<std::vector<int>> arrow_angles =
      calculate_arrow_angle(WINDOW_W / scale);
  for (int j = Y_WINDOW_OFFSET - 8; j <= WINDOW_H + Y_WINDOW_OFFSET;
       j += arrow_offset_h) {
    for (int i = X_WINDOW_OFFSET; i <= WINDOW_W + X_WINDOW_OFFSET;
         i += arrow_offset_w) {
      if (x_index >= BOARDER && x_index < AMMOUNT_OF_ARROWS - BOARDER &&
          y_index >= BOARDER && y_index < AMMOUNT_OF_ARROWS - BOARDER) {
        SDL_Rect arrow_pos = {i, j, ARROW_WIDTH, ARROW_HEIGHT};
        SDL_Point rot_axis = {0, ARROW_HEIGHT / 2};
        int angle = arrow_angles[y_index][x_index];
        SDL_RenderCopyEx(renderer, arrow, NULL, &arrow_pos, angle, &rot_axis, SDL_FLIP_NONE);
      }
      x_index++;
    }
    x_index = 0;
    y_index++;
  }
}

void render_scale_text(SDL_Renderer *renderer, SDL_Rect line, TTF_Font *font,
                       float number, float scale, bool vertical, bool position_up, int order) {
  std::vector<float> num_vect = {number};
  if (number != 0 || !vertical){
      SDL_Rect text_pos = line;
      std::string cords_str = generate_numbers(num_vect, scale, false);
      const char* text = cords_str.c_str();
      SDL_Texture* text_texture = text_to_texture(renderer, font, text);
      SDL_QueryTexture(text_texture, NULL, NULL, &text_pos.w, &text_pos.h);
    if (vertical){
      text_pos.x += SCALE_TEXT_OFFSET_Y_AXIS;
    }
    else {
      text_pos.x -= text_pos.w / 3;
      if (position_up)
        text_pos.y += SCALE_TEXT_OFFSET_X_AXIS_UP;
      else
        text_pos.y += SCALE_TEXT_OFFSET_X_AXIS_DOWN;
    }
    text_pos.w *= FONT_SCALE;
    text_pos.h *= FONT_SCALE;
    SDL_RenderCopy(renderer, text_texture, NULL, &text_pos);
    SDL_DestroyTexture(text_texture);
  }
}

void render_scale(SDL_Renderer *renderer, SDL_Texture *line, float scale,
                  TTF_Font *font) {
  int order = std::ceil(std::log10(WINDOW_W / (2 * scale)));
  if ((WINDOW_W / (2 * scale)) / pow(10, order - 1) < SCALE_MIN_LINE_AMMOUNT) {
    order--;
  }
  float diff = pow(10, order - 1);
  bool position_up = false;
  for (float i = 0; i <= WINDOW_W / (2 * scale); i += diff) {
    SDL_Rect line_r = {int(X_WINDOW_OFFSET + WINDOW_W / 2 +
                           i / (WINDOW_W / (scale)) * WINDOW_W) +
                           SCALE_LINE_X_X_OFFSET,
                       Y_WINDOW_OFFSET + WINDOW_H / 2 + SCALE_LINE_X_Y_OFFSET,
                       SCALE_LINE_X_WIDTH, SCALE_LINE_X_HEIGHT};
    if (line_r.x <= X_WINDOW_OFFSET + WINDOW_W - SCALE_BOARDER) {
      SDL_RenderCopy(renderer, line, NULL, &line_r);
      render_scale_text(renderer, line_r, font, i, scale, false, position_up, order);
    }
    position_up = !position_up;
  }
  position_up = false;
  for (float i = 0; i >= -WINDOW_W / (2 * scale); i -= diff) {
    SDL_Rect line_r = {int(X_WINDOW_OFFSET + WINDOW_W / 2 +
                           i / (WINDOW_W / (scale)) * WINDOW_W) +
                           SCALE_LINE_X_X_OFFSET,
                       Y_WINDOW_OFFSET + WINDOW_H / 2 + SCALE_LINE_X_Y_OFFSET,
                       SCALE_LINE_X_WIDTH, SCALE_LINE_X_HEIGHT};
    if (line_r.x >= X_WINDOW_OFFSET + SCALE_BOARDER) {
      SDL_RenderCopy(renderer, line, NULL, &line_r);
      render_scale_text(renderer, line_r, font, i, scale, false, position_up, order);
    }
    position_up = !position_up;
  }
  position_up = false;
  for (float i = 0; i <= WINDOW_H / (2 * scale); i += diff) {
    SDL_Rect line_r = {X_WINDOW_OFFSET + WINDOW_W / 2 + SCALE_LINE_Y_X_OFFSET,
                       int(Y_WINDOW_OFFSET + WINDOW_H / 2 +
                           i / (WINDOW_H / (scale)) * WINDOW_H) +
                           SCALE_LINE_Y_Y_OFFSET,
                       SCALE_LINE_Y_WIDTH, SCALE_LINE_Y_HEIGHT};
    if (line_r.y <= Y_WINDOW_OFFSET + WINDOW_H - SCALE_BOARDER) {
      SDL_RenderCopy(renderer, line, NULL, &line_r);
      render_scale_text(renderer, line_r, font, -i, scale, true, position_up, order);
    }
  }
  for (float i = 0; i >= -WINDOW_H / (2 * scale); i -= diff) {
    SDL_Rect line_r = {X_WINDOW_OFFSET + WINDOW_W / 2 + SCALE_LINE_Y_X_OFFSET,
                       int(Y_WINDOW_OFFSET + WINDOW_H / 2 +
                           i / (WINDOW_H / (scale)) * WINDOW_H) +
                           SCALE_LINE_Y_Y_OFFSET,
                       SCALE_LINE_Y_WIDTH, SCALE_LINE_Y_HEIGHT};
    if (line_r.y >= Y_WINDOW_OFFSET + SCALE_BOARDER) {
      SDL_RenderCopy(renderer, line, NULL, &line_r);
      render_scale_text(renderer, line_r, font, -i, scale, true, position_up, order);
    }
  }
}