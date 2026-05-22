#include "common.h"

extern void TSpritePrim(TSprite* ts, int dfe, int dtd, int tpage);

// non-gprel-used variables (extern)
extern TSprite biosStrSprites[2][32];
extern PrimList primLists[2];
extern int whichDrawDispEnv;

// gprel-used variables (defined in this file)
int biosStrLen;

typedef struct Tim {
    uint magic;
    uint flags;
    uint clutLength;
    ushort clutX;
    ushort clutY;
    ushort clutWidth;
    ushort clutHeight;
    short clutData[16];
    int imageLength;
    ushort imageX;
    ushort imageY;
    ushort imageWidth;
    ushort imageHeight;
} Tim;

typedef struct BiosTextWriter {
    short unk[2][17];
    Tim tim;
} BiosTextWriter;

void SetupBiosFontStr(ushort *str, int x, int y, int param_4, int fg, int bg, int param_7, Tim *tim) {
    RECT rect;
    int i;
    int j;
    int end;
    int idx;
    short* ptr;

    end = 0;
    idx = 0;
    ptr = tim;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 16; j++) {
            if (str[idx] == 0) {
                end = 1;
            }
            if (!end) {
                *ptr++ = str[idx++];
            } else {
                *ptr++ = 0;
            }
        }
        *ptr++ = 0;
    }
    ptr = tim;
    tim = (char*)tim + 0x44;
    biosStrLen = 0;

    for (i = 0; i < 2; i++) {
        Krom2Tim(&ptr[i * 17], tim, i * 64, 0xf0, 0x270, 0xf0, fg, bg);
        j = tim->imageLength - 12;
        if (j > 0) {
            biosStrLen += j / 128;
            for (j = 0; j < 16; j++) {
                if (tim->clutData[j]) {
                    tim->clutData[j] |= 0x8000;
                }
            }
            rect.x = tim->clutX;
            rect.y = tim->clutY;
            rect.w = tim->clutWidth;
            rect.h = tim->clutHeight;
            DrawSync(0);
            LoadImage(&rect, tim->clutData);
            DrawSync(0);

            rect.x = tim->imageX;
            rect.y = tim->imageY;
            rect.w = tim->imageWidth;
            rect.h = tim->imageHeight;
            LoadImage(&rect, tim + 1);
        }
        if (biosStrLen < 16)
            break;
    }

    for (i = 0; i < biosStrLen; i++) {
        TSpritePrim(&biosStrSprites[0][i], 0, 0, GetTPage(0, param_7 == -1 ? 0 : param_7, (i / 16) << 6, 0xf0));
        setRGB0(&biosStrSprites[0][i].sprt, 0x80, 0x80, 0x80);
        setXY0(&biosStrSprites[0][i].sprt,
            x + ((i * 16) % (param_4 << 4)),
            y + (i / param_4) * 16);
        SetSemiTrans(&biosStrSprites[0][i].sprt, param_7 != -1);
        SetShadeTex(&biosStrSprites[0][i].sprt, 0);
        biosStrSprites[0][i].sprt.clut = GetClut(0x270,0xf0);
        biosStrSprites[0][i].sprt.w = 16;
        biosStrSprites[0][i].sprt.h = 16;
        setUV0(&biosStrSprites[0][i].sprt, (i * 16 % 256), 0xf0);

        biosStrSprites[1][i] = biosStrSprites[0][i];
    }
}

void DrawBiosFontString(void) {
    int i;

    for (i = 0; i < biosStrLen; i++) {
        addPrim(&primLists[whichDrawDispEnv].gui1, &biosStrSprites[whichDrawDispEnv][i]);
    }
}
