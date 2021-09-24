#include "../types.h"
#include "../const.h"

#include "VGA_graphics.h"

void
draw_pixel(uchar* buffer, x_coord x, y_coord y, uchar color)
{
  if(0 <= x && x < VGA_graphic_width && 0 <= y && y < VGA_graphic_height)
    buffer[x + y*VGA_graphic_width] = color;
}

void
draw_horizontal_line
  (uchar* buffer, x_coord x0, x_coord x1, y_coord y, uchar color)
{ // Si x0 > x1 debería hacerse un ciclo muy largo, pero por algún motivo no se hace
  for(x_coord i = x0; i <= x1; i++){
    draw_pixel(buffer, i, y, color);
  }
}

void
draw_vertical_line
  (uchar* buffer, y_coord y0, y_coord y1, x_coord x, uchar color)
{
  for(y_coord j = y0; j <= y1; j++){
    draw_pixel(buffer, x, j, color);
  }
}

void
draw_rectangle
  (uchar* buffer, x_coord x0, x_coord x1, y_coord y0, y_coord y1, uchar color)
{
  for(x_coord i = x0; i <= x1; i++){
    draw_vertical_line(buffer, y0, y1, i, color);
  }
}


void
draw_circle(uchar* buffer, x_coord x, y_coord y, int r, uchar color)
{
  for(x_coord i = x - r; i <= x + r; i++){
    for(y_coord j = y - r; j <= y + r; j++){
      /* Hay que pintar el pixel solo si está adentro del circulo
         Está adentro del circulo si y solo si:
          (i - x)² + (j - y)² == r²
      */
      if((i - x)*(i - x) + (j - y)*(j - y) <= r*r)
        draw_pixel(buffer, i, j, color);
    }
  }
}
