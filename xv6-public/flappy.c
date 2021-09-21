#include "types.h"
#include "user.h"

#include "flappy_bird/flappy_bird_logic.c"
#include "flappy_bird/flappy_bird_graphics.c"
#include "flappy_bird/flappy_bird_TAD.h"


int main(void)
{
//  game_status game = malloc(sizeof(struct _s_game_status));

  uchar* buffer = malloc(VGA_graphic_hight*VGA_graphic_width);

//  draw_pixel(buffer, 0, 0, 0);
  draw_sky(buffer);
  draw_clouds(buffer);
  draw_bush(buffer);
  draw_ground(buffer);

  VGA_mode_switch(VGA_mode_graphic);

  VGA_plot_screen(buffer);

  exit();
}



