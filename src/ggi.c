#include "common.h"

void* ggiPart0A;
void* ggiPart0B;

typedef struct LocalSparkleEntry {
    uint ptr_and_flags;
    uint color;
    uint unk2;
    uint unk3;
    uint unk4;
} LocalSparkleEntry;

typedef struct LocalGemRandomSparkleEffect {
    uint* ggiPtr;
    int field1_0x4;
    int const3;
    int addRgb;
    int maxRgb;
    int subRgb;
    int modelType6;
    LocalSparkleEntry sparkles[16];
} LocalGemRandomSparkleEffect;

INCLUDE_ASM("asm/nonmatchings/ggi", ParseGGI);

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
