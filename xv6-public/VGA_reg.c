#include "types.h"
#include "const.h"
#include "defs.h"
#include "x86.h"
#include "VGA_reg.h"


// Alguna definiciones y funciones sacadas de: https://github.com/sam46/xv6


void
vgaSetPalette(int index, int r, int g, int b)
{
  outb(VGA_DAC_WRITE_INDEX, index);
  outb(VGA_DAC_DATA, r);
  outb(VGA_DAC_DATA, g);
  outb(VGA_DAC_DATA, b);
}

/* Seetea la paleta del VGA al default */
void
setdefaultVGApalette()
{
  for(int index = 0; index < 256; index++){
    int value = VGA_palette_256_24[index];
    vgaSetPalette(index,
              (value>>18)&0x3f,
              (value>>10)&0x3f,
              (value>>2)&0x3f);
  }
}

////

static bool
mode_is_text(VGA_mode mode)
{
  return(VGA_text_40x25 <= mode && mode <= VGA_text_90x60);
}

static bool
mode_is_graphic(VGA_mode mode)
{
  return(VGA_graphic_640x480x2 <= mode && mode <= VGA_graphic_320x200x256_modex);
}

/* Retorna la altura del modo, tanto si es modo texto como gráfico
 * si en un modo invalido retorna 0
 */
static int
mode_hight(VGA_mode mode)
{
  int res = 0;
  switch(mode){
  case VGA_text_40x25:
  case VGA_text_40x50:
    res = 40;
    break;
  case VGA_text_80x25:
  case VGA_text_80x50:
    res = 80;
    break;
  case VGA_text_90x30:
  case VGA_text_90x60:
    res = 90;
    break;
  case VGA_graphic_640x480x2:
    res = 640;
    break;
  case VGA_graphic_320x200x4:
    res = 320;
    break;
  case VGA_graphic_640x480x16:
    res = 640;
    break;
  case VGA_graphic_720x480x16:
    res = 720;
    break;
  case VGA_graphic_320x200x256:
  case VGA_graphic_320x200x256_modex:
    res = 320;
    break;
  }
  return(res);
}

/* Retorna el ancho del modo, tanto si es modo texto como gráfico
 * si en un modo invalido retorna 0
 */
static int
mode_width(VGA_mode mode)
{
  int res = 0;
  switch(mode){
  case VGA_text_40x25:
  case VGA_text_80x25:
    res = 25;
    break;
  case VGA_text_40x50:
  case VGA_text_80x50:
    res = 50;
    break;
  case VGA_text_90x30:
    res = 30;
    break;
  case VGA_text_90x60:
    res = 60;
    break;
  case VGA_graphic_640x480x2:
  case VGA_graphic_640x480x16:
  case VGA_graphic_720x480x16:
    res = 480;
    break;
  case VGA_graphic_320x200x4:
  case VGA_graphic_320x200x256:
  case VGA_graphic_320x200x256_modex:
    res = 200;
    break;
  }
  return(res);
}

/* Variable para saber el modo actual
 * Cada ves que se cambia de modo se la actualiza
 */
VGA_mode actual_mode = VGA_text_80x25;



/* Llamada al sistema que cambia de modo al modo dado,
 * si es un modo invalido no hace nada
 */
void
VGA_mode_switch(VGA_mode mode)
{
  if(mode_is_text(mode)){
    write_regs(VGA_modes[mode]);
    write_font(g_8x16_font, 16);
    actual_mode = mode;
  }
  else if(mode_is_graphic(mode)){
    write_regs(VGA_modes[mode]);
    actual_mode = mode;
  }
}

/* Si las cordenadas son dentro de la pantalla (de 320x200) pinta el pixel,
 * si están afuera no hace nada.
 *
 * Si no está en modo gráfico no hace nada
 */
void
VGA_plot_pixel(int x, int y, uchar color)
{
  if(0 <= x && x < mode_width(actual_mode)
    && 0 <= y && y < mode_hight(actual_mode)
    && mode_is_graphic(actual_mode)){
    framebuffer[y * mode_width(actual_mode) + x] = color;
  }
}

/* Copia mode_width(actual_mode)*mode_hight(actual_mode) bytes de buffer+
 * al arreglo de la pantalla
 *
 * Si no se está en modo gráfico no hace nada
 */
void
VGA_plot_screen(uchar* buffer)
{
  if(mode_is_graphic(actual_mode)){ // 0 es NULL
    for(int j = 0; j < mode_width(actual_mode)*mode_hight(actual_mode); j++){
      framebuffer[j] = buffer[j];
    }
  }
}
