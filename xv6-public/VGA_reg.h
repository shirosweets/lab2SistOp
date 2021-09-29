
/* */
//#define VGA_NUM_REGS (1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)

#define mode_array_size 61
/*
  VGA_mode y VGA_font están definidos (y enumerados en types.h)
  Por ende para incluir VGA_reg.h hay que primero incluir types.h
*/

VGA_mode actual_mode;

uchar g_40x25_text[mode_array_size];
uchar g_40x50_text[mode_array_size];
uchar g_80x25_text[mode_array_size];
uchar g_80x50_text[mode_array_size];
uchar g_90x30_text[mode_array_size];
uchar g_90x60_text[mode_array_size];
uchar g_640x480x2[mode_array_size];
uchar g_320x200x4[mode_array_size];
uchar g_640x480x16[mode_array_size];
uchar g_720x480x16[mode_array_size];
uchar g_320x200x256[mode_array_size];
uchar g_320x200x256_modex[mode_array_size];
uchar g_8x8_font[2048];
uchar g_8x16_font[4096];
int VGA_palette_256[256];
uchar* VGA_modes[VGA_mode_amount];
uchar* VGA_fonts[VGA_fonts_amount];

