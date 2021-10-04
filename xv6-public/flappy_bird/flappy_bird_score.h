#ifndef FLAPPY_BIRD_SCORE_H
#define FLAPPY_BIRD_SCORE_H

#include "../types.h"

/*
 * Dibuja en pantalla el score actual hasta 10
 * casillas. Las casillas están enumeradas desde
 * 0 hasta 9. La casilla 0 es la casilla correspondiente
 * a la unidad (primer casilla de la derecha).
*/
void
draw_score(uchar* buffer, int score);

#endif