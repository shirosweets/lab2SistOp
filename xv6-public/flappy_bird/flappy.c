#include "../types.h"
#include "../user.h"

#include "VGA_graphics.h"
#include "flappy_bird_logic.h"
#include "flappy_bird_graphics.h"
#include "flappy_bird_TAD.h"

/* Convierte un string en un int según el valor numérico de los
 * primeros 4 caracteres
 * Si str es NULL retorna 0
 */
static int
string_to_int(char* str)
{
  int res = 0;
  if(str != NULL)
    for(uint i = 0; str[i] != '\0' && i < 4; i++){
      res = (res << 8) | (int)str[i];
    }

  return(res);
}

/* Obtiene la semilla de los parámetros de el programa
 * La semilla es el primer parámetro, si no hay se obtiene una
 * pseudo-aleatoriamente con uptime
 * Cuando hay lo que se toma es el valor numérico de los primeros 4 caracteres
 */
static int
get_seed(int argc, char *argv[])
{
  int res = 0;
  if(argc >= 2)
    res = string_to_int(argv[1]);
  else
    res = uptime();
  return(res);
}

/* Ejecuta el ciclo principal del juego
 * Toma como parámetro la semilla
 * El valor que devuelve dice si hay que cerrar el programa o no
 */
static bool
game_loop(int seed)
{
  bool quit = false;
  uchar* buffer = malloc(VGA_graphic_height*VGA_graphic_width);
  if(buffer == NULL){
    printf(2, "Memory error");
    quit = true;
    return(quit);
  }

  VGA_mode_switch(VGA_graphic_320x200x256);

  init_game(seed);

  draw_game(buffer);
  VGA_plot_screen(buffer);

  char c = '\0';
  int start_time = uptime();
  int last_time = start_time;

  while(!quit && game.is_alive){
    int new_time = uptime();

    bool jump = stdin_ready(&c);

    // '\e' = tecla esc
    // 4 = fin de archivo (contrl + d)
    quit = (c == '\e' || c == 4);

    update_game(jump, new_time - last_time);

    draw_game(buffer);
    VGA_plot_screen(buffer);

    last_time = new_time;
  }
  VGA_mode_switch(VGA_text_80x25);
  free(buffer); buffer = NULL;

  int running_time_seconds = (last_time - start_time)/100;
  /* Como siempre se avanza a la misma velocidad,
     el puntaje se relaciona linealmente con el tiempo */
  int score =
    (-horizontal_speed*running_time_seconds -
      start_first_tube_x - start_flappy_pos_x
    )/offset_tubes + 1;

  printf(1, "Puntaje final: %d\n", score);

  return(quit);
}

int
main(int argc, char *argv[])
{
  bool quit = false;

  while(!quit){
    int seed = get_seed(argc, argv);
    quit = game_loop(seed);

    if(!quit){
      char c = '\0';
      printf(1,
        "Presiona esc para salir del juego, o cualquier otra tecla para jugar de nuevo\n"
      );
      while(!stdin_ready(&c)){
        sleep(30);
      }
      quit = (c == '\e' || c == 4);
    }
  }

  exit();
}
