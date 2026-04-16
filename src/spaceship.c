#include "spaceship.h"
#include "constants.h"
#include <SDL3/SDL.h>
#include <stdlib.h>

static float default_shape[6] = {0.0, -20.0, 20.0, 10.0, -20.0, 10.0};

spaceship* create_spaceship(float x, float y, float* points, int point_num, int width, int health) {
    spaceship *new_ship = malloc(sizeof(spaceship));
    new_ship->coord_x = x;
    new_ship->coord_y = y;
    for (int i = 0; i < point_num; i++) {
        new_ship->points[i] = points[i];
    }
    new_ship->point_num = point_num;
    new_ship->speed = PLAYER_SPEED;
    new_ship->width = width;
    new_ship->health = health;
    return new_ship;
}

spaceship* create_default_spaceship(float x, float y, int width) {
    return create_spaceship(x, y, default_shape, 6, width, 3);
}

void update_spaceship(spaceship *ship, bool key_left, bool key_right, bool key_up, bool key_down) {
    if (key_left) {
        ship->coord_x -= ship->speed;
    }
    if (key_right) {
        ship->coord_x += ship->speed;
    }
    if (key_up) {
        ship->coord_y -= ship->speed;
    }
    if (key_down) {
        ship->coord_y += ship->speed;
    }
    //TODO : refaire les limites relativement a la forme du vaisseau (pas de magic numbers)
    if (ship->coord_x-20 < 0) {
        ship->coord_x = 20;
    }
    if (ship->coord_x+20 > WINDOW_WIDTH) {
        ship->coord_x = WINDOW_WIDTH - 20;
    }
    if (ship->coord_y-20 < 0) {
        ship->coord_y = 20;
    }
    if (ship->coord_y+15 > WINDOW_HEIGHT-150) {
        ship->coord_y = WINDOW_HEIGHT-150 - 15;
    }
    if (ship->invincibility_frames > 0)
        ship->invincibility_frames--;
}

void free_spaceship(spaceship *ship) {
    free(ship);
}
