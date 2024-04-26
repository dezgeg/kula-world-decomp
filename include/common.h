#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <libgte.h>
#include <libgpu.h>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;

typedef unsigned char byte;
typedef signed char sbyte;

enum PadButton {
    PAD_SELECT = 0x0001,
    PAD_L3 = 0x0002,
    PAD_R3 = 0x0004,
    PAD_START = 0x0008,
    PAD_U = 0x0010,
    PAD_R = 0x0020,
    PAD_D = 0x0040,
    PAD_L = 0x0080,
    PAD_L2 = 0x0100,
    PAD_R2 = 0x0200,
    PAD_L1 = 0x0400,
    PAD_R1 = 0x0800,
    PAD_TRIANGLE = 0x1000,
    PAD_CIRCLE = 0x2000,
    PAD_CROSS = 0x4000,
    PAD_SQUARE = 0x8000,
};

typedef struct PrimList {
    void* main;
    void* gui1;
    void* gui2;
    void* gui3;
} PrimList;

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

typedef struct DrawDisp {
    DRAWENV draw;
    DISPENV disp;
} DrawDisp;

typedef struct Highscore {
    int score;
    short level;
    short fruits;
    int time;
    char name[8];
} Highscore;

typedef struct PsxSaveHeader {
    char magic[2]; /* SC */
    byte iconFlags;
    byte blockNumber;
    ushort title[32]; /* SJIS */
    byte unused[28];
    byte iconClut[32];
    byte iconBitmap[128];
} PsxSaveHeader;

typedef struct SaveSlot {
    int levelScores[150];
    int score;
    int playtime;
    short fruits;
    short curLevel;
    short curWorld;
    short fruitBitmask;
    short valid;
    short gameMode;
    short timeTrialDifficulty;
    short isFinal;
} SaveSlot;

typedef struct MemcardData {
    PsxSaveHeader header;
    short arcadeModeDone;
    short highestLevelReached;
    short musicVolume;
    short sfxVolume;
    short screenX;
    short screenY;
    Highscore highscore0;
    int highscore0LevelScores[150];
    SaveSlot saveslots[4];
    int turnDelayEnabled;
    int vibrationEnabled;
    byte pad[4804];
} MemcardData;

typedef struct Music {
    char* filename;
    ushort chan;
    short sectors;
} Music;

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
