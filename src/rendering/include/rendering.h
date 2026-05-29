#ifndef __RENDERING
#define __RENDERING

#include <SDL.h>
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

SDL_Texture *text_to_texture(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color={250,250,250,250});
void render_input_box(SDL_Renderer *renderer, TTF_Font *font, int lines_x = 1,
                      int lines_y = 1);
void render_curves(SDL_Renderer *renderer, double scale);
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
void render_background(SDL_Renderer *renderer, SDL_Rect phase_poitrat, SDL_Texture* backround);

#endif