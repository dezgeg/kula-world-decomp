#include "common.h"

typedef struct Sparkle {
    uint ptrAndFlags;
    uint color;
    uint unk2;
    uint unk3;
    uint unk4;
} Sparkle;

typedef struct GemSparkleEffect {
    uint* vertexColors;
    int numColors;
    int probability;
    int addRgb;
    int maxRgb;
    int subRgb;
    int numSparkles;
    Sparkle sparkles[16];
} GemSparkleEffect;

// non-gprel-used variables (extern)
extern GemSparkleEffect GemRandomSparkleEffect_ARRAY_ARRAY_000dd760[3][3];
extern Texture textures[150];

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

// Prototypes
void ParseTextures(int* headerPtr, Texture * out, int count);
void ParseGgiInner(int *eff, int modelType, int modelIdx, int i, int j, int probability, int addRgb, int maxRgb, int subRgb, int numSparkles);

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

void ParseTextures(int* headerPtr, Texture *out, int unused_count) {
    int count = *headerPtr;
    short *p;
    int i;
    int blendMode;
    int bpp;
    int alreadyLoaded;
    RECT rect;

    DrawSync(0);
    p = (short *)(headerPtr + 1);
    for (i = 0; i < count; i++) {
        bpp = *p++;
        if (bpp == -1) {
            // padding word
            p++;
            continue;
        }

        out[i].bitsPerPixel = bpp;
        blendMode = *p++;
        out[i].semitrans = (blendMode != -1);
        if (blendMode == -1) blendMode = 0;

        alreadyLoaded = p[3];

        if (bpp != 16) {
            rect.x = *p++;
            rect.y = *p++;
            out[i].clut = GetClut(rect.x, rect.y);
            if (p[0] == 0) {
                rect.w = 1 << bpp;
                rect.h = 1;
                LoadImage(&rect, p + 2);
                DrawSync(0);
                p += rect.w + 2;
            } else {
                // CLUT already loaded by some other texture
                p += 2;
            }
        } else {
            p += 4;
        }

        rect.x = *p++;
        rect.y = *p++;
        out[i].w = *p++;
        rect.w = (out[i].w * bpp) / 16;
        out[i].h = *p++;
        rect.h = out[i].h;

        if (alreadyLoaded == 0) {
            LoadImage(&rect, p);
            DrawSync(0);
            p += rect.w * rect.h;
            if ((rect.w * rect.h) & 1) {
                p++;
            }
        }

        out[i].tpage = GetTPage(bpp >> 3, blendMode, rect.x, rect.y);
        out[i].u = (rect.x % 64) * (16 / bpp);
        out[i].v = rect.y % 256;
    }
}

void ParseGgiInner(int *eff, int modelType, int modelIdx, int lodIdx, int variantIdx, int probability,
                   int addRgb, int maxRgb, int subRgb, int numSparkles) {
    int* p;
    int k;

    // Calculate pointer to ModelData
    if (modelType == 0) {
        p = (int*)((char*)ggiPart0A + (*(int*)((char*)ggiPart0A + modelIdx * 16 + lodIdx * 4) / 4) * 4);
    } else {
        p = (int*)((char*)ggiPart0B + (*(int*)((char*)ggiPart0B + modelIdx * 64 + lodIdx * 16 + variantIdx * 4) / 4) * 4);
    }

    // Advance to VertexColors structure
    p = (int*)((char*)p + (p[5] / 4) * 4);

    eff[0] = (uint*)((char*)p + 8); // pointer to colors array
    eff[1] = p[1] / 16; // number of vertex colors
    eff[2] = probability;
    eff[3] = addRgb;
    eff[4] = maxRgb;
    eff[5] = subRgb;
    eff[6] = numSparkles;
    eff += 7;

    for (k = 0; k < numSparkles; k++,eff += 5) {
        eff[0] = 0;
    }
}
