#include "random.h"
#include "../const.h"
#include "flappy_bird_TAD.h"

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
            || res + width_hole_tube/2 + min_distance_to_borden >= VGA_graphic_height);
  return(res);
}


game_status*
new_game(int seed)
{
  game_status* game = malloc(sizeof(game_status));
  if(game == NULL)
    return NULL;

  game->flappy_pos_x = 20;
  game->flappy_pos_y = VGA_graphic_height/2;

  game->vertical_speed = 0;

  global_seed = seed;
  game->first_tube_x = 50;

  game->hole_tubes_y[0] = VGA_graphic_height/2;
  for(uint j = 1u; j < amount_of_tubes; j++){
    game->hole_tubes_y[j] = new_hole_tube_y_pos(game->hole_tubes_y[j-1]);
  }

  game->is_alive = true;

  return(game);
}

/* Actualiza las posiciones de los tubos y del flappy
 * No crea ni destruye los tubos
 */
static void
update_positions(bool jump, int delta_time, game_status* game)
{
  float delta_time_secunds = ((float)delta_time)/100;

  game->vertical_speed = jump ? jump_speed
                              : game->vertical_speed - gravity * delta_time_secunds;

  game->flappy_pos_y = game->flappy_pos_y + game->vertical_speed * delta_time_secunds;

  game->first_tube_x = game->first_tube_x + horizontal_speed * delta_time_secunds;
}

/* Si hay un tubo que ya se dejo de ver lo elimina y agrega uno nuevo
 * en a la derecha
 */
static void
update_tubes(game_status* game)
{
  if(game->first_tube_x < (float)-width_tube/2){
    // Si el primer tubo ya está afuera de la pantalla

    for(uint i = 0; i + 1 < amount_of_tubes; i++){
      game->hole_tubes_y[i] = game->hole_tubes_y[i+1];
    }
    game->hole_tubes_y[amount_of_tubes-1] =
      new_hole_tube_y_pos(game->hole_tubes_y[amount_of_tubes-2]);

    game->first_tube_x = game->first_tube_x + offset_tubes;
  }
}

/* Dice si hay alguna colisión entre el flappy y los tubos
 * asumiendo que el flappy es cuadrado de tamaño 2*flappy_radius
 */
static bool
has_collitions(const game_status* game)
{
  distance left_part_flappy = game->flappy_pos_x;
  distance right_part_flappy = left_part_flappy + 2*flappy_radius;
  distance bottom_part_flappy = game->flappy_pos_y;
  distance top_part_flappy = bottom_part_flappy - 2*flappy_radius;

  bool collitions = top_part_flappy < 0 || bottom_part_flappy >= VGA_graphic_height;

  /* Primero se busca si hay un tubo que coincide con el flappy
     en el eje x */
  uint i = 0;

  distance center_tube = game->first_tube_x;
  distance left_side_tube = center_tube - width_tube/2;
  distance right_side_tube = center_tube + width_tube/2;

  while(!collitions && right_part_flappy >= left_side_tube){
    /* Una vez que el tubo está más a la derecha que flappy es claro que
       ese tubo, ni los que siguen tienen colisiones con el flappy */

    // Estas asignaciones no necesitarian ejecutarse la primera vez, pero si las demás
    center_tube = game->first_tube_x + ((float)i)*offset_tubes;
    left_side_tube = center_tube - width_tube/2;
    right_side_tube = center_tube + width_tube/2;

    distance center_hole_tube = game->hole_tubes_y[i];
    distance bottom_hole_tube = center_hole_tube + width_hole_tube/2;
    distance top_hole_tube = center_hole_tube - width_hole_tube/2;

    // Miro si hay colisión con el tubo i
    collitions =
      // Primero veo si está por dentro del tubo actual en x
      (left_part_flappy < right_side_tube
        && right_part_flappy > left_side_tube
      )
      && // Si está por dentro del tubo actual en x, veo si no está en el hueco
      (top_part_flappy < top_hole_tube
        || bottom_part_flappy > bottom_hole_tube
      )
      ;

    i++;
  }

  return(collitions);
}

void
update_game(bool jump, int delta_time, game_status* game)
{
  update_positions(jump, delta_time, game);
  update_tubes(game);
  game->is_alive = game->is_alive && !has_collitions(game);
}
