#ifndef flappy_bird_TAD_H
#define flappy_bird_TAD_H

#include "../types.h"

#include "VGA_graphics.h"


typedef int width;
typedef int height;

typedef float distance;
typedef float speed;
typedef float acceleration;


#define offset_tubes 72
#define amount_of_tubes VGA_graphic_width/offset_tubes + 1

#define width_tube 14
#define width_hole_tube 54

#define max_diff_hight_tubes 50
#define min_distance_to_borden 10

#define flappy_radius 8

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
  y_coord hole_tubes_y[amount_of_tubes];

  bool is_alive;
} game_status;


#endif // flappy_bird_TAD_H
