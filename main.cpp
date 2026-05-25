#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL.h>
#include "rendering.h"
#include "font.h"
#include "numeric.h"
#include "config.h"
#include "control.h"
#include "input_eq.h"
#include "parsing.h"
#include <vector>
#include <utility>
#include <string>


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

  SDL_Texture *cordinats, *arrow, *line, *tittle;
  SDL_Event e;
  SDL_Rect phase_poitrat = {X_WINDOW_OFFSET + CORDINAT_X_OFFSET,
                            Y_WINDOW_OFFSET + CORDINAT_Y_OFFSET, WINDOW_W,
                            WINDOW_H};
  cordinats = IMG_LoadTexture(renderer, "../assests/cordinats.png");
  arrow = IMG_LoadTexture(renderer, "../assests/arrow.png");
  line = IMG_LoadTexture(renderer, "../assests/rectangle.png");
  tittle = IMG_LoadTexture(renderer, "../assests/title.png");
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  double scale = BASE_SCALE;
  int lines_x = 1, lines_y = 1;
  std::string input_x = "x", input_y = "y";
  std::pair<bool, bool> choosen_equation = {false, false};
  bool running = true;
  bool is_generating = false;
  std::vector<std::pair<double, double>> curve;
  parse_input(&input_x, &input_y);

  while (running) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        running = false;
      }
    }
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 100);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 170, 170, 170, 50);
    SDL_RenderFillRect(renderer, &phase_poitrat);
    SDL_RenderFillRect(renderer, &phase_poitrat);

    if (equations::valid) {
      render_arrows(renderer, arrow, scale);
    }
    render_scale(renderer, line, scale, font);
    render_codinats_info(renderer, scale, font);
    calc_curve(scale, &is_generating, &curve);
    render_curve(renderer, &curve, scale);
    change_scale(&scale);
    get_input(&choosen_equation, &input_x, &input_y, &is_generating, &lines_x, &lines_y, &curve);
    render_equation(renderer, font, &input_x, &input_y, &lines_x, &lines_y);
    render_color_bar(renderer);
    render_color_box_scale(renderer, line, font);
    render_tittle(renderer, tittle);
    SDL_RenderCopy(renderer, cordinats, NULL, &phase_poitrat);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(cordinats);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  return 0;
}