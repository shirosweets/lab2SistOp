typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef uint pde_t;

typedef int bool;
#define false 0
#define true 1

typedef enum
{
  VGA_text_40x25,
  VGA_text_40x50,
  VGA_text_80x25,
  VGA_text_80x50,
  VGA_text_90x30,
  VGA_text_90x60,
  VGA_graphic_640x480x2,
  VGA_graphic_320x200x4,
  VGA_graphic_640x480x16,
  VGA_graphic_720x480x16,
  VGA_graphic_320x200x256,
  VGA_graphic_320x200x256_modex,
} VGA_mode;

typedef enum
{
  VGA_font_8x8,
  VGA_font_8x16,
} VGA_font;
