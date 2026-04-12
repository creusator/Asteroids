#include "../include/renderer.h"

void display_polygon(SDL_Renderer *renderer, float coord_x, float coord_y, float* points, int point_num) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // blanc
    for (int i = 0; i < point_num/2; i++) {
        SDL_RenderLine(renderer, coord_x + points[(i*2) % (point_num)], //x1
                                 coord_y + points[(i*2+1) % (point_num)], //y1
                                 coord_x + points[(i*2+2) % (point_num)], //x2
                                 coord_y + points[(i*2+3) % (point_num)]); //y2
    }
}
