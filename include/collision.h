#ifndef COLLISION_H
#define COLLISION_H

#include <SDL3/SDL.h>
#include "asteroid.h"
#include "spaceship.h"

void collide_asteroid(bool *running, asteroid *asteroid, spaceship *spaceship);

void collide_asteroid_array(bool *running, asteroid_array *array, spaceship *spaceship);

#endif
