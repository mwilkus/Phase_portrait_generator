#include "config.h"
#include "font.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <utility>
#include <vector>

float x_equation(float x, float y) { return x - pow(x, 3) / 3 - y; }

float y_equation(float x, float y) { return x; }

std::pair<float,float> RK4(float x, float y){
  float h =TIME_INTERVAL;
  float k1 = x_equation(x,y);
  float l1 = y_equation(x,y);
  float k2 = x_equation(x+h*k1/2, y+h*l1/2);
  float l2 = y_equation(x+h*k1/2, y+h*l1/2);
  float k3 = x_equation(x+h*k2/2, y+h*l2/2);
  float l3 = y_equation(x+h*k2/2, y+h*l2/2);
  float k4 = x_equation(x+h*k3, y+h*l3);
  float l4 = y_equation(x+h*k3, y+h*l3);
  x+=h/6*(k1+2*k2+2*k3+k4);
  y+=h/6*(l1+2*l2+2*l3+l4);
  return {x,y};
}

void render_curve(SDL_Renderer* renderer, std::vector<std::pair<float,float>>* curve, float scale){  
  for (auto point : *curve){
    point.first = point.first*scale + X_WINDOW_OFFSET + WINDOW_W / 2 + SCALE_LINE_X_X_OFFSET;
    point.second = Y_WINDOW_OFFSET + WINDOW_H / 2 + SCALE_LINE_Y_Y_OFFSET - point.second*scale;
    SDL_Rect point_rect = {int(point.first)-POINT_WIDTH/2,int(point.second)-POINT_HEIGHT/2,POINT_WIDTH,POINT_HEIGHT};
    if (point.first >= X_WINDOW_OFFSET + CORDS_INFO_BOARDER &&
      point.first <= X_WINDOW_OFFSET + WINDOW_W - CORDS_INFO_BOARDER &&
      point.second >= Y_WINDOW_OFFSET + CORDS_INFO_BOARDER &&
      point.second <= Y_WINDOW_OFFSET + WINDOW_H - CORDS_INFO_BOARDER)
    {
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
      SDL_RenderFillRect(renderer, &point_rect);
    }
  }
}

void calc_curve(SDL_Renderer* renderer, float scale, bool* is_generating, std::vector<std::pair<float,float>>* curve){
  int mouse_x, mouse_y;
  float x,y;
  Uint32 mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);
  if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)){
    *is_generating = true;
    curve->clear();
    x = (mouse_x - (X_WINDOW_OFFSET + WINDOW_W / 2 + SCALE_LINE_X_X_OFFSET))/scale;
    y = (Y_WINDOW_OFFSET + WINDOW_H / 2 + SCALE_LINE_Y_Y_OFFSET - mouse_y)/scale;
    curve->push_back({x,y});
  }
  else if (*is_generating){
    std::pair<float, float> last_cords = curve->back();
    std::pair<float, float> new_cords = RK4(last_cords.first, last_cords.second);
    curve->push_back(new_cords);
  }
  render_curve(renderer, curve, scale);
}

std::vector<std::vector<int>> calculate_arrow_angle(float range) {
  std::vector<std::vector<int>> output;
  float x = -range / 2, y = range / 2;
  float diff = range / (AMMOUNT_OF_ARROWS - 1);
  for (int i = 0; i <= AMMOUNT_OF_ARROWS; i++) {
    std::vector<int> line;
    for (int j = 0; j <= AMMOUNT_OF_ARROWS; j++) {
      float dy = y_equation(x, y);
      float dx = x_equation(x, y);
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

void render_codinats_info(SDL_Renderer *renderer, float scale, TTF_Font *font) {
  int mouse_x, mouse_y;
  Uint32 mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);
  SDL_Rect cords_info = {mouse_x+CORDS_INFO_X_OFFSET, mouse_y+CORDS_INFO_Y_OFFSET, CORDS_INFO_BOX_WIDTH,
                         CORDS_INFO_BOX_HEIGHT};
  float x = (mouse_x - (X_WINDOW_OFFSET + WINDOW_W / 2 + SCALE_LINE_X_X_OFFSET))/scale;
  float y = (Y_WINDOW_OFFSET + WINDOW_H / 2 + SCALE_LINE_Y_Y_OFFSET - mouse_y)/scale;
  SDL_Color black = {0, 0, 0, 255};
  std::stringstream stream_x, stream_y;

  if (mouse_x >= X_WINDOW_OFFSET + CORDS_INFO_BOARDER &&
      mouse_x <= X_WINDOW_OFFSET + WINDOW_W - CORDS_INFO_BOARDER &&
      mouse_y >= Y_WINDOW_OFFSET + CORDS_INFO_BOARDER &&
      mouse_y <= Y_WINDOW_OFFSET + WINDOW_H - CORDS_INFO_BOARDER) {
    int order = std::ceil(std::log10(WINDOW_W / (2 * scale)));
    if ((WINDOW_W / (2 * scale)) / pow(10, order - 1) < SCALE_MIN_LINE_AMMOUNT) {
      order--;
    }
    int prec = std::max(2,-(order-2));
    if ((x < 1e4 && x > 1e-4) || (x > -1e4 && x < -1e-4) || x == 0)
      stream_x << std::fixed << std::setprecision(prec) << x;
    else
      stream_x << std::scientific << std::setprecision(2) << x;

    if ((y < 1e4 && y > 1e-4) || (y > -1e4 && y < -1e-4) || y == 0)
      stream_y << std::fixed << std::setprecision(prec) << y;
    else
      stream_y << std::scientific << std::setprecision(2) << y;
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 100);
    SDL_RenderFillRect(renderer, &cords_info);
    std::string cords = stream_x.str();
    cords += ", ";
    cords += stream_y.str();
    const char *cords_c = cords.c_str();
    SDL_Surface *text_surface = TTF_RenderText_Blended(font, cords_c, black);
    SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_FreeSurface(text_surface);
    SDL_RenderCopy(renderer, text, NULL, &cords_info);
    SDL_DestroyTexture(text);
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
        SDL_RenderCopyEx(renderer, arrow, NULL, &arrow_pos, angle, &rot_axis,
                         SDL_FLIP_NONE);
      }
      x_index++;
    }
    x_index = 0;
    y_index++;
  }
}

