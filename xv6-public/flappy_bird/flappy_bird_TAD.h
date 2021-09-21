#ifndef flappy_bird_TAD_H
#define flappy_bird_TAD_H

#include "../types.h"

#include "VGA_graphics.h"


typedef int width;
typedef int height;

typedef float distance;
typedef float speed;
typedef float acceleration;


#define offset_tubes 60

#define width_tube 14
#define width_hole_tube 20


typedef struct _s_game_status
{
  // Posición del flappy
  distance flappy_pos_x;
  distance flappy_pos_y;

  // Velocidad del flappy
  speed vertical_speed;

  // Distancia en x hasta el borde de la pantalla del centro primer tubo
  x_coord first_tube_x;

  // Posiciones de en y de los centros de todos los tubos
  y_coord hole_tubes_y[VGA_graphic_width / offset_tubes + 1];

  bool is_alive;
}* game_status;


#endif // flappy_bird_TAD_H
