typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef uint pde_t;

typedef int bool;
#define false 0
#define true 1

#ifndef NULL
  #define NULL 0
#endif

#define VGA_mode_amount 2

typedef enum
{
  VGA_text_80x25,
  VGA_graphic_320x200x256,
} VGA_mode;


