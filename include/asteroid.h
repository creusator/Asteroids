#ifndef ASTEROID_H
#define ASTEROID_H

#include <SDL3/SDL.h>
#include "constants.h"

#define MAX_ASTEROID_POINTS 20

typedef struct {
    float coord_x, coord_y;
    float vel_x, vel_y;
    float points[MAX_ASTEROID_POINTS];
    int point_num; //taille effective
    int width; //taille collision
} asteroid;

typedef struct{
    asteroid asteroids[MAX_ASTEROIDS];
    int asteroid_num; //taille effective
} asteroid_array;

asteroid create_asteroid(float x, float y, float vel_x, float vel_y, float* points, int point_num, int width);

asteroid create_random_asteroid(float x, float y, float vel_x, float vel_y);

void move_asteroid(asteroid *asteroid);

void move_asteroid_array(asteroid_array *array);

void wrap_asteroid(asteroid *asteroid);

void wrap_asteroid_array(asteroid_array *array);

void destroy_asteroid(asteroid_array *array, int index);

void display_asteroid_array(SDL_Renderer *renderer, asteroid_array *array);

#endif
