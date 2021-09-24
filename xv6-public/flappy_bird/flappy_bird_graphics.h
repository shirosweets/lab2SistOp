#ifndef FLAPPY_BIRD_GRAPHICS_H
#define FLAPPY_BIRD_GRAPHICS_H

#include "../types.h"
#include "flappy_bird_TAD.h"
#include "flappy_bird_graphics.c"

// buffer de tamaño VGA_graphic_width*VGA_graphic_hight
void draw_game(const game_status* game, uchar* buffer);

#endif // FLAPPY_BIRD_GRAPHICS_H
