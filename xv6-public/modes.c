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


/*****************************************************************************
MAIN/DEMO ROUTINES
*****************************************************************************/


/*****************************************************************************
Escribe en los registros del arreglo regs
*****************************************************************************/
void
write_regs(uchar *regs)
{
  /* write MISCELLANEOUS reg */
  outb(VGA_MISC_WRITE, *regs);
  regs++;
  /* write SEQUENCER regs */
  for(uint i = 0; i < VGA_NUM_SEQ_REGS; i++){
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
  for(uint i = 0; i < VGA_NUM_CRTC_REGS; i++){
    outb(VGA_CRTC_INDEX, i);
    outb(VGA_CRTC_DATA, *regs);
    regs++;
  }
  /* write GRAPHICS CONTROLLER regs */
  for(uint i = 0; i < VGA_NUM_GC_REGS; i++){
    outb(VGA_GC_INDEX, i);
    outb(VGA_GC_DATA, *regs);
    regs++;
  }
  /* write ATTRIBUTE CONTROLLER regs */
  for(uint i = 0; i < VGA_NUM_AC_REGS; i++){
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
  p &= 3;
  uchar pmask = 1 << p;
  /* set read plane */
  outb(VGA_GC_INDEX, 4);
  outb(VGA_GC_DATA, p);
  /* set write plane */
  outb(VGA_SEQ_INDEX, 2);
  outb(VGA_SEQ_DATA, pmask);
}
/*****************************************************************************
VGA framebuffer is at A0000:0000, B0000:0000, or B8000:0000
depending on bits in GC 6
*****************************************************************************/
uint
get_fb_seg(void)
{
  outb(VGA_GC_INDEX, 6);
  uint seg = inb(VGA_GC_DATA);
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
write font to plane P4 (assuming planes are named P1, P2, P4, P8)
*****************************************************************************/
void
write_font(uchar *buf, uint font_height)
{
  /* save registers
  set_plane() modifies GC 4 and SEQ 2, so save them as well */
  outb(VGA_SEQ_INDEX, 2);
  uchar seq2 = inb(VGA_SEQ_DATA);

  outb(VGA_SEQ_INDEX, 4);
  uchar seq4 = inb(VGA_SEQ_DATA);
  /* turn off even-odd addressing (set flat addressing)
  assume: chain-4 addressing already off */
  outb(VGA_SEQ_DATA, seq4 | 0x04);

  outb(VGA_GC_INDEX, 4);
  uchar gc4 = inb(VGA_GC_DATA);

  outb(VGA_GC_INDEX, 5);
  uchar gc5 = inb(VGA_GC_DATA);
  /* turn off even-odd addressing */
  outb(VGA_GC_DATA, gc5 & ~0x10);

  outb(VGA_GC_INDEX, 6);
  uchar gc6 = inb(VGA_GC_DATA);
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
