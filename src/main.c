#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "constants.h"
#include "renderer.h"
#include "asteroid.h"
#include "spaceship.h"
#include "collision.h"
#include "input.h"

int main(void)
{
    srand(time(NULL));
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    SDL_SetRenderVSync(renderer, 1);

    bool running = true;
    SDL_Event event;

    input_state input = {false, false, false, false};
    asteroid_array asteroids;
    asteroids.asteroid_num = 0;
    asteroids.asteroids[0] = create_random_asteroid(WINDOW_WIDTH/2, 0, 0.1, 0.7);
    asteroids.asteroid_num = 1;
    spaceship* ship = create_default_spaceship(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 10);

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
