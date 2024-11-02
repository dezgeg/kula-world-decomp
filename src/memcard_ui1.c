#include "common.h"

extern TSprite loadGameSprite1[2];
extern TSprite loadGameSprite2[2];
extern MemcardData memCardData;
extern PrimList primLists[2];
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
extern int whichDrawDispEnv;

// https://decomp.me/scratch/ehGLH
INCLUDE_ASM("asm/nonmatchings/memcard_ui1", InitMemcardUi);

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
