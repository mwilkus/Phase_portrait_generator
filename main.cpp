#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL.h>
#include "rendering.h"
#include "font.h"
#include "numeric.h"
#include "config.h"
#include <vector>
#include <utility>

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
  cordinats = IMG_LoadTexture(renderer, "../assests/cordinats.png");
  arrow = IMG_LoadTexture(renderer, "../assests/arrow.png");
  line = IMG_LoadTexture(renderer, "../assests/rectangle.png");
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  float scale = 100;
  bool running = true;
  bool is_generating = false;
  std::vector<std::pair<float, float>> curve;

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
    calc_curve(scale, &is_generating, &curve);
    render_curve(renderer, &curve, scale);

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