#include "common.h"
#include "zlib.h"

// Prototypes
extern void PutDrawAndDispEnvs(void);
extern void SetupDisplay(u_char isbg, u_char bgR, u_char bgG, u_char bgB, u_char useDithering, u_char use24Bit);
extern void TSpritePrim(TSprite* ts, int dfe, int dtd, int tpage);

// non-gprel-used variables (extern)
extern int displayWidth;
extern int* MENU_DEFLATED_SPRITES2_PTR;
extern int whichDrawDispEnv;
extern MemcardData memCardData;
extern PrimList primLists[2];
extern char S_Fatal_error_in_jens_2d_eng[];
extern char S_1_0_4[];

int inflateRetCode;
int whichLevelEndSpriteLoaded;
TSprite loadGameSprite1[2];
TSprite loadGameSprite2[2];
TSprite saveBackButtonSprite1[2];
TSprite saveBackButtonSprite2[2];
TSprite saveGameSprite1[2];
TSprite saveGameSprite2[2];
TSprite saveSelectButtonSprite1[2];
TSprite saveSelectButtonSprite2[2];
TSprite saveSlot0Sprite1[2];
TSprite saveSlot0Sprite2[2];
TSprite saveSlot1Sprite1[2];
TSprite saveSlot1Sprite2[2];
TSprite saveSlot2Sprite1[2];
TSprite saveSlot2Sprite2[2];
TSprite saveSlot3Sprite1[2];
TSprite saveSlot3Sprite2[2];
z_stream zlibStream_a4dd4;

