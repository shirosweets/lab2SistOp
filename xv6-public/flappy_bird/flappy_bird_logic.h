#ifndef FLAPPY_BIRD_LOGIC_H
#define FLAPPY_BIRD_LOGIC_H

#include "flappy_bird_TAD.h"

game_status* new_game(void);
void update_game(bool jump, int delta_time, game_status* game);

#include "flappy_bird_logic.c"

#endif // FLAPPY_BIRD_LOGIC_H
