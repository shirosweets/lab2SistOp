#include "types.h"
#include "user.h"

#include "flappy_bird/flappy_bird_logic.c"
#include "flappy_bird/flappy_bird_graphics.c"
#include "flappy_bird/flappy_bird_TAD.h"


int main(void)
{
  VGA_mode_switch(VGA_mode_graphic);

  game_status* game = malloc(sizeof(game_status));

  game->hole_tubes_y[0] = 100;
  game->hole_tubes_y[1] = 100;
  game->hole_tubes_y[2] = 100;
  game->hole_tubes_y[3] = 100;
  game->hole_tubes_y[4] = 100;

  game->first_tube_x = 20;

  uchar* buffer = malloc(VGA_graphic_hight*VGA_graphic_width);

  draw_sky(buffer);
  draw_clouds(buffer);
  draw_bush(buffer);
  draw_ground(buffer);

  draw_tubes(game, buffer);


  VGA_plot_screen(buffer);

  exit();
}



