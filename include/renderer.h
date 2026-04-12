#ifndef RENDERER_H
#define RENDERER_H

#include <SDL3/SDL.h>

void display_polygon(SDL_Renderer *renderer, float coord_x, float coord_y, float* points, int point_num);

#endif