void InitMemcardUi(void) {
    RECT rect;
    short h;
    short w;
    int tex = LEVEL_END_GFX_MEMCARD_MENU;
    int len;
    int len2;
    int offset;
    int offset2;
    char* buf;
    char* buf2;

    if (whichLevelEndSpriteLoaded != LEVEL_END_GFX_MEMCARD_LOADED) {
        len = MENU_DEFLATED_SPRITES2_PTR[2 + tex * 2];
        offset = MENU_DEFLATED_SPRITES2_PTR[1 + tex * 2];
        buf = (char*)MENU_DEFLATED_SPRITES2_PTR + offset;
        zlibStream_a4dd4.avail_in = len;
        zlibStream_a4dd4.next_in = buf;
        zlibStream_a4dd4.avail_out = 0x10000;
        zlibStream_a4dd4.next_out = TIM_DECOMP_BUF;
        inflateRetCode = inflateInit_(&zlibStream_a4dd4, S_1_0_4, 0x38);
        inflateRetCode = inflate(&zlibStream_a4dd4, 4);
        inflateRetCode = inflateEnd(&zlibStream_a4dd4);
        w = *(short*)((char*)TIM_DECOMP_BUF + 0x3C);
        h = *(short*)((char*)TIM_DECOMP_BUF + 0x3E);
        if (h + 94 > 0xff) {
            SetupDisplay(1, 0x80, 0, 0, 0, 0);
            FntFlush(-1);
            DrawSync(0);
            whichDrawDispEnv = 0;
            PutDrawAndDispEnvs();
            FntPrint(S_Fatal_error_in_jens_2d_eng);
            FntPrint("sign too high");
            FntFlush(-1);
            whichDrawDispEnv = 1;
            PutDrawAndDispEnvs();
            for (;;)
                ;
        }
        rect.x = 704;
        rect.y = 94;
        rect.h = 1;
        rect.w = 16;
        DrawSync(0);
        LoadImage(&rect, (u_long*)((char*)TIM_DECOMP_BUF + 0x14));
        DrawSync(0);
        rect.x = 704;
        rect.y = 0x5f;
        rect.w = w;
        rect.h = h;
        LoadImage(&rect, (u_long*)((char*)TIM_DECOMP_BUF + 0x40));
        DrawSync(0);
        tex++;
        if (tex > MENU_DEFLATED_SPRITES2_PTR[0]) {
            SetupDisplay(1, 0x80, 0, 0, 0, 0);
            FntFlush(-1);
            DrawSync(0);
            whichDrawDispEnv = 0;
            PutDrawAndDispEnvs();
            FntPrint(S_Fatal_error_in_jens_2d_eng);
            FntPrint("sign nr too big");
            FntFlush(-1);
            whichDrawDispEnv = 1;
            PutDrawAndDispEnvs();
            for (;;)
                ;
        }
        len2 = MENU_DEFLATED_SPRITES2_PTR[2 + tex * 2];
        offset2 = MENU_DEFLATED_SPRITES2_PTR[1 + tex * 2];
        buf = (char*)MENU_DEFLATED_SPRITES2_PTR + offset2;
        zlibStream_a4dd4.avail_in = len2;
        zlibStream_a4dd4.next_in = buf;
        zlibStream_a4dd4.avail_out = 0x10000;
        zlibStream_a4dd4.next_out = TIM_DECOMP_BUF;
        inflateRetCode = inflateInit_(&zlibStream_a4dd4, S_1_0_4, 0x38);
        inflateRetCode = inflate(&zlibStream_a4dd4, 4);
        inflateRetCode = inflateEnd(&zlibStream_a4dd4);
        w = *(short*)((char*)TIM_DECOMP_BUF + 0x3C);
        h = *(short*)((char*)TIM_DECOMP_BUF + 0x3E);
        rect.x = 704;
        rect.y = 175;
        rect.w = 16;
        rect.h = 1;
        LoadImage(&rect, (u_long*)((char*)TIM_DECOMP_BUF + 0x14));
        DrawSync(0);
        rect.x = 704;
        rect.y = 176;
        rect.w = w;
        rect.h = h;
        LoadImage(&rect, (u_long*)((char*)TIM_DECOMP_BUF + 0x40));
        DrawSync(0);

        TSpritePrim(loadGameSprite2, 0, 0, GetTPage(0, 2, 0x2C0, 0x5E));
        SetSemiTrans(&loadGameSprite2->sprt, 2);
        SetShadeTex(&loadGameSprite2->sprt, 0);
        loadGameSprite2[0].sprt.clut = GetClut(0x2C0, 0x5E);
        setRGB0(&loadGameSprite2[0].sprt, 0x80, 0x80, 0x80);
        setUV0(&loadGameSprite2[0].sprt, 0, 95);
        setXY0(&loadGameSprite2[0].sprt, (displayWidth / 2) - 62, 13);
        loadGameSprite2[0].sprt.w = 124;
        loadGameSprite2[0].sprt.h = 24;
        loadGameSprite2[1] = loadGameSprite2[0];

        saveGameSprite2[0] = loadGameSprite2[1];
        setUV0(&saveGameSprite2[0].sprt, 124, 95);
        setXY0(&saveGameSprite2[0].sprt, (displayWidth / 2) - 63, 13);
        saveGameSprite2[0].sprt.w = 126;
        saveGameSprite2[0].sprt.h = 24;
        saveGameSprite2[1] = saveGameSprite2[0];

        saveSlot0Sprite2[0] = saveGameSprite2[1];
        setUV0(&saveSlot0Sprite2[0].sprt, 0, 124);
        setXY0(&saveSlot0Sprite2[0].sprt, 34, 45);
        saveSlot0Sprite2[0].sprt.w = 42;
        saveSlot0Sprite2[0].sprt.h = 39;
        saveSlot0Sprite2[1] = saveSlot0Sprite2[0];

        saveSlot1Sprite2[0] = saveSlot0Sprite2[1];
        setUV0(&saveSlot1Sprite2[0].sprt, 42, 124);
        setXY0(&saveSlot1Sprite2[0].sprt, 104, 45);
        saveSlot1Sprite2[0].sprt.w = 42;
        saveSlot1Sprite2[0].sprt.h = 39;
        saveSlot1Sprite2[1] = saveSlot1Sprite2[0];

        saveSlot2Sprite2[0] = saveSlot1Sprite2[1];
        setUV0(&saveSlot2Sprite2[0].sprt, 84, 124);
        setXY0(&saveSlot2Sprite2[0].sprt, 174, 45);
        saveSlot2Sprite2[0].sprt.w = 42;
        saveSlot2Sprite2[0].sprt.h = 39;
        saveSlot2Sprite2[1] = saveSlot2Sprite2[0];

        saveSlot3Sprite2[0] = saveSlot2Sprite2[1];
        setUV0(&saveSlot3Sprite2[0].sprt, 126, 124);
        setXY0(&saveSlot3Sprite2[0].sprt, 244, 45);
        saveSlot3Sprite2[0].sprt.w = 42;
        saveSlot3Sprite2[0].sprt.h = 39;
        saveSlot3Sprite2[1] = saveSlot3Sprite2[0];

        saveSelectButtonSprite2[0] = saveSlot3Sprite2[1];
        setUV0(&saveSelectButtonSprite2[0].sprt, 168, 119);
        setXY0(&saveSelectButtonSprite2[0].sprt, (displayWidth / 2) - 80, 229 - 17 * VID_NTSC);
        saveSelectButtonSprite2[0].sprt.w = 70;
        saveSelectButtonSprite2[0].sprt.h = 22;
        saveSelectButtonSprite2[1] = saveSelectButtonSprite2[0];

        saveBackButtonSprite2[0] = saveSelectButtonSprite2[1];
        setUV0(&saveBackButtonSprite2[0].sprt, 168, 141);
        setXY0(&saveBackButtonSprite2[0].sprt, (displayWidth / 2) + 20, 229 - 17 * VID_NTSC);
        saveBackButtonSprite2[0].sprt.w = 62;
        saveBackButtonSprite2[0].sprt.h = 22;
        saveBackButtonSprite2[1] = saveBackButtonSprite2[0];

        TSpritePrim(loadGameSprite1, 0, 0, GetTPage(0, 1, 0x2C0, 0xAF));
        SetSemiTrans(&loadGameSprite1->sprt, 1);
        SetShadeTex(&loadGameSprite1->sprt, 0);
        loadGameSprite1[0].sprt.clut = GetClut(0x2C0, 0xAF);
        loadGameSprite1[0].sprt.r0 = 0x80;
        loadGameSprite1[0].sprt.g0 = 0x80;
        loadGameSprite1[0].sprt.b0 = 0x80;
        setUV0(&loadGameSprite1[0].sprt, 0, 176);
        setXY0(&loadGameSprite1[0].sprt, (displayWidth / 2) - 62, 13);
        loadGameSprite1[0].sprt.w = 124;
        loadGameSprite1[0].sprt.h = 24;
        loadGameSprite1[1] = loadGameSprite1[0];

        saveGameSprite1[0] = loadGameSprite1[1];
        setUV0(&saveGameSprite1[0].sprt, 124, 176);
        setXY0(&saveGameSprite1[0].sprt, (displayWidth / 2) - 63, 13);
        saveGameSprite1[0].sprt.w = 126;
        saveGameSprite1[0].sprt.h = 24;
        saveGameSprite1[1] = saveGameSprite1[0];

        saveSlot0Sprite1[0] = saveGameSprite1[1];
        setUV0(&saveSlot0Sprite1[0].sprt, 0, 205);
        setXY0(&saveSlot0Sprite1[0].sprt, 34, 45);
        saveSlot0Sprite1[0].sprt.w = 42;
        saveSlot0Sprite1[0].sprt.h = 39;
        saveSlot0Sprite1[1] = saveSlot0Sprite1[0];

        saveSlot1Sprite1[0] = saveSlot0Sprite1[1];
        setUV0(&saveSlot1Sprite1[0].sprt, 42, 205);
        setXY0(&saveSlot1Sprite1[0].sprt, 104, 45);
        saveSlot1Sprite1[0].sprt.w = 42;
        saveSlot1Sprite1[0].sprt.h = 39;
        saveSlot1Sprite1[1] = saveSlot1Sprite1[0];

        saveSlot2Sprite1[0] = saveSlot1Sprite1[1];
        setUV0(&saveSlot2Sprite1[0].sprt, 84, 205);
        setXY0(&saveSlot2Sprite1[0].sprt, 174, 45);
        saveSlot2Sprite1[0].sprt.w = 42;
        saveSlot2Sprite1[0].sprt.h = 39;
        saveSlot2Sprite1[1] = saveSlot2Sprite1[0];

        saveSlot3Sprite1[0] = saveSlot2Sprite1[1];
        setUV0(&saveSlot3Sprite1[0].sprt, 126, 205);
        setXY0(&saveSlot3Sprite1[0].sprt, 244, 45);
        saveSlot3Sprite1[0].sprt.w = 42;
        saveSlot3Sprite1[0].sprt.h = 39;
        saveSlot3Sprite1[1] = saveSlot3Sprite1[0];

        saveSelectButtonSprite1[0] = saveSlot3Sprite1[1];
        setUV0(&saveSelectButtonSprite1[0].sprt, 168, 200);
        setXY0(&saveSelectButtonSprite1[0].sprt, (displayWidth / 2) - 80, 229 - 17 * VID_NTSC);
        saveSelectButtonSprite1[0].sprt.w = 70;
        saveSelectButtonSprite1[0].sprt.h = 22;
        saveSelectButtonSprite1[1] = saveSelectButtonSprite1[0];

        saveBackButtonSprite1[0] = saveSelectButtonSprite1[1];
        setUV0(&saveBackButtonSprite1[0].sprt, 168, 222);
        setXY0(&saveBackButtonSprite1[0].sprt, (displayWidth / 2) + 20, 229 - 17 * VID_NTSC);
        saveBackButtonSprite1[0].sprt.w = 62;
        saveBackButtonSprite1[0].sprt.h = 22;
        whichLevelEndSpriteLoaded = LEVEL_END_GFX_MEMCARD_LOADED;
        saveBackButtonSprite1[1] = saveBackButtonSprite1[0];
    }
}

