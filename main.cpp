#include "config.h"
#include "control.h"
#include "font.h"
#include "input_eq.h"
#include "numeric.h"
#include "parsing.h"
#include "rendering.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <string>
#include <utility>
#include <vector>

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

  SDL_Texture *cordinats, *arrow, *line, *tittle, *background;
  SDL_Event e;
  SDL_Rect phase_poitrat = {X_WINDOW_OFFSET + CORDINAT_X_OFFSET,
                            Y_WINDOW_OFFSET + CORDINAT_Y_OFFSET, WINDOW_W,
                            WINDOW_H};
  cordinats = IMG_LoadTexture(renderer, "../assests/cordinats.png");
  arrow = IMG_LoadTexture(renderer, "../assests/arrow.png");
  line = IMG_LoadTexture(renderer, "../assests/rectangle.png");
  tittle = IMG_LoadTexture(renderer, "../assests/title.png");
  background = IMG_LoadTexture(renderer, "../assests/bacground.png");
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  double scale = BASE_SCALE;
  int lines_x = 1, lines_y = 1;
  std::string input_x = "x", input_y = "y";
  std::pair<bool, bool> choosen_equation = {false, false};
  bool running = true;
  bool is_generating = false;
  parse_input(&input_x, &input_y);
  bool is_changed = true;

  while (running) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        running = false;
      }
    }
    render_background(renderer, phase_poitrat, background);

    if (equations::valid) {
      if (is_changed) {
        calculate_arrow_angle(WINDOW_W / scale);
        calculate_arrow_color(WINDOW_W / scale);
      }
      render_arrows(renderer, arrow, scale);
    }
    calc_curves(scale, &is_generating);
    render_curves(renderer, scale);
    render_scale(renderer, line, scale, font);
    render_codinats_info(renderer, scale, font);
    is_changed = (change_scale(&scale) ||
                  get_input(&choosen_equation, &input_x, &input_y,
                            &is_generating, &lines_x, &lines_y));
    render_equation(renderer, font, &input_x, &input_y, &lines_x, &lines_y, line);
    render_color_bar(renderer);
    render_color_box_scale(renderer, line, font);
    render_tittle(renderer, tittle);
    SDL_RenderCopy(renderer, cordinats, NULL, &phase_poitrat);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(cordinats);
  SDL_DestroyTexture(arrow);
  SDL_DestroyTexture(line);
  SDL_DestroyTexture(tittle);
  SDL_DestroyTexture(background);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  return 0;
}