#ifndef __RENDERING
#define __RENDERING

#include <SDL.h>
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

void render_input_box(SDL_Renderer *renderer, TTF_Font *font, int lines_x = 1,
                      int lines_y = 1);
void render_curve(SDL_Renderer *renderer,
                  std::vector<std::pair<double, double>> *curve, double scale);
void render_codinats_info(SDL_Renderer *renderer, double scale, TTF_Font *font);
void render_arrows(SDL_Renderer *renderer, SDL_Texture *arrow, double scale);
void render_scale(SDL_Renderer *renderer, SDL_Texture *line, double scale,
                  TTF_Font *font);
void render_equation(SDL_Renderer *renderer, TTF_Font *font,
                     std::string *equation_x, std::string *equation_y,
                     int *lines_x, int *lines_y, SDL_Texture* line);
void render_color_bar(SDL_Renderer *renderer);
void render_color_box_scale(SDL_Renderer *renderer, SDL_Texture *line,
                            TTF_Font *font);
void render_tittle(SDL_Renderer *renderer, SDL_Texture *title_texture);

#endif