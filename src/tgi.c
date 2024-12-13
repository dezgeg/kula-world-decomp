#include "common.h"

extern void InitCubeTextureMetadata(void);
extern void InitTurningMotionBlur(void);
extern void LoadImagesFromTgiPart9(short* p);

// gprel-used variables (defined in this file)
int recalcSkyboxes;
TgiFile* tgi;
short* tgiEnd;
short* tgiPart0;
short* tgiPart1;
short* tgiPart2;
short* tgiPart3;
short* tgiPart4;
short* tgiPart5;
short* tgiPart6;
short* tgiPart7;
short* tgiPart8;
short* tgiPart9;
short* tgiPart10;

extern DR_TPAGE drTpages1[2][1];
extern DR_TPAGE drTpages2[2][1];
extern DR_TPAGE hudDrTpages[2];

void* ParseTGI(TgiFile *tgiBuf) {
    int i;
    int j;
    short* p;
    RECT rect;

    tgi = tgiBuf;
    tgiPart0 = tgiBuf + 1;
    tgiPart1 = tgiPart0 + tgiBuf->part0Words;
    tgiPart2 = tgiPart1 + tgiBuf->part1Words;
    tgiPart3 = tgiPart2 + tgiBuf->part2Words;
    tgiPart4 = tgiPart3 + tgiBuf->part3Words;
    tgiPart5 = tgiPart4 + tgiBuf->part4Words;
    tgiPart6 = tgiPart5 + tgiBuf->part5Words;
    tgiPart7 = tgiPart6 + tgiBuf->part6Words;
    tgiPart8 = tgiPart7 + tgiBuf->part7Words;
    tgiPart9 = tgiPart8 + tgiBuf->part8Words;
    tgiPart10 = tgiPart9 + tgiBuf->part9Words;
    tgiEnd = tgiPart10 + tgiBuf->part10Words;
    InitTurningMotionBlur();
    InitCubeTextureMetadata();

    p = tgiPart10;
    DrawSync(0);
    while (p < tgiEnd) {
        rect.x = *p++;
        rect.y = *p++;
        rect.w = *p++;
        rect.h = *p++;
        LoadImage(&rect, p);
        DrawSync(0);
        p += rect.w * rect.h;
    }
    recalcSkyboxes = 1;
    if (tgi->skyboxFlag == 1025) {
        LoadImagesFromTgiPart9(tgiPart9 + 0x3f00 / 2);
    }

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 1; j++) {
            SetDrawTPage(&drTpages1[i][j],0,1,GetTPage(0,0,0,0));
            SetDrawTPage(&drTpages2[i][j],0,1,GetTPage(0,0,0,0));
        }
        SetDrawTPage(&hudDrTpages[i],0,0,GetTPage(0,0,0,0));
    }
    return tgiPart9 + 0x3f00 / 2;
}

INCLUDE_ASM("asm/nonmatchings/tgi", InitCubeTextureMetadata);

INCLUDE_ASM("asm/nonmatchings/tgi", GetHighscoreCubeStyle);

INCLUDE_ASM("asm/nonmatchings/tgi", LoadImagesFromTgiPart9);

INCLUDE_ASM("asm/nonmatchings/tgi", InitTurningMotionBlur);

INCLUDE_ASM("asm/nonmatchings/tgi", ParseLevelDataFromTgi);
