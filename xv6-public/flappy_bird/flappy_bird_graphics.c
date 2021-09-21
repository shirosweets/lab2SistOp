#include "flappy_bird_TAD.h"

#define clouds_height 100
#define bush_height 150
#define ground_height 180
#define sky_color 0x0B
#define clouds_color 0x0F
#define bush_color 0x0A
#define ground_color 0x06
#define tubes_color 0x02

/* Dibuja un tubo con el centro del hueco en (x, y)
 */
/*
static void
draw_tube(uchar* buffer, x_coord x, y_coord y)
{
  x_coord left_side = x - width_tube/2;
  x_coord right_side = x + width_tube/2;
  y_coord bottom_part_hole = y + width_hole_tube/2;
  y_coord top_part_hole = y - width_hole_tube/2;

  // Tubo de arriba
  draw_rectangle(buffer, left_side, right_side, 0, top_part_hole, tubes_color);

  // Tubo de anajo
  draw_rectangle(buffer, left_side, right_side, bottom_part_hole, VGA_graphic_hight, tubes_color);
}*/

/*
 *
 */
//static




static void draw_sky(uchar* buffer)
{
  draw_rectangle(buffer, 0, VGA_graphic_width, 0, clouds_height, sky_color);
}

static void draw_clouds(uchar* buffer)
{
  int r = 10;
  for(int i = 0; i < VGA_graphic_width; i += 20){
    if(r > 30){
      r = 10;
    }
    draw_circle(buffer, i, clouds_height, r, clouds_color);
    r += 5;
  }
  draw_rectangle(buffer, 0, VGA_graphic_width, clouds_height, bush_height, clouds_color);
}

static void draw_bush(uchar* buffer)
{
 for(int i = 0; i < VGA_graphic_width; i += 20){
   draw_circle(buffer, i, bush_height, 20, bush_color);
 }
 for(int i = 20; i < VGA_graphic_width; i += 10){
   draw_circle(buffer, i, bush_height, 10, bush_color);
 }
 draw_rectangle(buffer, 0, VGA_graphic_width, bush_height, ground_height, bush_color);
}

static void draw_ground(uchar* buffer)
{
  draw_rectangle(buffer, 0, VGA_graphic_width, ground_height, VGA_graphic_hight, ground_color);
}
/*
// buffer de tamaño VGA_graphic_width*VGA_graphic_hight
void draw_game(const game_status game, uchar* buffer)
{

}
*/