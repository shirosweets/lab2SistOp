#include "../types.h"
#include "../user.h"

#include "random.h"
#include "flappy_bird_TAD.h"

/* Toma la altura del hueco del tubo anterior, y devuelve la del siguiente
 */
static y_coord
new_hole_tube_y_pos(y_coord y)
{
  y_coord res;
  do{
    int rand = new_random_less_than(max_diff_hight_tubes);
    res = y + rand;
    // Genero nuevas alturas hasta que haya una que esté en el rango correcto
  } while(
    res - width_hole_tube/2 - min_distance_to_border < 0
      || res + width_hole_tube/2 + min_distance_to_border >= ground_height
  );
  return(res);
}


void
init_game(int seed)
{
  game.flappy_pos_x = start_flappy_pos_x;
  game.flappy_pos_y = ground_height/2 + flappy_radius;

  game.vertical_speed = 0;

  game.horizontal_speed = -55; // -80;

  game.score_threshold = false;

  game.score_increment = 100;

  game.tubes_passed = 0;

  game.current_score = 0;

  global_seed = seed;
  game.first_tube_x = start_first_tube_x;

  game.hole_tubes_y[0] = ground_height/2;
  for(uint j = 1u; j < amount_of_tubes; j++){
    game.hole_tubes_y[j] = new_hole_tube_y_pos(game.hole_tubes_y[j-1]);
  }

  game.is_alive = true;
}

/* Actualiza las posiciones de los tubos y del flappy
 * No crea ni destruye los tubos
 */
static void
update_positions(bool jump, int delta_time)
{
  float delta_time_seconds = ((float)delta_time)/100;

  game.vertical_speed =
    jump ? jump_speed
         : game.vertical_speed - gravity * delta_time_seconds;

  game.flappy_pos_y =
    game.flappy_pos_y + game.vertical_speed * delta_time_seconds;

  game.first_tube_x = game.first_tube_x + game.horizontal_speed * delta_time_secunds;
}

static void
update_score(void)
{
  int distance_flappy_tube = game.first_tube_x - game.flappy_pos_x;
  if(distance_flappy_tube < 0 && !game.score_threshold){
    game.current_score += game.score_increment;
    game.score_threshold = true;
    game.tubes_passed += 1;
    update_speed();
  }
}

static void
update_speed(void)
{
  // Incrementa la ganancia de puntos
  if(
    game.tubes_passed % 5 == 0) {
    game.score_increment += 50;
  }

  // Incrementa la velocidad horizontal
  if(game.tubes_passed % 6 == 0 &&
    game.horizontal_speed >= limit_horizontal_speed) {
    game.horizontal_speed -= 10;
  }
}

/* Si hay un tubo que ya se dejó de ver lo elimina
 * y agrega uno nuevo a la derecha
 */
static void
update_tubes(void)
{
  if(game.first_tube_x < (float)-width_tube/2){
    // Si el primer tubo ya está afuera de la pantalla
    for(uint i = 0; i + 1 < amount_of_tubes; i++){
      game.hole_tubes_y[i] = game.hole_tubes_y[i+1];
    }

    game.hole_tubes_y[amount_of_tubes-1] =
      new_hole_tube_y_pos(game.hole_tubes_y[amount_of_tubes-2]);

    game.first_tube_x = game.first_tube_x + offset_tubes;
    game.score_threshold = false;
  }
}

/* Dice si hay alguna colisión entre el flappy y los tubos
 * asumiendo que el flappy es cuadrado de tamaño 2*flappy_radius
 */
static bool
has_collitions(void)
{
  distance left_part_flappy = game.flappy_pos_x;
  distance right_part_flappy = left_part_flappy + 2*flappy_radius;
  distance bottom_part_flappy = game.flappy_pos_y;
  distance top_part_flappy = bottom_part_flappy - 2*flappy_radius;

  bool collitions = top_part_flappy < 0 || bottom_part_flappy >= ground_height;

  /* Primero se busca si hay un tubo que coincide con el flappy
     en el eje x */
  uint i = 0;

  distance center_tube = game.first_tube_x;
  distance left_side_tube = center_tube - width_tube/2;
  distance right_side_tube = center_tube + width_tube/2;

  while(!collitions && right_part_flappy >= left_side_tube){
    /* Una vez que el tubo está más a la derecha que flappy es claro que
       ese tubo, ni los que siguen tienen colisiones con el flappy */

    // Estas asignaciones no necesitarían ejecutarse la primera vez, pero si las demás
    center_tube = game.first_tube_x + ((float)i)*offset_tubes;
    left_side_tube = center_tube - width_tube/2;
    right_side_tube = center_tube + width_tube/2;

    distance center_hole_tube = game.hole_tubes_y[i];
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
update_game(bool jump, int delta_time)
{
  update_positions(jump, delta_time);
  update_score();
  update_tubes();
  game.is_alive = game.is_alive && !has_collitions();
}
