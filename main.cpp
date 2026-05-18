#include "config.h"
#include "font.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <vector>

float x_equation(float x, float y) { return x - pow(x, 3) / 3 - y; }

float y_equation(float x, float y) { return x; }

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

void render_scale_text(SDL_Renderer *renderer, SDL_Rect line, TTF_Font* font, float number, bool vertical, bool position_up){
    SDL_Color black = {0,0,0, 255};
    std::stringstream stream;
    if ((number<1e4 && number>1e-4) || (number>-1e4 && number<-1e-4) || number==0) stream << std::fixed << std::setprecision(0) << number;
    else stream << std::scientific << std::setprecision(1) << number;
    const char* num_text = stream.str().c_str();
    SDL_Surface* text_surface = TTF_RenderText_Blended(font, num_text ,black);
    SDL_Texture* text = SDL_CreateTextureFromSurface(renderer,text_surface);
    SDL_FreeSurface(text_surface);
    SDL_Rect text_pos = line;
    SDL_QueryTexture(text, NULL, NULL, &text_pos.w, &text_pos.h);
    if (vertical) text_pos.x+=SCALE_TEXT_OFFSET_Y_AXIS;
    else {
        text_pos.x-=text_pos.w/3;
        if(position_up) text_pos.y+=SCALE_TEXT_OFFSET_X_AXIS_UP;
        else text_pos.y+=SCALE_TEXT_OFFSET_X_AXIS_DOWN;
    }
    text_pos.w*=FONT_SCALE;
    text_pos.h*=FONT_SCALE;
    if(number !=0 || !vertical) SDL_RenderCopy(renderer, text, NULL, &text_pos);
    SDL_DestroyTexture(text);
}

void render_scale(SDL_Renderer *renderer, SDL_Texture *line, float scale, TTF_Font* font) {
  int order = std::ceil(std::log10(WINDOW_W / (2 * scale)));
  if ((WINDOW_W / (2 * scale)) / pow(10, order - 1) < SCALE_MIN_LINE_AMMOUNT) {
    order--;
  }
  int diff = pow(10, order - 1);
  bool position_up = false;
  for (float i = 0; i <= WINDOW_W / (2 * scale); i += diff) {
    SDL_Rect line_r = {int(X_WINDOW_OFFSET + WINDOW_W / 2 +
                           i / (WINDOW_W / (scale)) * WINDOW_W) +
                           SCALE_LINE_X_X_OFFSET,
                       Y_WINDOW_OFFSET + WINDOW_H / 2 + SCALE_LINE_X_Y_OFFSET,
                       SCALE_LINE_X_WIDTH, SCALE_LINE_X_HEIGHT};
    if (line_r.x <= X_WINDOW_OFFSET + WINDOW_W - SCALE_BOARDER){
      SDL_RenderCopy(renderer, line, NULL, &line_r);
      render_scale_text(renderer, line_r, font, i, false, position_up);
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
    if (line_r.x >= X_WINDOW_OFFSET + SCALE_BOARDER){
      SDL_RenderCopy(renderer, line, NULL, &line_r);
      render_scale_text(renderer, line_r, font, i, false, position_up);
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
    if (line_r.y <= Y_WINDOW_OFFSET + WINDOW_H - SCALE_BOARDER){
      SDL_RenderCopy(renderer, line, NULL, &line_r);
      render_scale_text(renderer, line_r, font, -i, true, position_up);
    }
  }
  for (float i = 0; i >= -WINDOW_H / (2 * scale); i -= diff) {
    SDL_Rect line_r = {X_WINDOW_OFFSET + WINDOW_W / 2 + SCALE_LINE_Y_X_OFFSET,
                       int(Y_WINDOW_OFFSET + WINDOW_H / 2 +
                           i / (WINDOW_H / (scale)) * WINDOW_H) +
                           SCALE_LINE_Y_Y_OFFSET,
                       SCALE_LINE_Y_WIDTH, SCALE_LINE_Y_HEIGHT};
    if (line_r.y >= Y_WINDOW_OFFSET + SCALE_BOARDER){
      SDL_RenderCopy(renderer, line, NULL, &line_r);
      render_scale_text(renderer, line_r, font, -i, true, position_up);
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

  float scale = 100;
  bool running = true;

  while (running) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        running = false;
      }
    }
    SDL_SetRenderDrawColor(renderer, 100, 200, 200, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 0);
    SDL_RenderFillRect(renderer, &phase_poitrat);

    render_arrows(renderer, arrow, scale);
    render_scale(renderer, line, scale, font);

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