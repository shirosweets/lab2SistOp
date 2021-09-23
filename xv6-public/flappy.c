#include "types.h"
#include "user.h"

#include "flappy_bird/flappy_bird_logic.h"
#include "flappy_bird/flappy_bird_graphics.h"
#include "flappy_bird/flappy_bird_TAD.h"

#include "flappy_bird/random.h"

int main(void)
{
  VGA_mode_switch(VGA_mode_graphic);
  uchar* buffer = malloc(VGA_graphic_height*VGA_graphic_width);
  game_status* game = new_game(3);

  draw_rectangle(buffer, 0, VGA_graphic_width, 0, VGA_graphic_height, 0);
  draw_game(game, buffer);
  VGA_plot_screen(buffer);

  char c;
  int last_time = uptime();
  while(game->is_alive){

    int new_time = uptime();

    bool jump = stdin_ready(&c);

    update_game(jump, new_time - last_time, game);

    draw_rectangle(buffer, 0, VGA_graphic_width, 0, VGA_graphic_height, 0);
    draw_game(game, buffer);
    VGA_plot_screen(buffer);

    last_time = new_time;
  }

  VGA_mode_switch(VGA_mode_text);

  exit();
}
