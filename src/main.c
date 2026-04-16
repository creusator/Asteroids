#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "constants.h"
#include "renderer.h"
#include "asteroid.h"
#include "spaceship.h"
#include "projectile.h"
#include "collision.h"
#include "input.h"
#include "user_interface.h"

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_TextEngine *engine;
    TTF_Font *font;
    TTF_Text *score_label;
    TTF_Text *score_value;
    TTF_Text *health_label;
    TTF_Text *health_value;

    spaceship* ship;

    asteroid_array asteroids;
    int new_asteroid_tick;

    int projectile_tick;
    projectile_array projectiles;

    input_state input;

    int score;
} app_state;


/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]){
    app_state *state = calloc(1, sizeof(app_state));
    *appstate = state;
    state->projectile_tick = 30; // déjà chargé pour tirer dès le début
    (void)argc;
    (void)argv;

    /* Create the window */
    if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0, &state->window, &state->renderer)) {
        SDL_Log("Couldn't create window and renderer: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderVSync(state->renderer, 1);

    if (!TTF_Init()) {
        SDL_Log("Couldn't initialize SDL_ttf: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    /* Load the font */
    state->font = TTF_OpenFont("fonts/SpaceGrotesk-Medium.ttf", 24);
    if (!state->font) {
        SDL_Log("Couldn't open font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    /* Create the text engine */
    state->engine = TTF_CreateRendererTextEngine(state->renderer);
    if (!state->engine) {
        SDL_Log("Couldn't create text engine: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    /* Create the texts */
    state->score_label = TTF_CreateText(state->engine, state->font, "Score: ", 0);
    TTF_SetTextColor(state->score_label, 255, 255, 255, 255); // White
    state->score_value = TTF_CreateText(state->engine, state->font, "0", 0);
    TTF_SetTextColor(state->score_value, 255, 255, 255, 255); // White
    state->health_label = TTF_CreateText(state->engine, state->font, "Health: ", 0);
    TTF_SetTextColor(state->health_label, 255, 255, 255, 255); // White
    state->health_value = TTF_CreateText(state->engine, state->font, "0", 0);
    TTF_SetTextColor(state->health_value, 255, 255, 255, 255); // White

    state->ship = create_default_spaceship(WINDOW_WIDTH/2.0, WINDOW_HEIGHT/2.0, 10);
    srand(time(NULL));
    state->asteroids.asteroid_num = 0;
    state->projectiles.num_projectiles = 0;

    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){
    app_state *state = (app_state *)appstate;

    if (event->type == SDL_EVENT_QUIT){
        return SDL_APP_SUCCESS;
    }
    if (event->type == SDL_EVENT_KEY_DOWN) {
        if (event->key.key == SDLK_ESCAPE)
            return SDL_APP_SUCCESS;
        if (event->key.key == SDLK_Q){
            state->input.key_left = true;
        }
        if (event->key.key == SDLK_D){
            state->input.key_right = true;
        }
        if (event->key.key == SDLK_Z){
            state->input.key_up = true;
        }
        if (event->key.key == SDLK_S){
            state->input.key_down = true;
        }
        if (event->key.key == SDLK_SPACE){
            state->input.key_shoot = true;
        }
    }
    if (event->type == SDL_EVENT_KEY_UP) {
        if (event->key.key == SDLK_Q){
            state->input.key_left = false;
        }
        if (event->key.key == SDLK_D){
            state->input.key_right = false;
        }
        if (event->key.key == SDLK_Z){
            state->input.key_up = false;
        }
        if (event->key.key == SDLK_S){
            state->input.key_down = false;
        }
        if (event->key.key == SDLK_SPACE){
            state->input.key_shoot = false;
        }
    }
    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) {
    app_state *state = (app_state *)appstate;

    update_spaceship(state->ship, state->input.key_left, state->input.key_right, state->input.key_up, state->input.key_down);
    if (state->ship->health <= 0) {
        return SDL_APP_SUCCESS;
    }
    move_asteroid_array(&state->asteroids);
    update_projectile_array(&state->projectiles);
    wrap_asteroid_array(&state->asteroids);
    collide_asteroid_array(&state->asteroids, state->ship);
    collide_projectile_array(&state->projectiles, &state->asteroids, &state->score);

    if (state->new_asteroid_tick >= 60 && state->asteroids.asteroid_num<MAX_ASTEROIDS){
        state->new_asteroid_tick = 0;
        float rx = rand() % WINDOW_WIDTH;
        float rvx = (((rand()%10)-5)*0.05);
        float rvy = ((rand()%10)*0.05)+0.5;
        state->asteroids.asteroids[state->asteroids.asteroid_num]=create_random_asteroid(rx, 0.0, rvx, rvy);
        state->asteroids.asteroid_num++;
    }
    state->new_asteroid_tick += 1;

    if (state->input.key_shoot && state->projectile_tick >= 30) {
        state->projectile_tick = 0;
        state->projectiles.projectiles[state->projectiles.num_projectiles] = create_projectile(state->ship, PROJECTILE_SPEED, PROJECTILE_DAMAGE);
        state->projectiles.num_projectiles++;
    }
    state->projectile_tick += 1;

    // --- Rendu ---
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255); // noir
    SDL_RenderClear(state->renderer);
    display_asteroid_array(state->renderer, &state->asteroids);
    display_projectile_array(&state->projectiles, state->renderer);
    if (state->ship->invincibility_frames % 10 < 5){
        display_polygon(state->renderer, state->ship->coord_x, state->ship->coord_y, state->ship->points, state->ship->point_num);
    }
    update_ui(state->renderer, state->ship, state->score, state->score_value, state->health_value);

    /* Draw the text */
    TTF_DrawRendererText(state->score_label, WINDOW_WIDTH/10.0, WINDOW_HEIGHT - WINDOW_HEIGHT/10.0);
    TTF_DrawRendererText(state->score_value, WINDOW_WIDTH/4.0, WINDOW_HEIGHT - WINDOW_HEIGHT/10.0);
    TTF_DrawRendererText(state->health_label, WINDOW_WIDTH - 200.0f, WINDOW_HEIGHT - WINDOW_HEIGHT/10.0);
    TTF_DrawRendererText(state->health_value, WINDOW_WIDTH - 100.0f, WINDOW_HEIGHT - WINDOW_HEIGHT/10.0);

    SDL_RenderPresent(state->renderer);
    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    app_state *state = (app_state *)appstate;
    (void)result;
    free_spaceship(state->ship);
    SDL_DestroyRenderer(state->renderer);
    SDL_DestroyWindow(state->window);
    if (state->font) {
        TTF_CloseFont(state->font);
    }
    free(state);
    TTF_Quit();
    SDL_Quit();
}
