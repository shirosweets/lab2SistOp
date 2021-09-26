/*****************************************************************************
Sets VGA-compatible video modes without using the BIOS
Chris Giese <geezer@execpc.com>  http://my.execpc.com/~geezer
Release date: ?
This code is public domain (no copyright).
You can do whatever you want with it.

To do:
- more registers dumps, for various text modes and ModeX
- flesh out code to support SVGA chips?
- do something with 16- and 256-color palettes?
*****************************************************************************/
#include "types.h"
#include "defs.h"
#include "x86.h"
#include "VGA_reg.h"
#include "memlayout.h"

#define peekb(S,O)    *(uchar *)(16uL * (S) + (O))
#define pokeb(S,O,V)    *(uchar *)(16uL * (S) + (O)) = (V)
#define pokew(S,O,V)    *(ushort *)(16uL * (S) + (O)) = (V)
#define _vmemwr(DS,DO,S,N)  memmove((char *)((DS) * 16 + (DO)), S, N)

#define VGA_AC_INDEX 0x3C0
#define VGA_AC_WRITE 0x3C0
#define VGA_AC_READ 0x3C1
#define VGA_MISC_WRITE 0x3C2
#define VGA_SEQ_INDEX 0x3C4
#define VGA_SEQ_DATA 0x3C5
#define VGA_DAC_READ_INDEX 0x3C7
#define VGA_DAC_WRITE_INDEX 0x3C8
#define VGA_DAC_DATA 0x3C9
#define VGA_MISC_READ 0x3CC
#define VGA_GC_INDEX 0x3CE
#define VGA_GC_DATA 0x3CF
  /*      COLOR emulation    MONO emulation */
#define VGA_CRTC_INDEX 0x3D4    /* 0x3B4 */
#define VGA_CRTC_DATA 0x3D5    /* 0x3B5 */
#define VGA_INSTAT_READ 0x3DA

#define VGA_NUM_SEQ_REGS 5
#define VGA_NUM_CRTC_REGS 25
#define VGA_NUM_GC_REGS 9
#define VGA_NUM_AC_REGS 21
#define VGA_NUM_REGS (1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + \
        VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)

