#include "../types.h"
#include "flappy_bird_score.h"
#include "flappy_bird_TAD.h"

#define score_background_color 0x00
#define score_digit_color 0x28

#define score_color_yellow 0x2E
#define score_color_purple 0x39
#define score_color_pink 0x26
#define score_color_orange 0x2A
#define score_color_cyan 0x0B
#define score_color_white 0x0F

/*
 * digit_cell:
 * Abstracción que modela la configuración de un dígito
 * para una casilla. El valor tiene dentro 7 celdas
 * que se prenden o apagan de acuerdo al número que se
 * desea dibujar.
 * Se modela con un char (tamaño: 1 byte)
*/
typedef char digit_cell;

/*
 * Devuelve un digit_cell donde todos los valores son true
 * equivalente a todas las lineas pintadas: 8
*/
static inline digit_cell
digit_c_all_true(void)
{
  return ((digit_cell)255); // 0b11111111
}

/*
 * Devuelve un digit_cell donde todos los valores son falso
 * equivalente a no pintar nada.
*/
static inline digit_cell
digit_c_all_false(void)
{
  return ((digit_cell)0); // 0b00000000
}

/*
 * Usa operaciones bitwise (bit a bit) para setear la posición correcta
 * con el valor dado por la variable 'active'
 * devuelve el valor modificado. Utilizado para configurar
 * los números para luego dibujarlos.
*/
static inline digit_cell
digit_c_put(digit_cell digit, int position, bool active)
{
  /*
   * Máscara (uchar)1 << position : todos 0 excepto en la posición del arg
   * donde es 1 siempre
   * luego cond: ((uchar)active << position): todos 0 y en la posición del arg
   * esta 0 o 1 dependiendo de 'active'
   * La función primero setea la posición pedida en cero: (digit & ~mask)
   * y luego hace un OR con el valor pasado (bool 'active')
   * Por ejemplo:
   *  digit = 0b01000010 (como configuramos al '1'),
   *  position = 3
   *  active = true
   *    -> mask            = 0b00001000
   *    -> cond            = 0b00001000 == 0b0000(active=1)000
   *    -> ~mask           = 0b11110111
   *    -> (digit & ~mask) = 0b01000010
   * luego (digit & ~mask) | cond = 0b01001010
   * Es decir el valor original de digit pero en la posicion 3 está
   * el valor de active (1)
  */
  return ((digit & ~((uchar)1 << position)) | ((uchar)active << position));
}

// Devuelve el booleano guardado en la posición pedida.
static inline bool
digit_c_get(digit_cell digit, int position)
{
  /*
   * Hace un & bitwise del dígito con la máscara.
   * El resultado son todos bits en 0, pero en la posición
   * pedida queda como estaba. Si era cero, entonces todo el
   * valor va a ser cero.
   * 0b00001111 << 2 == 0b00111100
   *
   * Ejemplo 1: position = 1, digit = 0b01000010
   * (uchar)1 << position == mask   = 0b00000010
   *                   digit & mask = 0b00000010
   * luego (digit & mask) NO es 0b00000000
   * por lo que devuelve true
   *
   * Ejemplo 2: position = 2, digit = 0b01000010
   * (uchar)1 << position == mask   = 0b00000100
   *                   digit & mask = 0b00000000
   * luego (digit & mask) es 0b00000000
   * por lo que devuelve false
  */
  return (((uchar)0 != ((uchar)(digit & ((uchar)1 << position)))));
}

//
# define score_cell_line_px 4
// dbox = digit box: tamaño de la casilla donde se dibujar el dígito
#define score_dbox_height (score_cell_line_px * 2 + 5)
#define score_dbox_width (score_cell_line_px + 4)
//
#define score_total_digit_boxes 10
#define score_offset_height score_dbox_height
#define score_offset_width (score_dbox_width * score_total_digit_boxes)

#define score_box_w0_x VGA_graphic_width
#define score_box_w0_y VGA_graphic_height

