#ifndef flappy_bird_TAD_H
#define flappy_bird_TAD_H

#include "../types.h"

#include "VGA_graphics.h"


typedef int width;
typedef int height;

typedef float distance;     // pixels
typedef float speed;        // pixels/secunds
typedef float acceleration; // pixels/secunds²


#define offset_tubes 72
#define amount_of_tubes VGA_graphic_width/offset_tubes + 1

#define width_tube 20
#define width_hole_tube 70

#define max_diff_hight_tubes 50
#define min_distance_to_borden 10

#define flappy_radius 8

#define horizontal_speed -80
#define jump_speed -100
#define gravity -220
// jump_speed y gravity son negativos porque el eje 'y' crece hacia abajo

#define ground_height 190

typedef struct _s_game_status
{
  // Posición del flappy
  distance flappy_pos_x;
  distance flappy_pos_y;

  // Velocidad del flappy
  speed vertical_speed;

  // Distancia en x hasta el borde de la pantalla del centro primer tubo
  distance first_tube_x;

  // Posiciones de en y de los centros de todos los tubos
  y_coord hole_tubes_y[amount_of_tubes];

  bool is_alive;
} game_status;


#endif // flappy_bird_TAD_H
