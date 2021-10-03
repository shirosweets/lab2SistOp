#ifndef flappy_bird_TAD_H
#define flappy_bird_TAD_H

#include "VGA_graphics.h"

#define VGA_graphic_width 320
#define VGA_graphic_height 200

typedef int width;
typedef int height;

typedef float distance;     // pixels
typedef float speed;        // pixels/seconds


#define offset_tubes 72
#define amount_of_tubes VGA_graphic_width/offset_tubes + 1

#define width_tube 20
#define width_hole_tube 70

#define max_diff_hight_tubes 50
#define min_distance_to_border 10

#define flappy_radius 8

//#define horizontal_speed -80
#define jump_speed -100
#define gravity -220
#define limit_horizontal_speed -200
// jump_speed y gravity son negativos porque el eje 'y' crece hacia abajo

#define ground_height 190

#define start_first_tube_x VGA_graphic_width/3
#define start_flappy_pos_x 20

typedef struct _s_game_status
{
  // Posición del flappy
  distance flappy_pos_x;
  distance flappy_pos_y;

  // Velocidad vertical del flappy
  speed vertical_speed;

  // Velocidad horizontal del flappy
  speed horizontal_speed;

  // Valor que se suma al puntaje respecto a la velocidad horizonal del flappy
  int score_increment;

  // Cantidad de tubos que el jugador logró atravezar
  int tubes_passed;

  // TODO Mejorar: Ya computó el puntaje del tubo actual
  bool score_threshold;

  // Puntaje actual
  int current_score;

  // Distancia en x hasta el borde de la pantalla del centro primer tubo
  distance first_tube_x;

  // Posiciones de en y de los centros de todos los tubos
  y_coord hole_tubes_y[amount_of_tubes];

  bool is_alive;
} game_status;


game_status game;

#endif // flappy_bird_TAD_H
