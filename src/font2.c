#include "common.h"

extern void InitWidgetSprites(void);
extern void SetTextParams(int posX, int posY, int align, int colorR, int colorG, int colorB);

extern int displayWidth;
extern uint firstGuiTexture;

extern void SetFontEntry(char c, uchar u, uchar v, uchar width, uchar height, uchar x, uchar y,
                         int texIndex);

void InitFont(void) {
    int i;

    InitWidgetSprites();
    SetTextParams(displayWidth >> 1, 0, 1, 0x80, 0x80, 0x80);
    for (i = 0; i < 128; i++) {
        SetFontEntry(i, 0, 0, 0, 0, 0, 0, firstGuiTexture + 9);
    }
    SetFontEntry('e', 0, 0, 0xb, 0xb, 0, 2, firstGuiTexture + 10);
    SetFontEntry('f', 0xc, 0, 0xb, 0xb, 0, 2, firstGuiTexture + 10);
    SetFontEntry('g', 0x18, 0, 0xb, 0xb, 0, 2, firstGuiTexture + 10);
    SetFontEntry('h', 0x24, 0, 0xb, 0xb, 0, 2, firstGuiTexture + 10);
    SetFontEntry('A', 0, 0, 8, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('B', 8, 0, 8, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('C', 0x10, 0, 8, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('D', 0x18, 0, 7, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('E', 0x20, 0, 6, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('F', 0x28, 0, 6, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('G', 0x30, 0, 8, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('H', 0x38, 0, 8, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('I', 0x40, 0, 4, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('J', 0x48, 0, 8, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('K', 0x50, 0, 8, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('L', 0x58, 0, 8, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('M', 0x60, 0, 8, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('N', 0x68, 0, 8, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('O', 0x70, 0, 8, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('P', 0x78, 0, 7, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('Q', 0x80, 0, 8, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('R', 0x88, 0, 8, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('S', 0x90, 0, 7, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('T', 0x98, 0, 7, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('U', 0xa0, 0, 8, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('V', 0xa8, 0, 8, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('W', 0xb0, 0, 8, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('X', 0xb8, 0, 8, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('Y', 0xc0, 0, 8, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('Z', 0xc8, 0, 8, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('0', 0xe8, 0, 7, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('1', 0xf0, 0, 7, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('2', 0xf8, 0, 7, 0xc, 0, 0, firstGuiTexture + 9);
    SetFontEntry('3', 0, 0xc, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('4', 8, 0xc, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('5', 0x10, 0xc, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('6', 0x18, 0xc, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('7', 0x20, 0xc, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('8', 0x28, 0xc, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('9', 0x30, 0xc, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('.', 0x38, 0xc, 4, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry(',', 0x40, 0xc, 4, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry(':', 0x48, 0xc, 4, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry(';', 0x50, 0xc, 4, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('-', 0x58, 0xc, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('+', 0x60, 0xc, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('\'', 0x68, 0xc, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('`', 0x68, 0xc, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('!', 0x70, 0xc, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('?', 0x78, 0xc, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('\"', 0x80, 0xc, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('#', 0x88, 0xc, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('%', 0x90, 0xc, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('&', 0x98, 0xc, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('/', 0xa0, 0xc, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('\\', 0xa8, 0xc, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('(', 0xb0, 0xc, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry(')', 0xb8, 0xc, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('[', 0xb0, 0xc, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry(']', 0xb8, 0xc, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('{', 0xb0, 0xc, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('}', 0xb8, 0xc, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('=', 0xc0, 0xc, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('>', 0xc8, 0xc, 0xc, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('<', 0xd4, 0xc, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('|', 0xdc, 0xc, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry(' ', 0, 0, 4, 0, 0, 2, firstGuiTexture + 9);
}