/*
 * Toma la configuración de qué celdas pintar de la variable number
 * y cuál casilla pintar de la variable position.
 *
 * Pinta las líneas respecto al dígito que deba ser.
*/
static void
draw_digit(uchar* buffer, uint position, digit_cell number)
{
  // wbr: (bottom right) primer pixel (inferior derecho) de la casilla actual
  // wtl: (top left) último pixel (superior izquierdo) de la casilla actual
  uint wbr_x = score_box_w0_x - score_dbox_width * position;
  uint wbr_y = score_box_w0_y;
  uint line_offset = score_cell_line_px - 1;
  uint wtl_x = wbr_x - score_dbox_width + 1;
  uint wtl_y = wbr_y - score_dbox_height + 1;
  uint x0, x1, y0, y1;

  if(digit_c_get(number, 0)){
    // Dibujamos celda 0
    x0 = wtl_x + 1;
    x1 = x0;
    y0 = wtl_y + 2;
    y1 = y0 + line_offset;
    draw_rectangle(buffer, x0, x1, y0, y1, score_digit_color);
  }

  if(digit_c_get(number, 1)){
    // Dibujamos celda 1
    x0 = wtl_x + 2;
    x1 = x0 + line_offset;
    y0 = wtl_y + 1;
    y1 = y0;
    draw_rectangle(buffer, x0, x1, y0, y1, score_digit_color);
  }

  if(digit_c_get(number, 2)){
    // Dibujamos celda 2
    x0 = wbr_x - 1;
    x1 = x0;
    y0 = wtl_y + 2;
    y1 = y0 + line_offset;
    draw_rectangle(buffer, x0, x1, y0, y1, score_digit_color);
  }

  if(digit_c_get(number, 3)){
    // Dibujamos celda 3
    x0 = wtl_x + 2;
    x1 = x0 + line_offset;
    y0 = score_box_w0_y - score_cell_line_px - 2;
    y1 = y0;
    draw_rectangle(buffer, x0, x1, y0, y1, score_digit_color);
  }

  if(digit_c_get(number, 4)){
    // Dibujamos celda 4
    x0 = wtl_x + 1;
    x1 = x0;
    y0 = wbr_y - score_cell_line_px - 1;
    y1 = y0 + line_offset;
    draw_rectangle(buffer, x0, x1, y0, y1, score_digit_color);
  }

  if(digit_c_get(number, 5)){
    // Dibujamos celda 5
    x0 = wtl_x + 2;
    x1 = x0 + line_offset;
    y0 = wbr_y - 1;
    y1 = y0;
    draw_rectangle(buffer, x0, x1, y0, y1, score_digit_color);
  }

  if(digit_c_get(number, 6)){
    // Dibujamos celda 6
    x0 = wbr_x - 1;
    x1 = x0;
    y0 = wbr_y - score_cell_line_px - 1;
    y1 = y0 + line_offset;
    draw_rectangle(buffer, x0, x1, y0, y1, score_digit_color);
  }
}

static void
draw_zero(uchar* buffer, uint position)
{
  digit_cell zero = digit_c_all_true();
  zero = digit_c_put(zero, 3, false);
  draw_digit(buffer, position, zero);
}

static void
draw_one(uchar* buffer, uint position)
{
  digit_cell one = digit_c_all_false();
  one = digit_c_put(one, 2, true);
  one = digit_c_put(one, 6, true);
  draw_digit(buffer, position, one);
}

static void
draw_two(uchar* buffer, uint position)
{
  digit_cell two = digit_c_all_true();
  two = digit_c_put(two, 0, false);
  two = digit_c_put(two, 6, false);
  draw_digit(buffer, position, two);
}

static void
draw_three(uchar* buffer, uint position)
{
  digit_cell three = digit_c_all_true();
  three = digit_c_put(three, 0, false);
  three = digit_c_put(three, 4, false);
  draw_digit(buffer, position, three);
}

static void
draw_four(uchar* buffer, uint position)
{
  digit_cell four = digit_c_all_true();
  four = digit_c_put(four, 1, false);
  four = digit_c_put(four, 4, false);
  four = digit_c_put(four, 5, false);
  draw_digit(buffer, position, four);
}

static void
draw_five(uchar* buffer, uint position)
{
  digit_cell five = digit_c_all_true();
  five = digit_c_put(five, 2, false);
  five = digit_c_put(five, 4, false);
  draw_digit(buffer, position, five);
}

static void
draw_six(uchar* buffer, uint position)
{
  digit_cell six = digit_c_all_true();
  six = digit_c_put(six, 2, false);
  draw_digit(buffer, position, six);
}

static void
draw_seven(uchar* buffer, uint position)
{
  digit_cell seven = digit_c_all_false();
  seven = digit_c_put(seven, 1, true);
  seven = digit_c_put(seven, 2, true);
  seven = digit_c_put(seven, 6, true);
  draw_digit(buffer, position, seven);
}

static void
draw_eight(uchar* buffer, uint position)
{
  digit_cell eight = digit_c_all_true();
  draw_digit(buffer, position, eight);
}

static void
draw_nine(uchar* buffer, uint position)
{
  digit_cell nine = digit_c_all_true();
  nine = digit_c_put(nine, 4, false);
  draw_digit(buffer, position, nine);
}

/*
* Llama a la función correspondiente al dígito
* para renderizarlo en pantalla en la casilla
* señalada para ese mismo.
*/
static void
draw_check_digit(uchar* buffer, uint digit, uint position){
  switch (digit){
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

void
draw_score(uchar* buffer, int score)
{
draw_rectangle(
    buffer,
    score_box_w0_x-score_offset_width+1, score_box_w0_x,
    score_box_w0_y-score_offset_height+1, score_box_w0_y,
    score_background_color);
  uint current_digit = 0;
  uint aux_score = game.current_score;

  if(game.current_score == 0){
    draw_check_digit(buffer, 0, 0);
    return;
  }

  // Recorremos cada unidad del score y la dibujamos
  for(uint pos_digit = 0; pos_digit < 10 && aux_score > 0; ++pos_digit){
    // Extraigo el último dígito el score actual
    current_digit = aux_score % 10;
    aux_score = aux_score / 10;
    draw_check_digit(buffer, current_digit, pos_digit);
  }
}