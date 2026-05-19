#ifndef __RENDERING
#define __RENDERING

#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>

void render_curve(SDL_Renderer *renderer,std::vector<std::pair<float, float>> *curve, float scale);
void render_codinats_info(SDL_Renderer *renderer, float scale, TTF_Font *font);
void render_arrows(SDL_Renderer *renderer, SDL_Texture *arrow, float scale);
void render_scale(SDL_Renderer *renderer, SDL_Texture *line, float scale,TTF_Font *font);

#endif