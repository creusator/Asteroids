#include "../include/constants.h"
#include "../include/renderer.h"
#include "../include/asteroid.h"


asteroid create_asteroid(float x, float y, float vel_x, float vel_y, float* points, int point_num, int width) {
    asteroid new_asteroid;
    new_asteroid.coord_x = x;
    new_asteroid.coord_y = y;
    new_asteroid.vel_x = vel_x;
    new_asteroid.vel_y = vel_y;
    new_asteroid.point_num = point_num;
    new_asteroid.width = width;
    for (int i = 0; i < point_num; i++) {
        new_asteroid.points[i] = points[i];
    }
    return new_asteroid;
}

void move_asteroid(asteroid *asteroid) {
    asteroid->coord_x += asteroid->vel_x;
    asteroid->coord_y += asteroid->vel_y;
}

void move_asteroid_array(asteroid_array *array) {
    for (int i = 0; i < array->asteroid_num; i++) {
        move_asteroid(&array->asteroids[i]);
    }
}

void wrap_asteroid(asteroid *asteroid) {
    if (asteroid->coord_x < 0){
        asteroid->coord_x += WINDOW_WIDTH;
    } else if (asteroid->coord_x > WINDOW_WIDTH) {
        asteroid->coord_x -= WINDOW_WIDTH;
    }
    if (asteroid->coord_y < 0){
        asteroid->coord_y += WINDOW_HEIGHT;
    } else if (asteroid->coord_y > WINDOW_HEIGHT) {
        asteroid->coord_y -= WINDOW_HEIGHT;
    }
}

void wrap_asteroid_array(asteroid_array *array) {
    for (int i = 0; i < array->asteroid_num; i++) {
        wrap_asteroid(&array->asteroids[i]);
    }
}

void display_asteroid_array(SDL_Renderer *renderer, asteroid_array *array) {
    for (int i = 0; i < array->asteroid_num; i++) {
        display_polygon(renderer, array->asteroids[i].coord_x, array->asteroids[i].coord_y, array->asteroids[i].points, array->asteroids[i].point_num);
    }
}
