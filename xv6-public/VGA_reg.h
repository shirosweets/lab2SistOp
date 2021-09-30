/*
  VGA_mode y VGA_font están definidos (y enumerados en types.h)
  P2V está definido en memlayout.h
  Por ende para incluir VGA_reg.h hay que primero incluir types.h y a memlayout.h
*/


#define buffer_text_80x25 P2V(0xB8000)
#define buffer_graphic_320x200 P2V(0xA0000)

#define mode_array_size 61


VGA_mode actual_mode;

uchar g_80x25_text[mode_array_size];
uchar g_320x200x256[mode_array_size];
uchar g_8x16_font[4096];
int VGA_palette_256[256];

uchar* VGA_modes[VGA_mode_amount];

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

#define VGA_font_array P2V(0xB8000)

