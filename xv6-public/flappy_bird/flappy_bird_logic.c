#include "random.h"

#include "flappy_bird_TAD.h"

#define NULL 0

/* Toma la altura del hueco del tubo anterior, y devuelve la del siguiente
 */
static y_coord
new_hole_tube_y_pos(y_coord y)
{
  int rand;
  y_coord res;
  do{
    rand = new_random_less_than(max_diff_hight_tubes);
    res = y + rand;
    // Genero nuevas alturas hasta que haya una que este en el rango correcto
  } while(res - width_hole_tube/2 - min_distance_to_borden < 0
            || res + width_hole_tube/2 + min_distance_to_borden >= VGA_graphic_hight);
  return(res);
}


game_status*
new_game(int seed)
{
  game_status* game = malloc(sizeof(game_status));
  if(game == NULL)
    return NULL;
  
  game->flappy_pos_x = 20;
  game->flappy_pos_y = VGA_graphic_hight/2;

  game->vertical_speed = 0;

  global_seed = seed;
  game->first_tube_x = 50;
  
  game->hole_tubes_y[0] = VGA_graphic_hight/2;
  for(uint j = 1u; j < amount_of_tubes; j++){
    game->hole_tubes_y[j] = new_hole_tube_y_pos(game->hole_tubes_y[j-1]);
  }
  
  game->is_alive = true;

  return(game);
}

/* Actualiza las posiciones de los tubos y del flappy
 * No crea ni destrulle los tubos
 */
static void
update_positons(bool jump, int delta_time, game_status* game)
{
  float delta_time_secunds = ((float)delta_time)/100;

/*   game->vertical_speed = jump ? jump_speed
                              : game->vertical_speed + gravity * delta_time_secunds;

  game->flappy_pos_y = game->flappy_pos_y + game->vertical_speed * delta_time_secunds; */

  game->first_tube_x = game->first_tube_x + horizontal_speed * delta_time_secunds;
}


void
update_game(bool jump, int delta_time, game_status* game)
{
  update_positons(jump, delta_time, game);
}
