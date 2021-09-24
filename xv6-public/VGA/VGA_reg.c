#include "../x86.h"
#include "../const.h"
#include "../defs.h"
#include "VGA_reg.h"

void c_regdump(unsigned char *regs, unsigned count)
{
  cprintf("\t");
  for(unsigned i = 0; count != 0; count--)
  {
    cprintf("0x%x, ", *regs);
    i++;
    if(i >= 8)
    {
      i = 0;
      cprintf("\n\t");
    }
    else
    {
      cprintf(" ");
    }
    regs++;
  }
  cprintf("\n");
}

void
read_regs(unsigned char *regs) {
  unsigned i;

  // read MISCELLANEOUS reg
	*regs = inb(VGA_MISC_READ);
	regs++;

  // read SEQUENCER regs
	for(i = 0; i < VGA_NUM_SEQ_REGS; i++)
	{
		outb(VGA_SEQ_INDEX, i);
		*regs = inb(VGA_SEQ_DATA);
		regs++;
	}
  // read CRTC regs
	for(i = 0; i < VGA_NUM_CRTC_REGS; i++)
	{
		outb(VGA_CRTC_INDEX, i);
		*regs = inb(VGA_CRTC_DATA);
		regs++;
	}

  // read GRAPHICS CONTROLLER regs
	for(i = 0; i < VGA_NUM_GC_REGS; i++)
	{
		outb(VGA_GC_INDEX, i);
		*regs = inb(VGA_GC_DATA);
		regs++;
	}

  // read ATTRIBUTE CONTROLLER regs
	for(i = 0; i < VGA_NUM_AC_REGS; i++)
	{
		(void)inb(VGA_INSTAT_READ);
		outb(VGA_AC_INDEX, i);
		*regs = inb(VGA_AC_READ);
		regs++;
	}

  // lock 16-color palette and unblank display
	(void)inb(VGA_INSTAT_READ);
	outb(VGA_AC_INDEX, 0x20);
}

void
dump_regs(unsigned char *regs)
{
	cprintf("unsigned char g_mode[] =\n");
	cprintf("{\n");

  /* dump MISCELLANEOUS reg */
	cprintf("/* MISC */\n");
	cprintf("\t0x%x,\n", *regs);
	regs++;

  /* dump SEQUENCER regs */
	cprintf("/* SEQ */\n");
	c_regdump(regs, VGA_NUM_SEQ_REGS);
	regs += VGA_NUM_SEQ_REGS;

  /* dump CRTC regs */
	cprintf("/* CRTC */\n");
	c_regdump(regs, VGA_NUM_CRTC_REGS);
	regs += VGA_NUM_CRTC_REGS;

  /* dump GRAPHICS CONTROLLER regs */
	cprintf("/* GC */\n");
	c_regdump(regs, VGA_NUM_GC_REGS);
	regs += VGA_NUM_GC_REGS;

  /* dump ATTRIBUTE CONTROLLER regs */
	cprintf("/* AC */\n");
	c_regdump(regs, VGA_NUM_AC_REGS);
	regs += VGA_NUM_AC_REGS;
	cprintf("};\n");
}

/* Imprime el estado actual de los registros */
void print_current_regs()
{
  unsigned char state[VGA_NUM_REGS];
  // for (int i=0; i<VGA_NUM_REGS; ++i)cprintf(" %x ", state[i]);
	read_regs(state);
  // for (int i=0; i<VGA_NUM_REGS; ++i)cprintf(" %x ", state[i]);
  dump_regs(state);
  // for (int i=0; i<VGA_NUM_REGS; ++i)cprintf(" %x ", state[i]);
}

// Agregados para los colores

