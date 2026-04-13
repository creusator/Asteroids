#ifndef INPUT_H
#define INPUT_H

#include <SDL3/SDL.h>

typedef struct {
    bool key_left, key_right, key_up, key_down; //Direction key
    bool key_shoot; //Shoot key
} input_state;

#endif
