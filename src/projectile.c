#include "projectile.h"

projectile create_projectile(spaceship *ship, float speed, int damage, int index){
    projectile proj;
    proj.coord_x = ship->coord_x;
    proj.coord_y = ship->coord_y;
    proj.speed = speed;
    proj.damage = damage;
    proj.index = index;
    return proj;
}

void update_projectile_array(projectile_array *array){
    for (int i = 0; i < array->num_projectiles; i++) {
        projectile proj = array->projectiles[i];
        if (proj.coord_y < 0) {
            destroy_projectile(array, i);
            i--;
        } else {
            proj.coord_y -= proj.speed;
            array->projectiles[i] = proj;
        }
    }
}

void display_projectile_array(projectile_array *array, SDL_Renderer *renderer){
    for (int i = 0; i < array->num_projectiles; i++) {
        projectile proj = array->projectiles[i];
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //blanc
        SDL_RenderLine(renderer, proj.coord_x, proj.coord_y, proj.coord_x, proj.coord_y + proj.speed);
    }
}

void destroy_projectile(projectile_array *array, int index) {
    for (int i = index; i < array->num_projectiles - 1; i++) {
        array->projectiles[i] = array->projectiles[i + 1];
    }
    array->num_projectiles--;
}
