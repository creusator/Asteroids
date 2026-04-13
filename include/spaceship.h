#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <SDL3/SDL.h>

typedef struct {
    float coord_x, coord_y;
    float points[10];
    int point_num;
    int speed;
    int width; //taille collision
} spaceship;

spaceship* create_spaceship(float x, float y, float* points, int point_num, int width);

spaceship* create_default_spaceship(float x, float y, int width);

void free_spaceship(spaceship *ship);

void move_spaceship(spaceship *ship, bool key_left, bool key_right, bool key_up, bool key_down);

#endif
