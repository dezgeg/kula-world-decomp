#include "common.h"

#include "zlib.h"

// Prototypes
extern void PutDrawAndDispEnvs(void);
extern void SetupDisplay(u_char isbg, u_char bgR, u_char bgG, u_char bgB, u_char useDithering, u_char use24Bit);
extern void TSpritePrim(TSprite* ts, int dfe, int dtd, int tpage);

// gprel-used variables (defined in this file)
int inflateRetCode;
int whichLevelEndSpriteLoaded;
z_stream zlibStream_a4dd4; // also used non-gprel

// non-gprel-used variables (extern)
extern int displayWidth;
extern int* MENU_DEFLATED_SPRITES_PTR2;
extern int whichDrawDispEnv;
extern MemcardData memCardData;
extern PrimList primLists[2];
extern TSprite loadGameSprite1[2];
extern TSprite loadGameSprite2[2];
extern TSprite saveBackButtonSprite1[2];
extern TSprite saveBackButtonSprite2[2];
extern TSprite saveGameSprite1[2];
extern TSprite saveGameSprite2[2];
extern TSprite saveSelectButtonSprite1[2];
extern TSprite saveSelectButtonSprite2[2];
extern TSprite saveSlot0Sprite1[2];
extern TSprite saveSlot0Sprite2[2];
extern TSprite saveSlot1Sprite1[2];
extern TSprite saveSlot1Sprite2[2];
extern TSprite saveSlot2Sprite1[2];
extern TSprite saveSlot2Sprite2[2];
extern TSprite saveSlot3Sprite1[2];
extern TSprite saveSlot3Sprite2[2];

extern char S_Fatal_error_in_jens_2d_eng[];
extern char S_sign_too_high[];
extern char S_sign_nr_too_big[];
extern char S_1_0_4[];

