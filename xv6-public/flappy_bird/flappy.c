#include "../types.h"
#include "../user.h"

#include "VGA_graphics.h"
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
 * La semilla es el primer parametro, si no hay se obtiene una
 * seudo-aleatoriamente con uptime
 * Cuando hay lo que se toma es el valor numerico de los primeros 4 caracteres
 */
static int
get_seed(int argc, char *argv[])
{
  int res = 0;
  if(argc >= 2)
    res = string_to_int(argv[1]);
  else{
    res = uptime();
  }
  return(res);
}


int
main(int argc, char *argv[])
{
  bool quit = false;

  uchar* buffer = malloc(VGA_graphic_height*VGA_graphic_width);
  if(buffer == NULL){
    printf(2, "Memory error");
    exit();
  }

  while(!quit){
    VGA_mode_switch(VGA_graphic_320x200x256);

    int seed = get_seed(argc, argv);
    init_game(seed);

    draw_game(buffer);
    VGA_plot_screen(buffer);

    char c = '\0';
    int last_time = uptime();

    while(game.is_alive){
      int new_time = uptime();

      bool jump = stdin_ready(&c);

      // '\e' = tecla esc
      // 4 = fin de archivo (contrl + d)
      if(c == '\e' || c == 4){
        quit = true;
        break;
      }

      update_game(jump, new_time - last_time);

      draw_game(buffer);
      VGA_plot_screen(buffer);

      last_time = new_time;
    }
    VGA_mode_switch(VGA_text_80x25);

    if(!quit){
      printf(1, "\nPresiona escape para salir del juego, o cualquier otra tecla para jugar de nuevo\n");
      while(!stdin_ready(&c)){
        sleep(30);
      }
      quit = (c == '\e' || c == 4);
    }

  }

  free(buffer); buffer = NULL;


  exit();
}
