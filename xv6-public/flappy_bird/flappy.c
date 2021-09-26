#include "../types.h"
#include "../user.h"

#include "flappy_bird_logic.h"
#include "flappy_bird_graphics.h"
#include "flappy_bird_TAD.h"

/* Convierte un string en un int según el valor numerico de los
 * primeros 4 caracteres
 * Si str es NULL retorna 0
 */
static int
string_to_int(char* str)
{
  int res = 0;
  if(str != NULL){
    for(uint i = 0; str[i] != '\0' && i < 4; i++){
      res = (res << 1) | (int)str[i];
    }
  }
  return(res);
}

/* Obtiene la semilla de los parametros de el programa
 * La semilla es el primer parametro, si no hay se retorna 0
 * Cuando hay lo que se toma es el valor numerico de los primeros 4 caracteres
 */
static int
get_seed(int argc, char *argv[])
{
  int res = 0;
  if(argc >= 2)
    res = string_to_int(argv[1]);
  return(res);
}


int
main(int argc, char *argv[])
{
  VGA_mode_switch(VGA_graphic_320x200x256);
  uchar* buffer = malloc(VGA_graphic_height*VGA_graphic_width);
  if(buffer == NULL){
    printf(2, "Memory error");
    exit();
  }

  int seed = get_seed(argc, argv);
  game_status* game = new_game(seed);
  if(game == NULL){
    printf(2, "Memory error");
    free(buffer);
    exit();
  }

  draw_game(game, buffer);
  VGA_plot_screen(buffer);

  char c = '\0';
  int last_time = uptime();
  while(game->is_alive){
    int new_time = uptime();

    bool jump = stdin_ready(&c);

    // '\e' = tecla esc
    // 4 = fin de archivo (contrl + d)
    if(c == '\e' || c == 4)
      break;

    update_game(jump, new_time - last_time, game);

    draw_game(game, buffer);
    VGA_plot_screen(buffer);

    last_time = new_time;
  }
  free(game); game = NULL;
  free(buffer); buffer = NULL;

  VGA_mode_switch(VGA_text_80x25);

  exit();
}
