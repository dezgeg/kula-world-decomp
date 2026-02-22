#include "common.h"

extern uint Rand(int param_1);

typedef struct P {
    ulong* p0;
    ulong* p1;
} P;

extern POLY_FT4 shadowPrims[2][1][2][16];
extern POLY_FT4 specularPrims[2][1][16];
extern P shadowPrimPtrs[2][1][2][16];
extern P specularPrimPtrs[2][1][16];
extern Texture textures[150];
extern Texture textures[150];
extern uint firstGuiTexture;
extern uint firstGuiTexture;

TgiFile* tgi;

INCLUDE_ASM("asm/nonmatchings/level_init", ProcessLevelData);

INCLUDE_ASM("asm/nonmatchings/level_init", ProcessCubesIntoFaces);

int CoordHash(int x,int y,int z,int dir,int div,int mod) {
    switch(dir) {
    case 0:
        z = z + -0x100;
        break;
    case 1:
        x = x + 0x100;
        break;
    case 2:
        y = y + 0x100;
        break;
    case 3:
        y = y + -0x100;
        break;
    case 4:
        x = x + -0x100;
        break;
    case 5:
        z = z + 0x100;
    }
    return ((x + y + z) / (div * 3)) % mod;
}

int GetRandomTextureRotation(void) {
    int ret;

    switch (tgi->randomTileRotation) {
        case 0:
            return 0;
        case 1:
            ret = Rand(2);
            if (ret == 1) {
                ret = 2;
            }
            return ret;
        default:
            return Rand(4);
    }
}

INCLUDE_ASM("asm/nonmatchings/level_init", ParseKeysAndSpecialLevelFromItemData);

INCLUDE_ASM("asm/nonmatchings/level_init", ScanLevelDataForMovingBlocks1);

INCLUDE_ASM("asm/nonmatchings/level_init", InitLasers2);

INCLUDE_ASM("asm/nonmatchings/level_init", ScanLevelDataForCrumblingBlocks);

INCLUDE_ASM("asm/nonmatchings/level_init", ScanLevelDataForFlashingBlocks);

INCLUDE_ASM("asm/nonmatchings/level_init", SetFaceData);

INCLUDE_ASM("asm/nonmatchings/level_init", CopyQuadData);

INCLUDE_ASM("asm/nonmatchings/level_init", InitAnimatedTextureChain);

INCLUDE_ASM("asm/nonmatchings/level_init", AddQuadToAnimatedTextureChain);

INCLUDE_ASM("asm/nonmatchings/level_init", FUN_000298e0);

void InitShadowSprites(void) {
    int i;
    int j;
    int k;
    int l;
    int tex;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 1; j++) {
            for (k = 0; k < 2; k++) {
                for (l = 0; l < 16; l++) {
                    tex = k + 14;
                    shadowPrimPtrs[i][j][k][l].p0 = 0xffffffff;
                    setPolyFT4(&shadowPrims[i][j][k][l]);
                    setUV4(&shadowPrims[i][j][k][l],
                            textures[firstGuiTexture + tex].u, textures[firstGuiTexture + tex].v,
                            textures[firstGuiTexture + tex].u + textures[firstGuiTexture + tex].w - 1, textures[firstGuiTexture + tex].v,
                            textures[firstGuiTexture + tex].u, textures[firstGuiTexture + tex].v + textures[firstGuiTexture + tex].h - 1,
                            textures[firstGuiTexture + tex].u + textures[firstGuiTexture + tex].w - 1, textures[firstGuiTexture + tex].v + textures[firstGuiTexture + tex].h - 1);
                    SetSemiTrans(&shadowPrims[i][j][k][l], textures[firstGuiTexture + tex].semitrans);
                    SetShadeTex(&shadowPrims[i][j][k][l], 0);
                    shadowPrims[i][j][k][l].tpage = textures[firstGuiTexture + tex].tpage;
                    shadowPrims[i][j][k][l].clut = textures[firstGuiTexture + tex].clut;
                }
            }
        }
    }


}

void InitPlayerSpecularSprite(void) {
    int i;
    int j;
    int k;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 1; j++) {
            for (k = 0; k < 16; k++) {
                specularPrimPtrs[i][j][k].p0 = 0xffffffff;
                setPolyFT4(&specularPrims[i][j][k]);
                setUV4(&specularPrims[i][j][k],
                        textures[firstGuiTexture + 18].u, textures[firstGuiTexture + 18].v,
                        textures[firstGuiTexture + 18].u + textures[firstGuiTexture + 18].w - 1, textures[firstGuiTexture + 18].v,
                        textures[firstGuiTexture + 18].u, textures[firstGuiTexture + 18].v + textures[firstGuiTexture + 18].h - 1,
                        textures[firstGuiTexture + 18].u + textures[firstGuiTexture + 18].w - 1, textures[firstGuiTexture + 18].v + textures[firstGuiTexture + 18].h - 1);
                SetSemiTrans(&specularPrims[i][j][k], textures[firstGuiTexture + 18].semitrans);
                SetShadeTex(&specularPrims[i][j][k], 0);
                setRGB0(&specularPrims[i][j][k], 0x80, 0x80, 0x80);
                specularPrims[i][j][k].tpage = textures[firstGuiTexture + 18].tpage;
                specularPrims[i][j][k].clut = textures[firstGuiTexture + 18].clut;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/level_init", ScanLevelDataForBlinkingEntities);
