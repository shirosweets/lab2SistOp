#include "types.h"
#include "user.h"

#include "flappy_bird/flappy_bird_logic.h"
#include "flappy_bird/flappy_bird_graphics.h"
#include "flappy_bird/flappy_bird_TAD.h"

#include "flappy_bird/random.h"

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

  game->flappy_pos_x = 15;
  game->flappy_pos_y = 100;

  uchar* buffer = malloc(VGA_graphic_hight*VGA_graphic_width);

  draw_game(game, buffer);

  VGA_plot_screen(buffer);

  exit();
}



