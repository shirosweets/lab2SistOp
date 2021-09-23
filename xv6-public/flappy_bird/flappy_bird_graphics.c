#include "../types.h"

#include "flappy_bird_TAD.h"

#define clouds_height 100
#define bush_height 150

#define sky_color 0x4E
#define clouds_color 0x66
#define bush_color 0x32
#define ground_color 0x06
#define tubes_color 0x02
#define tubes1_color 0xbe
#define tubes2_color 0x90
#define tubes3_color 0x02
#define line_color 0x0F
#define flappy_eye_color 0x0F
#define flappy_pupil_color 0x00
#define flappy_wing_color 0x2C
#define flappy_mouth_color 0x28
#define tubes_shadow_color 0x00
#define flappy_color 0x00

/* Dibuja un tubo con el centro del hueco en (x, y)
 */
static void
draw_tube(uchar* buffer, x_coord x, y_coord y)
{
  x_coord left_side = x - width_tube/2;
  x_coord right_side = x + width_tube/2;
  y_coord bottom_part_hole = y + width_hole_tube/2;
  y_coord top_part_hole = y - width_hole_tube/2;

  // Tubo de arriba
  draw_rectangle(buffer, left_side-2, right_side+2, 0, top_part_hole+2, tubes_shadow_color);
  x_coord first_third = left_side + width_tube/3;
  draw_rectangle(buffer, left_side, first_third, 0, top_part_hole, tubes1_color);
  x_coord second_third = first_third + width_tube/3;
  draw_rectangle(buffer, first_third, second_third, 0, top_part_hole, tubes2_color);
  draw_rectangle(buffer, second_third, right_side, 0, top_part_hole, tubes3_color);
  draw_vertical_line(buffer, 0, top_part_hole, second_third+1, line_color);
  draw_rectangle(buffer, left_side-4, right_side+4, top_part_hole-5, top_part_hole+2, tubes_shadow_color);
  draw_rectangle(buffer, left_side-3, first_third-1, top_part_hole-4, top_part_hole+1, tubes1_color);
  draw_rectangle(buffer, first_third-1, second_third+3, top_part_hole-4, top_part_hole+1, tubes2_color);
  draw_vertical_line(buffer, top_part_hole-4, top_part_hole+1, second_third, line_color);
  draw_rectangle(buffer, second_third+1, right_side+3, top_part_hole-4, top_part_hole+1, tubes3_color);

  // Tubo de abajo
  draw_rectangle(buffer, left_side-2, right_side+2, bottom_part_hole-2, ground_height-1, tubes_shadow_color);
  draw_rectangle(buffer, left_side, first_third, bottom_part_hole, ground_height-1, tubes1_color);
  draw_rectangle(buffer, first_third, second_third, bottom_part_hole, ground_height-1, tubes2_color);
  draw_rectangle(buffer, second_third, right_side, bottom_part_hole, ground_height-1, tubes3_color);
  draw_vertical_line(buffer, bottom_part_hole, ground_height-1, second_third, line_color);
  draw_rectangle(buffer, left_side-4, right_side+4, bottom_part_hole-5, bottom_part_hole+2, tubes_shadow_color);
  draw_rectangle(buffer, left_side-3, first_third+1, bottom_part_hole-4, bottom_part_hole+1, tubes1_color);
  draw_rectangle(buffer, first_third-1, second_third+3, bottom_part_hole-4, bottom_part_hole+1, tubes2_color);
  draw_vertical_line(buffer, bottom_part_hole-4, bottom_part_hole+1, second_third+1, line_color);
  draw_rectangle(buffer, second_third+2, right_side+3, bottom_part_hole-4, bottom_part_hole+1, tubes3_color);}

/* Dibuja en buffer todos los tubos de game
 */
static void
draw_tubes(const game_status* game, uchar* buffer)
{
  for(uint i = 0u; i < amount_of_tubes; i++){
    draw_tube(buffer, ((int)game->first_tube_x) + i*offset_tubes, game->hole_tubes_y[i]);
  }
}


/* Dibuja el flappy con la parte inferior izquierda en (x, y)
 */
// Para mejorar
static void
draw_flappy(uchar* buffer, x_coord x, y_coord y)
{
  draw_circle(buffer, x + flappy_radius, y - flappy_radius, flappy_radius, flappy_color);
  draw_circle(buffer, x + (flappy_radius+7), y - flappy_radius - 3, flappy_radius/2, flappy_eye_color);
  draw_circle(buffer, x + (flappy_radius+7), y - flappy_radius - 3, flappy_radius/4, flappy_pupil_color);
  draw_circle(buffer, x + (flappy_radius-7), y - flappy_radius - 1, flappy_radius/2, flappy_wing_color);
  draw_circle(buffer, x + (flappy_radius-7), y - flappy_radius - 2, flappy_radius/2-1, flappy_eye_color);
  draw_circle(buffer, x + (flappy_radius+7), y - flappy_radius + 2, flappy_radius/3+1, flappy_mouth_color);
  draw_horizontal_line(buffer, x + flappy_radius, x + flappy_radius + 10, y - flappy_radius + 3, flappy_pupil_color);
}



static void
draw_sky(uchar* buffer)
{
  draw_rectangle(buffer, 0, VGA_graphic_width, 0, clouds_height, sky_color);
}

static void
draw_clouds(uchar* buffer)
{
  uint r = 10;
  for(x_coord i = 0; i < VGA_graphic_width; i += 30){
    if(r > 30){
      r = 10;
    }
    draw_circle(buffer, i, clouds_height, r, clouds_color);
    r += 5;
  }
  draw_rectangle(buffer, 0, VGA_graphic_width, clouds_height, bush_height, clouds_color);
}

static void
draw_bush(uchar* buffer)
{
  for(x_coord i = 0; i < VGA_graphic_width; i += 25){
    draw_circle(buffer, i, bush_height, 7, bush_color);
  }
  for(x_coord i = 20; i < VGA_graphic_width; i += 10){
    draw_circle(buffer, i, bush_height, 4, bush_color);
  }
  draw_rectangle(buffer, 0, VGA_graphic_width, bush_height, ground_height, bush_color);
}

static void
draw_ground(uchar* buffer)
{
  draw_rectangle(buffer, 0, VGA_graphic_width, ground_height, VGA_graphic_height, ground_color);
} 

static void
draw_background(uchar* buffer)
{
  draw_sky(buffer);
  draw_clouds(buffer);
  draw_bush(buffer);
  draw_ground(buffer);
}



// buffer de tamaño VGA_graphic_width*VGA_graphic_height
void
draw_game(const game_status* game, uchar* buffer)
{
  draw_background(buffer);
  draw_tubes(game, buffer);
  draw_flappy(buffer, (int)game->flappy_pos_x, (int)game->flappy_pos_y);
}
