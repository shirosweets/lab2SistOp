#ifndef FLAPPY_BIRD_LOGIC_H
#define FLAPPY_BIRD_LOGIC_H

#include "flappy_bird_TAD.h"

/* Inicializa game
 */
void init_game(int seed);

/* Actualiza game según si tiene que saltar o no
 * delta_time son la cantidad de centesimas de segundo desde la última
 * llamada a update_game, y se usa para que el juego vaya siempre a la
 * misma velocidad sin importar que tan rapido se ejecuta
 */
void update_game(bool jump, int delta_time);


#endif // FLAPPY_BIRD_LOGIC_H
