#ifndef __RENDERING
#define __RENDERING

#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

void render_input_box(SDL_Renderer* renderer, TTF_Font* font, int lines_x=1, int lines_y=1);
void render_curve(SDL_Renderer *renderer,std::vector<std::pair<float, float>> *curve, float scale);
void render_codinats_info(SDL_Renderer *renderer, float scale, TTF_Font *font);
void render_arrows(SDL_Renderer *renderer, SDL_Texture *arrow, float scale);
void render_scale(SDL_Renderer *renderer, SDL_Texture *line, float scale,TTF_Font *font);
void render_equation(SDL_Renderer *renderer, TTF_Font *font, std::string* equation_x, std::string* equation_y, int* lines_x, int* lines_y);

#endif