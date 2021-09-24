#ifndef VGA_REG_H
#define VGA_REG_H

/* */
#define VGA_NUM_REGS (1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)

/* Lee los registros actuales y los escribe en el arreglo regs */
void read_regs(unsigned char *regs);

/* Escribe en consola los registros del arreglo regs */
void dump_regs(unsigned char *regs);

/* Escribe en consola los registros actuales */
void print_current_regs(void);

/* */
void vgaSetPalette(int index, int r, int g, int b);

/* Setea la paleta del VGA al default */
void setdefaultVGApalette();

/* Escribe en los registros del arreglo regs */
void write_regs(unsigned char *regs);

#include "VGA_reg.c"

#endif