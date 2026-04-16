#ifndef COLLISION_H
#define COLLISION_H

#include <SDL3/SDL.h>
#include "asteroid.h"
#include "spaceship.h"
#include "projectile.h"

void collide_asteroid(asteroid *asteroid, spaceship *spaceship);

void collide_asteroid_array(asteroid_array *array, spaceship *spaceship);

bool collide_projectile(projectile_array *projectiles, int index, asteroid_array *asteroid_array, int asteroid_index);

void collide_projectile_array(projectile_array *array, asteroid_array *asteroid_array, int *score);

#endif
