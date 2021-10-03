#include "../types.h"

#include "VGA_graphics.h"
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
#define score_background_color 0x00
#define score_digit_color 0x28

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
  // Borde negro
  draw_rectangle(
    buffer,
    left_side-2, right_side+2,
    0, top_part_hole+2,
    tubes_shadow_color);
  // Degradado
  x_coord first_third = left_side + width_tube/3;

  draw_rectangle(
    buffer,
    left_side, first_third,
    0, top_part_hole,
    tubes1_color);
  x_coord second_third = first_third + width_tube/3;

  draw_rectangle(
    buffer,
    first_third, second_third,
    0, top_part_hole,
    tubes2_color);

  draw_rectangle(
    buffer,
    second_third, right_side,
    0, top_part_hole,
    tubes3_color);

  draw_vertical_line(
    buffer,
    0, top_part_hole, second_third+1,
    line_color);

  // Boca del tubo
  // Borde negro
  draw_rectangle(
    buffer,
    left_side-4, right_side+4,
    top_part_hole-5, top_part_hole+2,
    tubes_shadow_color);

  // Degradado
  draw_rectangle(
    buffer,
    left_side-3, first_third-1,
    top_part_hole-4, top_part_hole+1,
    tubes1_color);

  draw_rectangle(
    buffer,
    first_third-1, second_third+3,
    top_part_hole-4, top_part_hole+1,
    tubes2_color);

  draw_vertical_line(
    buffer,
    top_part_hole-4, top_part_hole+1, second_third,
    line_color);

  draw_rectangle(
    buffer,
    second_third+1, right_side+3,
    top_part_hole-4, top_part_hole+1,
    tubes3_color);

  // Tubo de abajo
  // Borde negro
  draw_rectangle(
    buffer,
    left_side-2, right_side+2,
    bottom_part_hole-2, ground_height-1,
    tubes_shadow_color);

  // Degradado
  draw_rectangle(
    buffer,
    left_side, first_third,
    bottom_part_hole, ground_height-1,
    tubes1_color);

  draw_rectangle(
    buffer,
    first_third, second_third,
    bottom_part_hole, ground_height-1,
    tubes2_color);

  draw_rectangle(
    buffer,
    second_third, right_side,
    bottom_part_hole, ground_height-1,
    tubes3_color);

  draw_vertical_line(
    buffer,
    bottom_part_hole, ground_height-1, second_third,
    line_color);

  // Boca del tubo
  // Borde negro
  draw_rectangle(
    buffer,
    left_side-4, right_side+4,
    bottom_part_hole-5, bottom_part_hole+2,
    tubes_shadow_color);

  // Degradado
  draw_rectangle(
    buffer,
    left_side-3, first_third+1,
    bottom_part_hole-4, bottom_part_hole+1,
    tubes1_color);

  draw_rectangle(
    buffer,
    first_third-1, second_third+3,
    bottom_part_hole-4, bottom_part_hole+1,
    tubes2_color);

  draw_vertical_line(
    buffer,
    bottom_part_hole-4, bottom_part_hole+1, second_third+1,
    line_color);

  draw_rectangle(
    buffer,
    second_third+2, right_side+3,
    bottom_part_hole-4, bottom_part_hole+1,
    tubes3_color);
}

/* Dibuja en buffer todos los tubos de game
 */
static void
draw_tubes(uchar* buffer)
{
  for(uint i = 0u; i < amount_of_tubes; i++){
    draw_tube(
      buffer, ((int)game.first_tube_x) + i*offset_tubes, game.hole_tubes_y[i]);
  }
}


/* Dibuja el flappy con la parte inferior izquierda en (x, y)
 */
static void
draw_flappy(uchar* buffer, x_coord x, y_coord y)
{
  draw_circle(
    buffer,
    x + flappy_radius, y - flappy_radius,
    flappy_radius, flappy_color);

  draw_circle(
    buffer,
    x + (flappy_radius+7), y - flappy_radius - 3,
    flappy_radius/2, flappy_eye_color);

  draw_circle(
    buffer,
    x + (flappy_radius+7), y - flappy_radius - 3,
    flappy_radius/4, flappy_pupil_color);

  draw_circle(
    buffer,
    x + (flappy_radius-7), y - flappy_radius - 1,
    flappy_radius/2, flappy_wing_color);

  draw_circle(
    buffer,
    x + (flappy_radius-7), y - flappy_radius - 2,
    flappy_radius/2-1, flappy_eye_color);

  draw_circle(
    buffer,
    x + (flappy_radius+7), y - flappy_radius + 2,
    flappy_radius/3+1, flappy_mouth_color);

  draw_horizontal_line(
    buffer,
    x + flappy_radius, x + flappy_radius + 10,
    y - flappy_radius + 3, flappy_pupil_color);
}

