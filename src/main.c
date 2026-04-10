#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define WINDOW_TITLE "Asteroids"
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 1000

bool key_left = false;
bool key_right = false;
bool key_up = false;
bool key_down = false;

typedef struct {
    float coord_x, coord_y;
    float offset[4];
    int speed;
} spaceship;

spaceship* create_spaceship(float x, float y, float o1x, float o1y, float o2x, float o2y) {
    spaceship *new_ship = malloc(sizeof(spaceship));
    new_ship->coord_x = x;
    new_ship->coord_y = y;
    new_ship->offset[0] = o1x;
    new_ship->offset[1] = o1y;
    new_ship->offset[2] = o2x;
    new_ship->offset[3] = o2y;
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

void display_spaceship(SDL_Renderer *renderer, spaceship *ship) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // blanc
    SDL_RenderLine(renderer, ship->coord_x, ship->coord_y, ship->coord_x+ship->offset[0], ship->coord_y+ship->offset[1]);
    SDL_RenderLine(renderer, ship->coord_x+ship->offset[0], ship->coord_y+ship->offset[1], ship->coord_x+ship->offset[2], ship->coord_y+ship->offset[3]);
    SDL_RenderLine(renderer, ship->coord_x+ship->offset[2], ship->coord_y+ship->offset[3], ship->coord_x, ship->coord_y);
}

int main(void)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window   *window   = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    SDL_SetRenderVSync(renderer, 1); // VSync activé (obligatoire en SDL3, pas automatique)

    bool running = true;
    SDL_Event event;


    spaceship* ship = create_spaceship(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, -20.0, 25.0, 20.0, 25.0);

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
                    key_left = true;
                }
                if (event.key.key == SDLK_D){
                    key_right = true;
                }
                if (event.key.key == SDLK_Z){
                    key_up = true;
                }
                if (event.key.key == SDLK_S){
                    key_down = true;
                }
            }
            if (event.type == SDL_EVENT_KEY_UP) {
                if (event.key.key == SDLK_Q){
                    key_left = false;
                }
                if (event.key.key == SDLK_D){
                    key_right = false;
                }
                if (event.key.key == SDLK_Z){
                    key_up = false;
                }
                if (event.key.key == SDLK_S){
                    key_down = false;
                }
            }
        }

        move_spaceship(ship, key_left, key_right, key_up, key_down);

        // --- Rendu ---
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // noir
        SDL_RenderClear(renderer);
        display_spaceship(renderer, ship);
        SDL_RenderPresent(renderer);
    }

    free_spaceship(ship);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
