#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define WINDOW_TITLE "Asteroids"
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 1000

float asteroid1[10] = {0.0,20.0,20.0,10.0,10.0,-10.0,-10.0,-10.0,-20.0,10.0};
float spaceship1[6] = {0.0, -20.0, 20.0, 10.0, -20.0, 10.0};

typedef struct {
    bool key_left, key_right, key_up, key_down;
} input_state;

typedef struct {
    float coord_x, coord_y;
    float vel_x, vel_y;
    float points[20];
    int point_num; //taille effective
    int width; //taille collision
} asteroid;

typedef struct{
    asteroid asteroids[100];
    int asteroid_num; //taille effective
} asteroid_array;

typedef struct {
    float coord_x, coord_y;
    float points[10];
    int point_num;
    int speed;
} spaceship;

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

void collide_asteroid(bool *running, asteroid *asteroid, spaceship *spaceship) {
    float dx = asteroid->coord_x - spaceship->coord_x;
    float dy = asteroid->coord_y - spaceship->coord_y;
    if (dx*dx + dy*dy < asteroid->width * asteroid->width) {
        *running = false;
    }
}

void collide_asteroid_array(bool *running, asteroid_array *array, spaceship *spaceship) {
    for (int i = 0; i < array->asteroid_num; i++) {
        collide_asteroid(running, &array->asteroids[i], spaceship);
    }
}

void display_polygon(SDL_Renderer *renderer, float coord_x, float coord_y, float* points, int point_num) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // blanc
    for (int i = 0; i < point_num/2; i++) {
        SDL_RenderLine(renderer, coord_x + points[(i*2) % (point_num)], //x1
                                 coord_y + points[(i*2+1) % (point_num)], //y1
                                 coord_x + points[(i*2+2) % (point_num)], //x2
                                 coord_y + points[(i*2+3) % (point_num)]); //y2
    }
}

void display_asteroid_array(SDL_Renderer *renderer, asteroid_array *array) {
    for (int i = 0; i < array->asteroid_num; i++) {
        display_polygon(renderer, array->asteroids[i].coord_x, array->asteroids[i].coord_y, array->asteroids[i].points, array->asteroids[i].point_num);
    }
}

spaceship* create_spaceship(float x, float y, float* points, int point_num) {
    spaceship *new_ship = malloc(sizeof(spaceship));
    new_ship->coord_x = x;
    new_ship->coord_y = y;
    for (int i = 0; i < point_num; i++) {
        new_ship->points[i] = points[i];
    }
    new_ship->point_num = point_num;
    new_ship->speed = 1;
    return new_ship;
}

void free_spaceship(spaceship *ship) {
    free(ship);
}

void move_spaceship(spaceship *ship, bool key_left, bool key_right, bool key_up, bool key_down) {
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
}

int main(void)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window   *window   = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    SDL_SetRenderVSync(renderer, 1);

    bool running = true;
    SDL_Event event;

    input_state input = {false, false, false, false};
    asteroid_array asteroids;
    asteroids.asteroid_num = 0;
    asteroids.asteroids[0] = create_asteroid(WINDOW_WIDTH/2, 0, 0.1, 0.7, asteroid1, 10, 30);
    asteroids.asteroid_num = 1;
    spaceship* ship = create_spaceship(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, spaceship1, 6);

    while (running) {
        // --- Events ---
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT){
                running = false;
            }
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE)
                    running = false;
                if (event.key.key == SDLK_Q){
                    input.key_left = true;
                }
                if (event.key.key == SDLK_D){
                    input.key_right = true;
                }
                if (event.key.key == SDLK_Z){
                    input.key_up = true;
                }
                if (event.key.key == SDLK_S){
                    input.key_down = true;
                }
            }
            if (event.type == SDL_EVENT_KEY_UP) {
                if (event.key.key == SDLK_Q){
                    input.key_left = false;
                }
                if (event.key.key == SDLK_D){
                    input.key_right = false;
                }
                if (event.key.key == SDLK_Z){
                    input.key_up = false;
                }
                if (event.key.key == SDLK_S){
                    input.key_down = false;
                }
            }
        }

        // --- Update ---
        move_spaceship(ship, input.key_left, input.key_right, input.key_up, input.key_down);
        move_asteroid_array(&asteroids);
        wrap_asteroid_array(&asteroids);
        collide_asteroid_array(&running, &asteroids, ship);

        // --- Rendu ---
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // noir
        SDL_RenderClear(renderer);
        display_asteroid_array(renderer, &asteroids);
        display_polygon(renderer, ship->coord_x, ship->coord_y, ship->points, ship->point_num);
        SDL_RenderPresent(renderer);
    }

    free_spaceship(ship);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