/*
/
*/

// Configuración de cada número para dibujar en una casilla
struct _digit_cell{
  /* Cada celda representa una linea del gráfico de cada
   * de cada casilla de dígito. Distintas configuraciones
   * 'iluminan' cada celda, determinando distintos números
  */
  bool cells[7];
};

typedef struct _digit_cell digit_cell;

/// INICIO NUMEROS INDIVIDUALES

static void
set_digit_false(digit_cell number)
{
  for (uint i = 0; i < 7; ++i)
    number.cells[i] = false;
}

static void
set_digit_true(digit_cell number)
{
  for (uint i = 0; i < 7; ++i)
    number.cells[i] = true;
}

//
# define score_cell_line_px 4
// dbox = digit box: tamaño de la casilla donde se dibujar el dígito
#define score_dbox_height score_cell_line_px * 2 + 5
#define score_dbox_width score_cell_line_px + 4
//
#define score_total_digit_boxes 10
#define score_offset_height score_dbox_height
#define score_offset_width score_dbox_width * score_total_digit_boxes

#define score_box_w0_x VGA_graphic_width - 50 // FIXME
#define score_box_w0_y VGA_graphic_height - 50 // FIXME

/*
 * Toma la configuración de qué celdas pintar de la variable number
 * y cuál casilla pintar de la variable position.
 *
 * Pinta las líneas respecto al dígito que deba ser.
*/
static void
draw_digit(uchar* buffer, uint position, digit_cell number)
{
  // w0: primer pixel (inferior derecho) de la casilla actual
  uint w0 = score_box_w0_x - score_dbox_width * position;
  uint x0, x1, y0, y1;

  set_digit_true(number);

  if(number.cells[0]){
    // Dibujamos celda 0
    x0 = w0 - score_dbox_width + 1;
    x1 = x0;
    y0 = score_box_w0_y - score_dbox_height + 2;
    y1 = y0 + score_cell_line_px;
    draw_rectangle(buffer, x0, x1, y0, y1, score_digit_color);
  }

  if(number.cells[1]){
    // Dibujamos celda 1
    x0 = w0 - score_dbox_width + 2;
    x1 = x0 + score_cell_line_px;
    y0 = score_box_w0_y - score_dbox_height + 1;
    y1 = y0;
    draw_rectangle(buffer, x0, x1, y0, y1, bush_color);  // FIXME
  }

  if(number.cells[2]){
    // Dibujamos celda 2
    x0 = w0 - 1;
    x1 = x0;
    y0 = score_box_w0_y - score_dbox_height + 2;
    y1 = y0 + score_cell_line_px;
    draw_rectangle(buffer, x0, x1, y0, y1, sky_color);  // FIXME
  }

  if(number.cells[3]){
    // Dibujamos celda 3
    x0 = w0 - score_dbox_width + 2;
    x1 = x0 + score_cell_line_px;
    y0 = score_box_w0_y - (score_cell_line_px + 3);
    y1 = y0;
    draw_rectangle(buffer, x0, x1, y0, y1, clouds_color);  // FIXME
  }

  if(number.cells[4]){
    // Dibujamos celda 4
    x0 = w0 - score_dbox_width + 1;
    x1 = x0;
    y0 = score_box_w0_y - (score_cell_line_px + 2);
    y1 = y0 + score_cell_line_px;
    draw_rectangle(buffer, x0, x1, y0, y1, score_digit_color);
  }

  if(number.cells[5]){
    // Dibujamos celda 5
    x0 = w0 - score_dbox_width + 2;
    x1 = x0;
    y0 = score_box_w0_y - 1;
    y1 = y0 + score_cell_line_px;
    draw_rectangle(buffer, x0, x1, y0, y1, score_digit_color);
  }

  if(number.cells[6]){
    // Dibujamos celda 6
    x0 = w0 - 1;
    x1 = x0;
    y0 = score_box_w0_y - (score_cell_line_px + 2);
    y1 = y0 + score_cell_line_px;
    draw_rectangle(buffer, x0, x1, y0, y1, score_digit_color);
  }
}