void InitMemcardUi(void) {
    RECT rect;
    short h;
    short w;
    int tex = 26;
    int len;
    int len2;
    int offset;
    int offset2;
    char* buf;
    char* buf2;

    if (whichLevelEndSpriteLoaded != 200) {
        len = MENU_DEFLATED_SPRITES_PTR2[2 + tex * 2];
        offset = MENU_DEFLATED_SPRITES_PTR2[1 + tex * 2];
        buf = (char *) MENU_DEFLATED_SPRITES_PTR2 + offset;
        zlibStream_a4dd4.avail_in = len;
        zlibStream_a4dd4.next_in = buf;
        zlibStream_a4dd4.avail_out = 0x10000;
        zlibStream_a4dd4.next_out = 0x1EA000;
        inflateRetCode = inflateInit_(&zlibStream_a4dd4, S_1_0_4, 0x38);
        inflateRetCode = inflate(&zlibStream_a4dd4, 4);
        inflateRetCode = inflateEnd(&zlibStream_a4dd4);
        w = *(short*)0x1EA03C;
        h = *(short*)0x1EA03E;
        if (h + 94 > 0xff) {
            SetupDisplay(1, 0x80, 0, 0, 0, 0);
            FntFlush(-1);
            DrawSync(0);
            whichDrawDispEnv = 0;
            PutDrawAndDispEnvs();
            FntPrint(S_Fatal_error_in_jens_2d_eng);
            FntPrint(S_sign_too_high);
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
        LoadImage(&rect,0x1EA014);
        DrawSync(0);
        rect.x = 704;
        rect.y = 0x5f;
        rect.w = w;
        rect.h = h;
        LoadImage(&rect,0x1EA040);
        DrawSync(0);
        tex++;
        if (MENU_DEFLATED_SPRITES_PTR2[0] < tex) {
            SetupDisplay(1, 0x80, 0, 0, 0, 0);
            FntFlush(-1);
            DrawSync(0);
            whichDrawDispEnv = 0;
            PutDrawAndDispEnvs();
            FntPrint(S_Fatal_error_in_jens_2d_eng);
            FntPrint(S_sign_nr_too_big);
            FntFlush(-1);
            whichDrawDispEnv = 1;
            PutDrawAndDispEnvs();
            for (;;)
                ;
        }
        len2 = MENU_DEFLATED_SPRITES_PTR2[2 + tex * 2];
        offset2 = MENU_DEFLATED_SPRITES_PTR2[1 + tex * 2]  & 0xFFFFFFFFFFFFFFFFu; // XXX permuter mess
        buf = (char *) MENU_DEFLATED_SPRITES_PTR2 + offset2;
        zlibStream_a4dd4.avail_in = len2;
        zlibStream_a4dd4.next_in = buf;
        zlibStream_a4dd4.avail_out = 0x10000;
        zlibStream_a4dd4.next_out = 0x1EA000;
        inflateRetCode = inflateInit_(&zlibStream_a4dd4, S_1_0_4, 0x38);
        inflateRetCode = inflate(&zlibStream_a4dd4, 4);
        inflateRetCode = inflateEnd(&zlibStream_a4dd4);
        w = *(short*)0x1EA03C;
        h = *(short*)0x1EA03E;
        rect.x = 704;
        rect.y = 175;
        rect.w = 16;
        rect.h = 1;
        LoadImage(&rect,0x1EA014);
        DrawSync(0);
        rect.x = 704;
        rect.y = 176;
        rect.w = w;
        rect.h = h;
        LoadImage(&rect,0x1EA040);
        DrawSync(0);

        TSpritePrim(loadGameSprite2, 0, 0, GetTPage(0, 2, 0x2C0, 0x5E));
        SetSemiTrans(&loadGameSprite2->sprt, 2);
        SetShadeTex(&loadGameSprite2->sprt, 0);
        loadGameSprite2[0].sprt.clut = GetClut(0x2C0, 0x5E);
        loadGameSprite2[0].sprt.r0 = 0x80;
        loadGameSprite2[0].sprt.g0 = 0x80;
        loadGameSprite2[0].sprt.b0 = 0x80;
        loadGameSprite2[0].sprt.u0 = 0;
        loadGameSprite2[0].sprt.v0 = 0x5F;
        loadGameSprite2[0].sprt.x0 = (displayWidth / 2) - 0x3E;
        loadGameSprite2[0].sprt.y0 = 0xD;
        loadGameSprite2[0].sprt.w = 0x7C;
        loadGameSprite2[0].sprt.h = 0x18;
        loadGameSprite2[1] = loadGameSprite2[0];

        saveGameSprite2[0] = loadGameSprite2[1];
        saveGameSprite2[0].sprt.u0 = 0x7C;
        saveGameSprite2[0].sprt.v0 = 0x5F;
        saveGameSprite2[0].sprt.x0 = (displayWidth / 2) - 0x3F;
        saveGameSprite2[0].sprt.y0 = 0xD;
        saveGameSprite2[0].sprt.w = 0x7E;
        saveGameSprite2[0].sprt.h = 0x18;
        saveGameSprite2[1] = saveGameSprite2[0];

        saveSlot0Sprite2[0] = saveGameSprite2[1];
        saveSlot0Sprite2[0].sprt.u0 = 0;
        saveSlot0Sprite2[0].sprt.v0 = 0x7C;
        saveSlot0Sprite2[0].sprt.x0 = 0x22;
        saveSlot0Sprite2[0].sprt.y0 = 0x2D;
        saveSlot0Sprite2[0].sprt.w = 0x2A;
        saveSlot0Sprite2[0].sprt.h = 0x27;
        saveSlot0Sprite2[1] = saveSlot0Sprite2[0];

        saveSlot1Sprite2[0] = saveSlot0Sprite2[1];
        saveSlot1Sprite2[0].sprt.u0 = 0x2A;
        saveSlot1Sprite2[0].sprt.v0 = 0x7C;
        saveSlot1Sprite2[0].sprt.x0 = 0x68;
        saveSlot1Sprite2[0].sprt.y0 = 0x2D;
        saveSlot1Sprite2[0].sprt.w = 0x2A;
        saveSlot1Sprite2[0].sprt.h = 0x27;
        saveSlot1Sprite2[1] = saveSlot1Sprite2[0];

        saveSlot2Sprite2[0] = saveSlot1Sprite2[1];
        saveSlot2Sprite2[0].sprt.u0 = 0x54;
        saveSlot2Sprite2[0].sprt.v0 = 0x7C;
        saveSlot2Sprite2[0].sprt.x0 = 0xAE;
        saveSlot2Sprite2[0].sprt.y0 = 0x2D;
        saveSlot2Sprite2[0].sprt.w = 0x2A;
        saveSlot2Sprite2[0].sprt.h = 0x27;
        saveSlot2Sprite2[1] = saveSlot2Sprite2[0];

        saveSlot3Sprite2[0] = saveSlot2Sprite2[1];
        saveSlot3Sprite2[0].sprt.u0 = 0x7E;
        saveSlot3Sprite2[0].sprt.v0 = 0x7C;
        saveSlot3Sprite2[0].sprt.x0 = 0xF4;
        saveSlot3Sprite2[0].sprt.y0 = 0x2D;
        saveSlot3Sprite2[0].sprt.w = 0x2A;
        saveSlot3Sprite2[0].sprt.h = 0x27;
        saveSlot3Sprite2[1] = saveSlot3Sprite2[0];

        saveSelectButtonSprite2[0] = saveSlot3Sprite2[1];
        saveSelectButtonSprite2[0].sprt.u0 = 0xA8;
        saveSelectButtonSprite2[0].sprt.v0 = 0x77;
        saveSelectButtonSprite2[0].sprt.x0 = (displayWidth / 2) - 0x50;
        saveSelectButtonSprite2[0].sprt.y0 = 0xE5;
        saveSelectButtonSprite2[0].sprt.w = 0x46;
        saveSelectButtonSprite2[0].sprt.h = 0x16;
        saveSelectButtonSprite2[1] = saveSelectButtonSprite2[0];

        saveBackButtonSprite2[0] = saveSelectButtonSprite2[1];
        saveBackButtonSprite2[0].sprt.v0 = 0x8D;
        saveBackButtonSprite2[0].sprt.u0 = 0xA8;
        saveBackButtonSprite2[0].sprt.x0 = (displayWidth / 2) + 0x14;
        saveBackButtonSprite2[0].sprt.y0 = 0xE5;
        saveBackButtonSprite2[0].sprt.w = 0x3E;
        saveBackButtonSprite2[0].sprt.h = 0x16;
        saveBackButtonSprite2[1] = saveBackButtonSprite2[0];

        TSpritePrim(loadGameSprite1, 0, 0, GetTPage(0, 1, 0x2C0, 0xAF));
        SetSemiTrans(&loadGameSprite1->sprt, 1);
        SetShadeTex(&loadGameSprite1->sprt, 0);
        loadGameSprite1[0].sprt.clut = GetClut(0x2C0, 0xAF);
        loadGameSprite1[0].sprt.r0 = 0x80;
        loadGameSprite1[0].sprt.g0 = 0x80;
        loadGameSprite1[0].sprt.b0 = 0x80;
        loadGameSprite1[0].sprt.u0 = 0;
        loadGameSprite1[0].sprt.v0 = 0xB0;
        loadGameSprite1[0].sprt.x0 = (displayWidth / 2) - 0x3E;
        loadGameSprite1[0].sprt.y0 = 0xD;
        loadGameSprite1[0].sprt.w = 0x7C;
        loadGameSprite1[0].sprt.h = 0x18;
        loadGameSprite1[1] = loadGameSprite1[0];

        saveGameSprite1[0] = loadGameSprite1[1];
        saveGameSprite1[0].sprt.u0 = 0x7C;
        saveGameSprite1[0].sprt.v0 = 0xB0;
        saveGameSprite1[0].sprt.x0 = (displayWidth / 2) - 0x3F;
        saveGameSprite1[0].sprt.y0 = 0xD;
        saveGameSprite1[0].sprt.w = 0x7E;
        saveGameSprite1[0].sprt.h = 0x18;
        saveGameSprite1[1] = saveGameSprite1[0];

        saveSlot0Sprite1[0] = saveGameSprite1[1];
        saveSlot0Sprite1[0].sprt.u0 = 0;
        saveSlot0Sprite1[0].sprt.v0 = 0xCD;
        saveSlot0Sprite1[0].sprt.x0 = 0x22;
        saveSlot0Sprite1[0].sprt.y0 = 0x2D;
        saveSlot0Sprite1[0].sprt.w = 0x2A;
        saveSlot0Sprite1[0].sprt.h = 0x27;
        saveSlot0Sprite1[1] = saveSlot0Sprite1[0];

        saveSlot1Sprite1[0] = saveSlot0Sprite1[1];
        saveSlot1Sprite1[0].sprt.u0 = 0x2A;
        saveSlot1Sprite1[0].sprt.v0 = 0xCD;
        saveSlot1Sprite1[0].sprt.x0 = 0x68;
        saveSlot1Sprite1[0].sprt.y0 = 0x2D;
        saveSlot1Sprite1[0].sprt.w = 0x2A;
        saveSlot1Sprite1[0].sprt.h = 0x27;
        saveSlot1Sprite1[1] = saveSlot1Sprite1[0];

        saveSlot2Sprite1[0] = saveSlot1Sprite1[1];
        saveSlot2Sprite1[0].sprt.u0 = 0x54;
        saveSlot2Sprite1[0].sprt.v0 = 0xCD;
        saveSlot2Sprite1[0].sprt.x0 = 0xAE;
        saveSlot2Sprite1[0].sprt.y0 = 0x2D;
        saveSlot2Sprite1[0].sprt.w = 0x2A;
        saveSlot2Sprite1[0].sprt.h = 0x27;
        saveSlot2Sprite1[1] = saveSlot2Sprite1[0];

        saveSlot3Sprite1[0] = saveSlot2Sprite1[1];
        saveSlot3Sprite1[0].sprt.u0 = 0x7E;
        saveSlot3Sprite1[0].sprt.v0 = 0xCD;
        saveSlot3Sprite1[0].sprt.x0 = 0xF4;
        saveSlot3Sprite1[0].sprt.y0 = 0x2D;
        saveSlot3Sprite1[0].sprt.w = 0x2A;
        saveSlot3Sprite1[0].sprt.h = 0x27;
        saveSlot3Sprite1[1] = saveSlot3Sprite1[0];

        saveSelectButtonSprite1[0] = saveSlot3Sprite1[1];
        saveSelectButtonSprite1[0].sprt.u0 = 0xA8;
        saveSelectButtonSprite1[0].sprt.v0 = 0xC8;
        saveSelectButtonSprite1[0].sprt.x0 = (displayWidth / 2) - 0x50;
        saveSelectButtonSprite1[0].sprt.y0 = 0xE5;
        saveSelectButtonSprite1[0].sprt.w = 0x46;
        saveSelectButtonSprite1[0].sprt.h = 0x16;
        saveSelectButtonSprite1[1] = saveSelectButtonSprite1[0];

        saveBackButtonSprite1[0] = saveSelectButtonSprite1[1];
        saveBackButtonSprite1[0].sprt.v0 = 0xDE;
        saveBackButtonSprite1[0].sprt.u0 = 0xA8;
        saveBackButtonSprite1[0].sprt.x0 = (displayWidth / 2) + 0x14;
        saveBackButtonSprite1[0].sprt.y0 = 0xE5;
        saveBackButtonSprite1[0].sprt.w = 0x3E;
        saveBackButtonSprite1[0].sprt.h = 0x16;
        saveBackButtonSprite1[1] = saveBackButtonSprite1[0];
        whichLevelEndSpriteLoaded = 0xC8;
    }
}

void DrawSaveSlotSprites(int isSave) {
    if (isSave == 0) {
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
