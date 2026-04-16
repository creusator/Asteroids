#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "spaceship.h"
#include <SDL3_ttf/SDL_ttf.h>

void update_ui(SDL_Renderer *renderer, spaceship *ship, int score, TTF_Text *score_label, TTF_Text *health_label);

#endif