static void
draw_zero(uchar* buffer, uint position)
{
  digit_cell zero;
  set_digit_true(zero);
  zero.cells[3] = false;
  draw_digit(buffer, position, zero);
}

static void
draw_one(uchar* buffer, uint position)
{
  digit_cell one;
  set_digit_false(one);
  one.cells[2] = true;
  one.cells[6] = true;
  draw_digit(buffer, position, one);
}

static void
draw_two(uchar* buffer, uint position)
{
  digit_cell two;
  set_digit_true(two);
  two.cells[0] = false;
  two.cells[6] = false;
  draw_digit(buffer, position, two);
}

static void
draw_three(uchar* buffer, uint position)
{
  digit_cell three;
  set_digit_true(three);
  three.cells[0] = false;
  three.cells[4] = false;
  draw_digit(buffer, position, three);
}

static void
draw_four(uchar* buffer, uint position)
{
  digit_cell four;
  set_digit_true(four);
  four.cells[1] = false;
  four.cells[4] = false;
  four.cells[5] = false;
  draw_digit(buffer, position, four);
}

static void
draw_five(uchar* buffer, uint position)
{
  digit_cell five;
  set_digit_true(five);
  five.cells[2] = false;
  five.cells[4] = false;
  draw_digit(buffer, position, five);
}

static void
draw_six(uchar* buffer, uint position)
{
  digit_cell six;
  set_digit_true(six);
  six.cells[2] = false;
  draw_digit(buffer, position, six);
}

static void
draw_seven(uchar* buffer, uint position)
{
  digit_cell seven;
  set_digit_false(seven);
  seven.cells[1] = true;
  seven.cells[2] = true;
  seven.cells[6] = true;
  draw_digit(buffer, position, seven);
}

static void
draw_eight(uchar* buffer, uint position)
{
  digit_cell eight;
  set_digit_true(eight);
  draw_digit(buffer, position, eight);
}

static void
draw_nine(uchar* buffer, uint position)
{
  digit_cell nine;
  set_digit_true(nine);
  nine.cells[4] = false;
  draw_digit(buffer, position, nine);
}

/*
* Llama a la función correspondiente al dígito
* para renderizarlo en pantalla en la casilla
* señalada para ese mismo.
*/
static void
draw_check_digit(uchar* buffer, uint digit, uint position){
  switch (digit)
    {
    case 0:
      draw_zero(buffer, position);
      break;

    case 1:
      draw_one(buffer, position);
      break;

    case 2:
      draw_two(buffer, position);
      break;

    case 3:
      draw_three(buffer, position);
      break;

    case 4:
      draw_four(buffer, position);
      break;

    case 5:
      draw_five(buffer, position);
      break;

    case 6:
      draw_six(buffer, position);
      break;

    case 7:
      draw_seven(buffer, position);
      break;

    case 8:
      draw_eight(buffer, position);
      break;

    case 9:
      draw_nine(buffer, position);
      break;

    default:
      break;
    }
}

/// FIN DIBUJAR NUMEROS INDIVIDUALES

/*
 * Dibuja en pantalla el score actual hasta 10
 * casillas. Las casillas están enumeradas desde
 * 0 hasta 9. La casilla 0 es la casilla correspondiente
 * a la unidad (primer casilla de la derecha).
*/
static void
draw_score(uchar* buffer)
{
  draw_rectangle(
    buffer,
    score_box_w0_x-score_offset_width, score_box_w0_x,
    score_box_w0_y-score_offset_height, score_box_w0_y,
    score_background_color);
  uint current_digit = 0;
  uint aux_score = game.current_score;
  for(uint pos_digit = 0; pos_digit < 10 && aux_score > 0; ++pos_digit){
    // Extraigo el último dígito el score actual
    current_digit = aux_score % 10;
    aux_score = aux_score / 10;
    draw_check_digit(buffer, current_digit, pos_digit);
  }
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
  draw_rectangle(
    buffer, 0, VGA_graphic_width, clouds_height, bush_height, clouds_color);
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
  draw_rectangle(
    buffer, 0, VGA_graphic_width, bush_height, ground_height, bush_color);
}

static void
draw_ground(uchar* buffer)
{
  draw_rectangle(
    buffer,
    0, VGA_graphic_width,
    ground_height, VGA_graphic_height,
    ground_color);
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
draw_game(uchar* buffer)
{
  draw_background(buffer);
  draw_tubes(buffer);
  draw_flappy(buffer, (int)game.flappy_pos_x, (int)game.flappy_pos_y);
  draw_score(buffer);
}
