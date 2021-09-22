#ifndef FLAPPY_BIRD_LOGIC_H
#define FLAPPY_BIRD_LOGIC_H

#include "flappy_bird_TAD.h"

/* Crea e inicializa un nuevo game_status
 * Pide memoria con malloc que debe ser liberada por el llamador
 * En caso de error de alocado de memoria devuelve NULL
 */
game_status* new_game(int seed);


void update_game(bool jump, int delta_time, game_status* game);

#include "flappy_bird_logic.c"

#endif // FLAPPY_BIRD_LOGIC_H
