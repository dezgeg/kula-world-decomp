#include "common.h"

extern void InitWidgetSprites(void);
extern void SetTextParams(int posX, int posY, int align, int colorR, int colorG, int colorB);
extern void SetFontEntry(char c, uchar u, uchar v, uchar width, uchar height, uchar x, uchar y,
                         int texIndex);

extern int displayWidth;
extern uint firstGuiTexture;

void InitFont(void) {
    int i;

    InitWidgetSprites();
    SetTextParams(displayWidth >> 1, 0, 1, 128, 128, 128);
    for (i = 0; i < 128; i++) {
        SetFontEntry(i, 0, 0, 0, 0, 0, 0, firstGuiTexture + 9);
    }
    SetFontEntry('e', 0, 0, 11, 11, 0, 2, firstGuiTexture + 10);
    SetFontEntry('f', 12, 0, 11, 11, 0, 2, firstGuiTexture + 10);
    SetFontEntry('g', 24, 0, 11, 11, 0, 2, firstGuiTexture + 10);
    SetFontEntry('h', 36, 0, 11, 11, 0, 2, firstGuiTexture + 10);
    SetFontEntry('A', 0, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('B', 8, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('C', 16, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('D', 24, 0, 7, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('E', 32, 0, 6, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('F', 40, 0, 6, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('G', 48, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('H', 56, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('I', 64, 0, 4, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('J', 72, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('K', 80, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('L', 88, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('M', 96, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('N', 104, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('O', 112, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('P', 120, 0, 7, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('Q', 128, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('R', 136, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('S', 144, 0, 7, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('T', 152, 0, 7, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('U', 160, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('V', 168, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('W', 176, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('X', 184, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('Y', 192, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('Z', 200, 0, 8, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('0', 232, 0, 7, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('1', 240, 0, 7, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('2', 248, 0, 7, 12, 0, 0, firstGuiTexture + 9);
    SetFontEntry('3', 0, 12, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('4', 8, 12, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('5', 16, 12, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('6', 24, 12, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('7', 32, 12, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('8', 40, 12, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('9', 48, 12, 7, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('.', 56, 12, 4, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry(',', 64, 12, 4, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry(':', 72, 12, 4, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry(';', 80, 12, 4, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('-', 88, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('+', 96, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('\'', 104, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('`', 104, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('!', 112, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('?', 120, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('\"', 128, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('#', 136, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('%', 144, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('&', 152, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('/', 160, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('\\', 168, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('(', 176, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry(')', 184, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('[', 176, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry(']', 184, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('{', 176, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('}', 184, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('=', 192, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('>', 200, 12, 12, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('<', 212, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry('|', 220, 12, 8, 10, 0, 2, firstGuiTexture + 9);
    SetFontEntry(' ', 0, 0, 4, 0, 0, 2, firstGuiTexture + 9);
}
