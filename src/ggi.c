#include "common.h"

typedef struct GgiFile {
    int numFruitTextures;
    int numParticleTextures;
    int numUnk1Textures;
    int numGuiTextTextures;
    int numUnk2Textures;
    int numBonusWidgetTextures;
    int part0Len;
    int part1Len;
    int part2Len;
    int part3Len;
    int part4Len;
    int part5Len;
    int part6Len;
    int part0Aoff;
    int part0Boff;
    int pad[5];
} GgiFile;

typedef struct LocalSparkleEntry {
    uint ptr_and_flags;
    uint color;
    uint unk2;
    uint unk3;
    uint unk4;
} LocalSparkleEntry;

typedef struct LocalGemRandomSparkleEffect {
    uint* ggiPtr;
    int unknown_4;
    int const3;
    int addRgb;
    int maxRgb;
    int subRgb;
    int const16;
    LocalSparkleEntry sparkles[16];
} LocalGemRandomSparkleEffect;

// gprel-used variables (defined in this file)
int firstBonusWidgetTexture;
int firstFruitTexture;
int firstGuiTextTexture;
uint firstGuiTexture;
int firstLensFlareOrPsxButtonTexture;
int firstParticleTexture;
int firstUnk2Texture;
GgiFile * ggi;
void* ggiPart0A;
void* ggiPart0B;
void* ggiPart1HourglassAnim;
void* ggiPart2DepthCueingLookup;
void* ggiPart3;
void* ggiPart4Dummy;
void* ggiPart5JumpAnimData;
void* ggiPart6Textures;
void* ggiPart7Unused;
int numBonusWidgetTextures;
int numGuiTextTextures;
int numParticleTextures;
int numUnk1Textures;
int numUnk2Textures;

// non-gprel-used variables (extern)
extern LocalGemRandomSparkleEffect GemRandomSparkleEffect_ARRAY_ARRAY_000dd760[3][3];
extern Texture textures[150];

// Prototypes
void ParseTextures(void* headerPtr, Texture * out, int count);
void ParseGgiInner(int *eff, int modelType, int const36, int i, int j, int const3, int addRgb, int maxRgb, int subRgb, int modelType6);

void* ParseGGI(GgiFile *ggi_ptr) {
    int i, j;
    int* pOff;

    ggi = ggi_ptr;

    pOff = (int*)&ggi_ptr->part0Aoff;
    ggiPart0A = (void*)((int)pOff + pOff[0] / 4 * 4);
    ggiPart0B = (void*)((int)pOff + pOff[1] / 4 * 4);

    ggiPart1HourglassAnim = (void*)((int)pOff + ggi_ptr->part0Len * 2);
    ggiPart2DepthCueingLookup = (void*)((int)ggiPart1HourglassAnim + ggi_ptr->part1Len * 2);
    ggiPart3 = (void*)((int)ggiPart2DepthCueingLookup + ggi_ptr->part2Len * 2);
    ggiPart4Dummy = (void*)((int)ggiPart3 + ggi_ptr->part3Len * 2);
    ggiPart5JumpAnimData = (void*)((int)ggiPart4Dummy + ggi_ptr->part4Len * 2);
    ggiPart6Textures = (void*)((int)ggiPart5JumpAnimData + ggi_ptr->part5Len * 2);
    ggiPart7Unused = (void*)((int)ggiPart6Textures + ggi_ptr->part6Len * 2);

    ParseTextures(ggiPart6Textures, textures, 150);

    firstFruitTexture = 0;
    firstParticleTexture = ((int*)ggi)[0];
    numParticleTextures = ((int*)ggi)[1];
    firstLensFlareOrPsxButtonTexture = firstParticleTexture + numParticleTextures;
    numUnk1Textures = ((int*)ggi)[2];
    firstGuiTextTexture = firstLensFlareOrPsxButtonTexture + numUnk1Textures;
    numGuiTextTextures = ((int*)ggi)[3];
    firstUnk2Texture = firstGuiTextTexture + numGuiTextTextures;
    numUnk2Textures = ((int*)ggi)[4];
    firstBonusWidgetTexture = firstUnk2Texture + numUnk2Textures;
    numBonusWidgetTextures = ((int*)ggi)[5];
    firstGuiTexture = firstBonusWidgetTexture + numBonusWidgetTextures;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            ParseGgiInner((int *)&GemRandomSparkleEffect_ARRAY_ARRAY_000dd760[i][j], 1, 36, i, j, 3, 0x808080, 0xfefefe, 0x101010, 0x10);
        }
    }

    return ggiPart6Textures;
}

INCLUDE_ASM("asm/nonmatchings/ggi", ParseTextures);

void ParseGgiInner(int *eff, int modelType, int const36, int i, int j, int const3,
                   int addRgb, int maxRgb, int subRgb, int modelType6) {
    int* p;
    int offset;
    int fromModelDataOff0x14;
    int unk;
    int k;
    LocalSparkleEntry* s;
    int* ptr;

    if (modelType == 0) {
        p = (int*)((char*)ggiPart0A + (*(int*)((char*)ggiPart0A + const36 * 16 + i * 4) / 4) * 4);
    } else {
        p = (int*)((char*)ggiPart0B + (*(int*)((char*)ggiPart0B + const36 * 64 + i * 16 + j * 4) / 4) * 4);
    }

    p = (int*)((char*)p + (p[5] / 4) * 4);

    eff[0] = (uint*)((char*)p + 8);
    eff[1] = p[1] / 16;
    eff[2] = const3;
    eff[3] = addRgb;
    eff[4] = maxRgb;
    eff[5] = subRgb;
    eff[6] = modelType6;
    eff += 7;

    for (k = 0; k < modelType6; k++,eff += 5) {
        eff[0] = 0;
    }
}
