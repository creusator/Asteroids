#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SDL3/SDL.h>
#include "spaceship.h"
#include "constants.h"

typedef struct {
    float coord_x;
    float coord_y;
    float speed;
    int damage;
    int index;
} projectile;

typedef struct {
    projectile projectiles[MAX_PROJECTILES];
    int num_projectiles; //taille_effective
} projectile_array;

projectile create_projectile(spaceship *ship, float speed, int damage, int index);

void update_projectile_array(projectile_array *array);

void display_projectile_array(projectile_array *array, SDL_Renderer *renderer);

void destroy_projectile(projectile_array *array, int index);

#endif