// Alguna definiciones y funciones sacadas de: https://github.com/sam46/xv6
int vga256_24bit[256] =
{
  0x000000, 0x0000a8, 0x00a800, 0x00a8a8, 0xa80000, 0xa800a8, 0xa85400, 0xa8a8a8,
  0x545454, 0x5454fc, 0x54fc54, 0x54fcfc, 0xfc5454, 0xfc54fc, 0xfcfc54, 0xfcfcfc,
  0x000000, 0x141414, 0x202020, 0x2c2c2c, 0x383838, 0x444444, 0x505050, 0x606060,
  0x707070, 0x808080, 0x909090, 0xa0a0a0, 0xb4b4b4, 0xc8c8c8, 0xe0e0e0, 0xfcfcfc,
  0x0000fc, 0x4000fc, 0x7c00fc, 0xbc00fc, 0xfc00fc, 0xfc00bc, 0xfc007c, 0xfc0040,
  0xfc0000, 0xfc4000, 0xfc7c00, 0xfcbc00, 0xfcfc00, 0xbcfc00, 0x7cfc00, 0x40fc00,
  0x00fc00, 0x00fc40, 0x00fc7c, 0x00fcbc, 0x00fcfc, 0x00bcfc, 0x007cfc, 0x0040fc,
  0x7c7cfc, 0x9c7cfc, 0xbc7cfc, 0xdc7cfc, 0xfc7cfc, 0xfc7cdc, 0xfc7cbc, 0xfc7c9c,
  0xfc7c7c, 0xfc9c7c, 0xfcbc7c, 0xfcdc7c, 0xfcfc7c, 0xdcfc7c, 0xbcfc7c, 0x9cfc7c,
  0x7cfc7c, 0x7cfc9c, 0x7cfcbc, 0x7cfcdc, 0x7cfcfc, 0x7cdcfc, 0x7cbcfc, 0x7c9cfc,
  0xb4b4fc, 0xc4b4fc, 0xd8b4fc, 0xe8b4fc, 0xfcb4fc, 0xfcb4e8, 0xfcb4d8, 0xfcb4c4,
  0xfcb4b4, 0xfcc4b4, 0xfcd8b4, 0xfce8b4, 0xfcfcb4, 0xe8fcb4, 0xd8fcb4, 0xc4fcb4,
  0xb4fcb4, 0xb4fcc4, 0xb4fcd8, 0xb4fce8, 0xb4fcfc, 0xb4e8fc, 0xb4d8fc, 0xb4c4fc,
  0x000070, 0x1c0070, 0x380070, 0x540070, 0x700070, 0x700054, 0x700038, 0x70001c,
  0x700000, 0x701c00, 0x703800, 0x705400, 0x707000, 0x547000, 0x387000, 0x1c7000,
  0x007000, 0x00701c, 0x007038, 0x007054, 0x007070, 0x005470, 0x003870, 0x001c70,
  0x383870, 0x443870, 0x543870, 0x603870, 0x703870, 0x703860, 0x703854, 0x703844,
  0x703838, 0x704438, 0x705438, 0x706038, 0x707038, 0x607038, 0x547038, 0x447038,
  0x387038, 0x387044, 0x387054, 0x387060, 0x387070, 0x386070, 0x385470, 0x384470,
  0x505070, 0x585070, 0x605070, 0x685070, 0x705070, 0x705068, 0x705060, 0x705058,
  0x705050, 0x705850, 0x706050, 0x706850, 0x707050, 0x687050, 0x607050, 0x587050,
  0x507050, 0x507058, 0x507060, 0x507068, 0x507070, 0x506870, 0x506070, 0x505870,
  0x000040, 0x100040, 0x200040, 0x300040, 0x400040, 0x400030, 0x400020, 0x400010,
  0x400000, 0x401000, 0x402000, 0x403000, 0x404000, 0x304000, 0x204000, 0x104000,
  0x004000, 0x004010, 0x004020, 0x004030, 0x004040, 0x003040, 0x002040, 0x001040,
  0x202040, 0x282040, 0x302040, 0x382040, 0x402040, 0x402038, 0x402030, 0x402028,
  0x402020, 0x402820, 0x403020, 0x403820, 0x404020, 0x384020, 0x304020, 0x284020,
  0x204020, 0x204028, 0x204030, 0x204038, 0x204040, 0x203840, 0x203040, 0x202840,
  0x2c2c40, 0x302c40, 0x342c40, 0x3c2c40, 0x402c40, 0x402c3c, 0x402c34, 0x402c30,
  0x402c2c, 0x40302c, 0x40342c, 0x403c2c, 0x40402c, 0x3c402c, 0x34402c, 0x30402c,
  0x2c402c, 0x2c4030, 0x2c4034, 0x2c403c, 0x2c4040, 0x2c3c40, 0x2c3440, 0x2c3040,
  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000
};

void
vgaSetPalette(int index, int r, int g, int b)
{
  outb(VGA_DAC_WRITE_INDEX, index);
  outb(VGA_DAC_DATA, r);
  outb(VGA_DAC_DATA, g);
  outb(VGA_DAC_DATA, b);
}

void
setdefaultVGApalette()
{
  for(int index = 0; index < 256; index++)
  {
    int value = vga256_24bit[index];
    vgaSetPalette(index,
              (value>>18)&0x3f,
              (value>>10)&0x3f,
              (value>>2)&0x3f);
  }
}

///

/*
  write_regs sacado de https://files.osdev.org/mirrors/geezer/osd/graphics/modes.c
  (código de dominio público)
 */
void
write_regs(unsigned char *regs)
{
  unsigned i;

  /* write MISCELLANEOUS reg */
  outb(VGA_MISC_WRITE, *regs);
  regs++;

  /* write SEQUENCER regs */
  for(i = 0; i < VGA_NUM_SEQ_REGS; i++) {
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
  regs[0x03] |= 0x80;
  regs[0x11] &= ~0x80;

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

  setdefaultVGApalette();
}
