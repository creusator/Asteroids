#include "collision.h"
#include <stdbool.h>

void collide_asteroid(asteroid *asteroid, spaceship *spaceship) {
    float dx = asteroid->coord_x - spaceship->coord_x;
    float dy = asteroid->coord_y - spaceship->coord_y;
    float r = asteroid->width + spaceship->width;
    if (dx*dx + dy*dy < r * r && spaceship->invincibility_frames == 0) {
        spaceship->health -= 1;
        spaceship->invincibility_frames = 90;
    }
}

void collide_asteroid_array(asteroid_array *array, spaceship *spaceship) {
    for (int i = 0; i < array->asteroid_num; i++) {
        collide_asteroid(&array->asteroids[i], spaceship);
    }
}

bool collide_projectile(projectile_array *projectiles, int proj_index, asteroid_array *asteroid_array, int asteroid_index) {
    float dx = projectiles->projectiles[proj_index].coord_x - asteroid_array->asteroids[asteroid_index].coord_x;
    float dy = projectiles->projectiles[proj_index].coord_y - asteroid_array->asteroids[asteroid_index].coord_y;
    float r = asteroid_array->asteroids[asteroid_index].width;
    if (dx*dx + dy*dy < r * r) {
        destroy_projectile(projectiles, proj_index);
        destroy_asteroid(asteroid_array, asteroid_index);
        return true;
    }
    return false;
}

void collide_projectile_array(projectile_array *array, asteroid_array *asteroid_array, int *score) {
    for (int i = 0; i < array->num_projectiles; i++) {
        for (int j = 0; j < asteroid_array->asteroid_num; j++) {
            if (collide_projectile(array, i, asteroid_array, j)) {
                *score += 1;
                i--;
            }
        }
    }
}