void DrawSaveSlotSprites(int isSave) {
    if (!isSave) {
        addPrim(&primLists[whichDrawDispEnv].main, &loadGameSprite1[whichDrawDispEnv]);
        addPrim(&primLists[whichDrawDispEnv].main, &loadGameSprite2[whichDrawDispEnv]);
    } else {
        addPrim(&primLists[whichDrawDispEnv].main, &saveGameSprite1[whichDrawDispEnv]);
        addPrim(&primLists[whichDrawDispEnv].main, &saveGameSprite2[whichDrawDispEnv]);
    }
    if (!isSave && !memCardData.saveslots[0].valid) {
        setRGB0(&saveSlot0Sprite2[whichDrawDispEnv].sprt, 0x80, 0x80, 0x80);
        setRGB0(&saveSlot0Sprite1[whichDrawDispEnv].sprt, 0x80, 0, 0);
    } else {
        setRGB0(&saveSlot0Sprite2[whichDrawDispEnv].sprt, 0x80, 0x80, 0x80);
        setRGB0(&saveSlot0Sprite1[whichDrawDispEnv].sprt, 0x80, 0x80, 0x80);
    }

    if (!isSave && !memCardData.saveslots[1].valid) {
        setRGB0(&saveSlot1Sprite2[whichDrawDispEnv].sprt, 0x80, 0x80, 0x80);
        setRGB0(&saveSlot1Sprite1[whichDrawDispEnv].sprt, 0x80, 0, 0);
    } else {
        setRGB0(&saveSlot1Sprite2[whichDrawDispEnv].sprt, 0x80, 0x80, 0x80);
        setRGB0(&saveSlot1Sprite1[whichDrawDispEnv].sprt, 0x80, 0x80, 0x80);
    }

    if (!isSave && !memCardData.saveslots[2].valid) {
        setRGB0(&saveSlot2Sprite2[whichDrawDispEnv].sprt, 0x80, 0x80, 0x80);
        setRGB0(&saveSlot2Sprite1[whichDrawDispEnv].sprt, 0x80, 0, 0);
    } else {
        setRGB0(&saveSlot2Sprite2[whichDrawDispEnv].sprt, 0x80, 0x80, 0x80);
        setRGB0(&saveSlot2Sprite1[whichDrawDispEnv].sprt, 0x80, 0x80, 0x80);
    }

    if (!isSave && !memCardData.saveslots[3].valid) {
        setRGB0(&saveSlot3Sprite2[whichDrawDispEnv].sprt, 0x80, 0x80, 0x80);
        setRGB0(&saveSlot3Sprite1[whichDrawDispEnv].sprt, 0x80, 0, 0);
    } else {
        setRGB0(&saveSlot3Sprite2[whichDrawDispEnv].sprt, 0x80, 0x80, 0x80);
        setRGB0(&saveSlot3Sprite1[whichDrawDispEnv].sprt, 0x80, 0x80, 0x80);
    }

    addPrim(&primLists[whichDrawDispEnv].main, &saveSlot0Sprite1[whichDrawDispEnv]);
    addPrim(&primLists[whichDrawDispEnv].main, &saveSlot0Sprite2[whichDrawDispEnv]);
    addPrim(&primLists[whichDrawDispEnv].main, &saveSlot1Sprite1[whichDrawDispEnv]);
    addPrim(&primLists[whichDrawDispEnv].main, &saveSlot1Sprite2[whichDrawDispEnv]);
    addPrim(&primLists[whichDrawDispEnv].main, &saveSlot2Sprite1[whichDrawDispEnv]);
    addPrim(&primLists[whichDrawDispEnv].main, &saveSlot2Sprite2[whichDrawDispEnv]);
    addPrim(&primLists[whichDrawDispEnv].main, &saveSlot3Sprite1[whichDrawDispEnv]);
    addPrim(&primLists[whichDrawDispEnv].main, &saveSlot3Sprite2[whichDrawDispEnv]);

    addPrim(&primLists[whichDrawDispEnv].main, &saveSelectButtonSprite1[whichDrawDispEnv]);
    addPrim(&primLists[whichDrawDispEnv].main, &saveSelectButtonSprite2[whichDrawDispEnv]);
    addPrim(&primLists[whichDrawDispEnv].main, &saveBackButtonSprite1[whichDrawDispEnv]);
    addPrim(&primLists[whichDrawDispEnv].main, &saveBackButtonSprite2[whichDrawDispEnv]);
}
