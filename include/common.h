#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <libgte.h>
#include <libgpu.h>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

typedef struct TSprite {
    DR_TPAGE tpage;
    SPRT sprt;
} TSprite;

typedef struct TPolyF4 {
    DR_TPAGE tpage;
    POLY_F4 poly;
} TPolyF4;

typedef struct Texture {
    u_char bitsPerPixel;
    u_char semitrans;
    u_short clut;
    short tpage;
    u_char u;
    u_char v;
    u_short w;
    u_short h;
} Texture;

#define INCLUDE_ASM(FOLDER, NAME) \
__asm__( \
    ".text\n" \
    "\t.align\t2\n" \
    "\t.set noreorder\n" \
    "\t.set noat\n" \
    ".include \""FOLDER"/"#NAME".s\"\n" \
    "\t.set reorder\n" \
    "\t.set at\n" \
)
#define INCLUDE_RODATA(FOLDER, NAME) \
__asm__( \
    ".section .rodata\n" \
    "\t.include \""FOLDER"/"#NAME".s\"\n" \
    ".section .text" \
)

#endif
