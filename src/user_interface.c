#include "user_interface.h"
#include "renderer.h"
#include "constants.h"
#include <SDL3_ttf/SDL_ttf.h>

void update_ui(SDL_Renderer *renderer, spaceship *ship, int score, TTF_Text *score_label, TTF_Text *health_label){
    float ui_rectangle[8] = {0.0, 0.0, WINDOW_WIDTH-1, 0.0, WINDOW_WIDTH-1, 150.0, 0.0, 150.0};
    display_polygon(renderer, 0.0, WINDOW_HEIGHT-151.0, ui_rectangle, 8);

    char score_str[32];
    SDL_snprintf(score_str, sizeof(score_str), "%d", score);
    TTF_SetTextString(score_label, score_str, 0);

    char health_str[32];
    SDL_snprintf(health_str, sizeof(health_str), "%d", ship->health);
    TTF_SetTextString(health_label, health_str, 0);
}
