#ifndef VGA_graphics_H
#define VGA_graphics_H


#include "../types.h"

typedef int x_coord;
typedef int y_coord;


/* Pinta el pixel (x, y) en buffer con color, si es que (x, y) está dentro
 * del tamaño de la pantalla
 */
void draw_pixel(uchar* buffer, x_coord x, y_coord y, uchar color);

/* Pinta una línea horizontal en buffer con color, comenzando desde el parámetro
 * x0 y terminando en el x1, se pintará la línea siempre que las coordenadas
 * esten dentro de la pantalla.
 */
void draw_horizontal_line
  (uchar* buffer, x_coord x0, x_coord x1, y_coord y, uchar color);

/* Pinta una línea vertical en buffer con color, comenzando desde el parámetro
 * y0 y terminando en el y1, se pintará la línea siempre que las coordenadas
 * esten dentro de la pantalla.
 */
void draw_vertical_line
  (uchar* buffer, y_coord y0, y_coord y1, x_coord x, uchar color);

/* Pinta un rectángulo entre (x0, y0) y (x1, y1)
 */
void draw_rectangle
  (uchar* buffer, x_coord x0, x_coord x1, y_coord y0, y_coord y1, uchar color);

/* Pinta en buffer un circulo de radio r centrado en (x, y)
 */
void draw_circle(uchar* buffer, x_coord x, y_coord y, int r, uchar color);


#include "VGA_graphics.c"
#endif // VGA_graphics_H