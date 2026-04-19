#include "constants.h"
#include "renderer.h"
#include "asteroid.h"
#include <stdlib.h>

static asteroid asteroid1 = {0.0, 0.0, 0.0, 0.0, {0.0, 12.0, 9.0, 5.0, 7.0, -8.0, -2.0, -11.0, -10.0, -3.0, -7.0, 8.0}, 12, 19, ASTEROID_SMALL};
static asteroid asteroid2 = {0.0, 0.0, 0.0, 0.0, {2.0, 15.0, 14.0, 8.0, 18.0, 0.0, 11.0, -10.0, -3.0, -13.0, -12.0, -6.0, -16.0, 4.0, -7.0, 12.0}, 16, 30, ASTEROID_MEDIUM};
static asteroid asteroid3 = {0.0, 0.0, 0.0, 0.0, {5.0, 25.0, 18.0, 16.0, 26.0, 2.0, 19.0, -14.0, 6.0, -23.0, -11.0, -21.0, -24.0, -9.0, -27.0, 6.0, -19.0, 19.0, -6.0, 24.0}, 20, 40, ASTEROID_LARGE};

asteroid create_asteroid(float x, float y, float vel_x, float vel_y, float* points, asteroid_size size) {
    asteroid new_asteroid;
    switch (size) {
        case ASTEROID_SMALL: new_asteroid = asteroid1; break;
        case ASTEROID_MEDIUM: new_asteroid = asteroid2; break;
        default: new_asteroid = asteroid3; break;
    }
    new_asteroid.coord_x = x;
    new_asteroid.coord_y = y;
    new_asteroid.vel_x = vel_x;
    new_asteroid.vel_y = vel_y;
    for (int i = 0; i < new_asteroid.point_num; i++) {
        new_asteroid.points[i] = points[i];
    }
    return new_asteroid;
}

asteroid create_random_asteroid(float x, float y, float vel_x, float vel_y) {
    int random_int = rand()%3;
    asteroid new_asteroid;
    switch (random_int) {
        case 0: new_asteroid = asteroid1; new_asteroid.size = ASTEROID_SMALL; break;
        case 1: new_asteroid = asteroid2; new_asteroid.size = ASTEROID_MEDIUM; break;
        default: new_asteroid = asteroid3; new_asteroid.size = ASTEROID_LARGE; break;
    }
    new_asteroid.coord_x = x;
    new_asteroid.coord_y = y;
    new_asteroid.vel_x = vel_x;
    new_asteroid.vel_y = vel_y;
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
    } else if (asteroid->coord_y > WINDOW_HEIGHT-150) {
        asteroid->coord_y = 0.0;
    }
}

void wrap_asteroid_array(asteroid_array *array) {
    for (int i = 0; i < array->asteroid_num; i++) {
        wrap_asteroid(&array->asteroids[i]);
    }
}

void destroy_asteroid(asteroid_array *array, int index) {
    asteroid removed_asteroid = array->asteroids[index];
    array->asteroids[index] = array->asteroids[array->asteroid_num - 1];
    array->asteroid_num--;
    if (removed_asteroid.size == ASTEROID_LARGE) {
        if (array->asteroid_num < MAX_ASTEROIDS) {
            array->asteroids[array->asteroid_num] = create_asteroid(removed_asteroid.coord_x, removed_asteroid.coord_y, removed_asteroid.vel_x+0.1, removed_asteroid.vel_y, asteroid2.points, ASTEROID_MEDIUM);
            array->asteroid_num++;
        }
        if (array->asteroid_num < MAX_ASTEROIDS) {
            array->asteroids[array->asteroid_num] = create_asteroid(removed_asteroid.coord_x, removed_asteroid.coord_y, -removed_asteroid.vel_x-0.1, removed_asteroid.vel_y, asteroid2.points, ASTEROID_MEDIUM);
            array->asteroid_num++;
        }
    }
}

void display_asteroid_array(SDL_Renderer *renderer, asteroid_array *array) {
    for (int i = 0; i < array->asteroid_num; i++) {
        display_polygon(renderer, array->asteroids[i].coord_x, array->asteroids[i].coord_y, array->asteroids[i].points, array->asteroids[i].point_num);
    }
}