void render_scale_text(SDL_Renderer *renderer, SDL_Rect line, TTF_Font *font,
                       float number, bool vertical, bool position_up, int order) {
  SDL_Color black = {0, 0, 0, 255};
  std::stringstream stream;
  if ((number < 1e4 && number > 1e-4) || (number > -1e4 && number < -1e-4) ||
      number == 0){
    int prec = std::max(0,-(order-1));
    stream << std::fixed << std::setprecision(prec) << number;
  }
  else{
    stream << std::scientific << std::setprecision(1) << number;
  }
  const char *num_text = stream.str().c_str();
  SDL_Surface *text_surface = TTF_RenderText_Blended(font, num_text, black);
  SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, text_surface);
  SDL_FreeSurface(text_surface);
  SDL_Rect text_pos = line;
  SDL_QueryTexture(text, NULL, NULL, &text_pos.w, &text_pos.h);
  if (vertical)
    text_pos.x += SCALE_TEXT_OFFSET_Y_AXIS;
  else {
    text_pos.x -= text_pos.w / 3;
    if (position_up)
      text_pos.y += SCALE_TEXT_OFFSET_X_AXIS_UP;
    else
      text_pos.y += SCALE_TEXT_OFFSET_X_AXIS_DOWN;
  }
  text_pos.w *= FONT_SCALE;
  text_pos.h *= FONT_SCALE;
  if (number != 0 || !vertical)
    SDL_RenderCopy(renderer, text, NULL, &text_pos);
  SDL_DestroyTexture(text);
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
      render_scale_text(renderer, line_r, font, i, false, position_up, order);
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
      render_scale_text(renderer, line_r, font, i, false, position_up, order);
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
      render_scale_text(renderer, line_r, font, -i, true, position_up, order);
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
      render_scale_text(renderer, line_r, font, -i, true, position_up, order);
    }
  }
}

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  IMG_Init(IMG_INIT_PNG);
  TTF_Init();
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
  SDL_Window *window =
      SDL_CreateWindow("Project", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_SHOWN);
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, 1, SDL_RENDERER_PRESENTVSYNC);
  SDL_RWops *rw = SDL_RWFromMem(Roboto_Black_ttf, Roboto_Black_ttf_len);
  TTF_Font *font = TTF_OpenFontRW(rw, 1, 14);

  SDL_Texture *cordinats, *arrow, *line;
  SDL_Event e;
  SDL_Rect phase_poitrat = {X_WINDOW_OFFSET + CORDINAT_X_OFFSET,
                            Y_WINDOW_OFFSET + CORDINAT_Y_OFFSET, WINDOW_W,
                            WINDOW_H};
  cordinats = IMG_LoadTexture(renderer, "cordinats.png");
  arrow = IMG_LoadTexture(renderer, "arrow.png");
  line = IMG_LoadTexture(renderer, "rectangle.png");
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  float scale = 100;
  bool running = true;
  bool is_generating = false;
  std::vector<std::pair<float,float>> curve;

  while (running) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        running = false;
      }
    }
    SDL_SetRenderDrawColor(renderer, 100, 200, 200, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &phase_poitrat);

    render_arrows(renderer, arrow, scale);
    render_scale(renderer, line, scale, font);
    render_codinats_info(renderer, scale, font);
    calc_curve(renderer, scale, &is_generating, &curve);

    const uint8_t *keyboard_state = SDL_GetKeyboardState(NULL);
    if (keyboard_state[SDL_SCANCODE_UP] == true) {
      scale *= 1.01;
    } else if (keyboard_state[SDL_SCANCODE_DOWN] == true) {
      scale /= 1.01;
    }

    SDL_RenderCopy(renderer, cordinats, NULL, &phase_poitrat);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(cordinats);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  return 0;
}