/*****************************************************************************
MAIN/DEMO ROUTINES
*****************************************************************************/
void
dump(uchar *regs, uint count)
{
  uint i;

  i = 0;
  cprintf("\t");
  for(; count != 0; count--){
    cprintf("0x%02X,", *regs);
    i++;
    if(i >= 8){
      i = 0;
      cprintf("\n\t");
    }
    else
      cprintf(" ");
    regs++;
  }
  cprintf("\n");
}
/*****************************************************************************
Escribe en los registros del arreglo regs
*****************************************************************************/
void
dump_regs(uchar *regs)
{
  cprintf("uchar g_mode[] =\n");
  cprintf("{\n");
  /* dump MISCELLANEOUS reg */
  cprintf("/* MISC */\n");
  cprintf("\t0x%02X,\n", *regs);
  regs++;
  /* dump SEQUENCER regs */
  cprintf("/* SEQ */\n");
  dump(regs, VGA_NUM_SEQ_REGS);
  regs += VGA_NUM_SEQ_REGS;
  /* dump CRTC regs */
  cprintf("/* CRTC */\n");
  dump(regs, VGA_NUM_CRTC_REGS);
  regs += VGA_NUM_CRTC_REGS;
  /* dump GRAPHICS CONTROLLER regs */
  cprintf("/* GC */\n");
  dump(regs, VGA_NUM_GC_REGS);
  regs += VGA_NUM_GC_REGS;
  /* dump ATTRIBUTE CONTROLLER regs */
  cprintf("/* AC */\n");
  dump(regs, VGA_NUM_AC_REGS);
  regs += VGA_NUM_AC_REGS;
  cprintf("};\n");
}
/*****************************************************************************
Lee los registros actuales y los escribe en el arreglo regs
*****************************************************************************/
void
dump_regs(uchar *regs)
{
  uint i;

  /* read MISCELLANEOUS reg */
  *regs = inb(VGA_MISC_READ);
  regs++;
  /* read SEQUENCER regs */
  for(i = 0; i < VGA_NUM_SEQ_REGS; i++){
    outb(VGA_SEQ_INDEX, i);
    *regs = inb(VGA_SEQ_DATA);
    regs++;
  }
  /* read CRTC regs */
  for(i = 0; i < VGA_NUM_CRTC_REGS; i++){
    outb(VGA_CRTC_INDEX, i);
    *regs = inb(VGA_CRTC_DATA);
    regs++;
  }
  /* read GRAPHICS CONTROLLER regs */
  for(i = 0; i < VGA_NUM_GC_REGS; i++){
    outb(VGA_GC_INDEX, i);
    *regs = inb(VGA_GC_DATA);
    regs++;
  }
  /* read ATTRIBUTE CONTROLLER regs */
  for(i = 0; i < VGA_NUM_AC_REGS; i++){
    (void)inb(VGA_INSTAT_READ);
    outb(VGA_AC_INDEX, i);
    *regs = inb(VGA_AC_READ);
    regs++;
  }
  /* lock 16-color palette and unblank display */
  (void)inb(VGA_INSTAT_READ);
  outb(VGA_AC_INDEX, 0x20);
}
/*****************************************************************************
Escribe en los registros del arreglo regs
*****************************************************************************/
void
write_regs(uchar *regs)
{
  uint i;

  /* write MISCELLANEOUS reg */
  outb(VGA_MISC_WRITE, *regs);
  regs++;
  /* write SEQUENCER regs */
  for(i = 0; i < VGA_NUM_SEQ_REGS; i++){
    outb(VGA_SEQ_INDEX, i);
    outb(VGA_SEQ_DATA, *regs);
    regs++;
  }
  /* unlock CRTC registers */
  outb(VGA_CRTC_INDEX, 0x03);
  outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) | 0x80);
  outb(VGA_CRTC_INDEX, 0x11);
  outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) & ~0x80);
  /* make sure they remain unlocked */
  // regs[0x03] |= 0x80;
  // regs[0x11] &= ~0x80;
  /* write CRTC regs */
  for(i = 0; i < VGA_NUM_CRTC_REGS; i++){
    outb(VGA_CRTC_INDEX, i);
    outb(VGA_CRTC_DATA, *regs);
    regs++;
  }
  /* write GRAPHICS CONTROLLER regs */
  for(i = 0; i < VGA_NUM_GC_REGS; i++){
    outb(VGA_GC_INDEX, i);
    outb(VGA_GC_DATA, *regs);
    regs++;
  }
  /* write ATTRIBUTE CONTROLLER regs */
  for(i = 0; i < VGA_NUM_AC_REGS; i++){
    (void)inb(VGA_INSTAT_READ);
    outb(VGA_AC_INDEX, i);
    outb(VGA_AC_WRITE, *regs);
    regs++;
  }
  /* lock 16-color palette and unblank display */
  (void)inb(VGA_INSTAT_READ);
  outb(VGA_AC_INDEX, 0x20);
}
/*****************************************************************************
*****************************************************************************/
void
set_plane(uint p)
{
  uchar pmask;

  p &= 3;
  pmask = 1 << p;
  /* set read plane */
  outb(VGA_GC_INDEX, 4);
  outb(VGA_GC_DATA, p);
  /* set write plane */
  outb(VGA_SEQ_INDEX, 2);
  outb(VGA_SEQ_DATA, pmask);
}
/*****************************************************************************
VGA framebuffer is at A0000:0000, B0000:0000, or B8000:0000 // A confirmar
depending on bits in GC 6
*****************************************************************************/
uint
get_fb_seg(void)
{
  uint seg;

  outb(VGA_GC_INDEX, 6);
  seg = inb(VGA_GC_DATA);
  seg >>= 2;
  seg &= 3;
  switch(seg){
  case 0:
  case 1:
    seg = 0xA0000;
    break;
  case 2:
    seg = 0xB0000;
    break;
  case 3:
    seg = 0xB8000;
    break;
  }
  return seg;
}
/*****************************************************************************
*****************************************************************************/
void
vmemwr(uint dst_off, uchar *src, uint count)
{
  _vmemwr(get_fb_seg(), dst_off, src, count);
}
/*****************************************************************************
*****************************************************************************/
void
vpokeb(uint off, uint val)
{
  pokeb(get_fb_seg(), off, val);
}
/*****************************************************************************
*****************************************************************************/
uint
vpeekb(uint off)
{
  return peekb(get_fb_seg(), off);
}
/*****************************************************************************
write font to plane P4 (assuming planes are named P1, P2, P4, P8)
*****************************************************************************/
void
write_font(uchar *buf, uint font_height)
{
  uchar seq2, seq4, gc4, gc5, gc6;

  /* save registers
  set_plane() modifies GC 4 and SEQ 2, so save them as well */
  outb(VGA_SEQ_INDEX, 2);
  seq2 = inb(VGA_SEQ_DATA);

  outb(VGA_SEQ_INDEX, 4);
  seq4 = inb(VGA_SEQ_DATA);
  /* turn off even-odd addressing (set flat addressing)
  assume: chain-4 addressing already off */
  outb(VGA_SEQ_DATA, seq4 | 0x04);

  outb(VGA_GC_INDEX, 4);
  gc4 = inb(VGA_GC_DATA);

  outb(VGA_GC_INDEX, 5);
  gc5 = inb(VGA_GC_DATA);
  /* turn off even-odd addressing */
  outb(VGA_GC_DATA, gc5 & ~0x10);

  outb(VGA_GC_INDEX, 6);
  gc6 = inb(VGA_GC_DATA);
  /* turn off even-odd addressing */
  outb(VGA_GC_DATA, gc6 & ~0x02);
  /* write font to plane P4 */
  set_plane(2);
  /* write font 0 */
  for(uint i = 0; i < 256; i++){
    for (uint j = 0; j < font_height; j++){
      // VGA_MEMBASE
      ((uchar*)VGA_font_array)[32*i + j] = *buf;
      buf++;
    }
  }

  /* restore registers */
  outb(VGA_SEQ_INDEX, 2);
  outb(VGA_SEQ_DATA, seq2);
  outb(VGA_SEQ_INDEX, 4);
  outb(VGA_SEQ_DATA, seq4);
  outb(VGA_GC_INDEX, 4);
  outb(VGA_GC_DATA, gc4);
  outb(VGA_GC_INDEX, 5);
  outb(VGA_GC_DATA, gc5);
  outb(VGA_GC_INDEX, 6);
  outb(VGA_GC_DATA, gc6);
}
/*****************************************************************************
*****************************************************************************/
void (*g_write_pixel)(uint x, uint y, uint c);
uint g_wd, g_ht;
/*****************************************************************************
READ AND DUMP VGA REGISTER VALUES FOR CURRENT VIDEO MODE
This is where g_40x25_text[], g_80x50_text[], etc. came from :)
*****************************************************************************/
void
dump_state(void)
{
  uchar state[VGA_NUM_REGS];

  read_regs(state);
  dump_regs(state);
}
/*****************************************************************************
SET TEXT MODES
*****************************************************************************/
void
set_text_mode(int hi_res)
{
  uint rows, cols, ht, i;

  if(hi_res){
    write_regs(g_90x60_text);
    cols = 90;
    rows = 60;
    ht = 8;
  }
  else{
    write_regs(g_80x25_text);
    cols = 80;
    rows = 25;
    ht = 16;
  }
  /* set font */
  if(ht >= 16)
    write_font(g_8x16_font, 16);
  else
    write_font(g_8x8_font, 8);
  /* tell the BIOS what we've done, so BIOS text output works OK */
  pokew(0x40, 0x4A, cols);  /* columns on screen */
  pokew(0x40, 0x4C, cols * rows * 2); /* framebuffer size */
  pokew(0x40, 0x50, 0);    /* cursor pos'n */
  pokeb(0x40, 0x60, ht - 1);  /* cursor shape */
  pokeb(0x40, 0x61, ht - 2);
  pokeb(0x40, 0x84, rows - 1);  /* rows on screen - 1 */
  pokeb(0x40, 0x85, ht);    /* char height */
  /* set white-on-black attributes for all text */
  for(i = 0; i < cols * rows; i++)
    pokeb(0xB800, i * 2 + 1, 7);
}
/*****************************************************************************
*****************************************************************************/
uchar
reverse_bits(uchar arg)
{
  uchar ret_val = 0;

  if(arg & 0x01)
    ret_val |= 0x80;
  if(arg & 0x02)
    ret_val |= 0x40;
  if(arg & 0x04)
    ret_val |= 0x20;
  if(arg & 0x08)
    ret_val |= 0x10;
  if(arg & 0x10)
    ret_val |= 0x08;
  if(arg & 0x20)
    ret_val |= 0x04;
  if(arg & 0x40)
    ret_val |= 0x02;
  if(arg & 0x80)
    ret_val |= 0x01;
  return ret_val;
}
/*****************************************************************************
512-CHARACTER FONT
*****************************************************************************/
void
font512(void)
{
  /* Turbo C++ 1.0 seems to 'lose' any data declared 'const' */
  /*static*/ const char msg1[] = "!txet sdrawkcaB";
  /*static*/ const char msg2[] = "?rorrim a toG";
  /**/
  uchar seq2, seq4, gc4, gc5, gc6;
  uint font_height, i, j;

  /* start in 80x25 text mode */
  set_text_mode(0);
  /* code pasted in from write_font():
  save registers
  set_plane() modifies GC 4 and SEQ 2, so save them as well */
  outb(VGA_SEQ_INDEX, 2);
  seq2 = inb(VGA_SEQ_DATA);

  outb(VGA_SEQ_INDEX, 4);
  seq4 = inb(VGA_SEQ_DATA);
  /* turn off even-odd addressing (set flat addressing)
  assume: chain-4 addressing already off */
  outb(VGA_SEQ_DATA, seq4 | 0x04);

  outb(VGA_GC_INDEX, 4);
  gc4 = inb(VGA_GC_DATA);

  outb(VGA_GC_INDEX, 5);
  gc5 = inb(VGA_GC_DATA);
  /* turn off even-odd addressing */
  outb(VGA_GC_DATA, gc5 & ~0x10);

  outb(VGA_GC_INDEX, 6);
  gc6 = inb(VGA_GC_DATA);
  /* turn off even-odd addressing */
  outb(VGA_GC_DATA, gc6 & ~0x02);
  /* write font to plane P4 */
  set_plane(2);
  /* this is different from write_font():
  use font 1 instead of font 0, and use it for BACKWARD text */
  font_height = 16;
  for(i = 0; i < 256; i++){
    for(j = 0; j < font_height; j++){
      vpokeb(16384u * 1 + 32 * i + j,
        reverse_bits(
          g_8x16_font[font_height * i + j]));
    }
  }
  /* restore registers */
  outb(VGA_SEQ_INDEX, 2);
  outb(VGA_SEQ_DATA, seq2);
  outb(VGA_SEQ_INDEX, 4);
  outb(VGA_SEQ_DATA, seq4);
  outb(VGA_GC_INDEX, 4);
  outb(VGA_GC_DATA, gc4);
  outb(VGA_GC_INDEX, 5);
  outb(VGA_GC_DATA, gc5);
  outb(VGA_GC_INDEX, 6);
  outb(VGA_GC_DATA, gc6);
  /* now: sacrifice attribute bit b3 (foreground intense color)
  use it to select characters 256-511 in the second font */
  outb(VGA_SEQ_INDEX, 3);
  outb(VGA_SEQ_DATA, 4);
  /* xxx - maybe re-program 16-color palette here
  so attribute bit b3 is no longer used for 'intense' */
  for(i = 0; i < sizeof(msg1); i++){
    vpokeb((80 * 8  + 40 + i) * 2 + 0, msg1[i]);
  /* set attribute bit b3 for backward font */
    vpokeb((80 * 8  + 40 + i) * 2 + 1, 0x0F);
  }
  for(i = 0; i < sizeof(msg2); i++){
    vpokeb((80 * 16 + 40 + i) * 2 + 0, msg2[i]);
    vpokeb((80 * 16 + 40 + i) * 2 + 1, 0x0F);
  }
}
