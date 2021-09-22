#include "types.h"
#include "user.h"

#include "flappy_bird/flappy_bird_logic.h"
#include "flappy_bird/flappy_bird_graphics.h"
#include "flappy_bird/flappy_bird_TAD.h"

#include "flappy_bird/random.h"

int main(void)
{
  VGA_mode_switch(VGA_mode_graphic);

  game_status* game = new_game(3);

  uchar* buffer = malloc(VGA_graphic_hight*VGA_graphic_width);

  draw_game(game, buffer);

  VGA_plot_screen(buffer);

  exit();
}



