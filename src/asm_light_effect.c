#include "common.h"

int saveRegsUpdateGemRandomSparkleEffect[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int sparkleRngSeed[] = { 1 };
int saveRegsProcessLightEffects[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int saveRegsProcessInvisibleBlockVisibility[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int SQUARE_ROOT_TABLE[] = {
    0x00004AFB, 0x7FFFFFFF, 0x0000B505, 0x00000000, 0x00003505, 0x3FFFFFFF, 0x00008000, 0x00000000,
    0x0000257E, 0x1FFFFFFF, 0x00005A82, 0x00000000, 0x00001A82, 0x0FFFFFFF, 0x00004000, 0x00000000,
    0x000012BF, 0x07FFFFFF, 0x00002D41, 0x00000000, 0x00000D41, 0x03FFFFFF, 0x00002000, 0x00000000,
    0x0000095F, 0x01FFFFFF, 0x000016A1, 0x00000000, 0x000006A1, 0x00FFFFFF, 0x00001000, 0x00000000,
    0x000004B0, 0x007FFFFF, 0x00000B50, 0x00000000, 0x00000350, 0x003FFFFF, 0x00000800, 0x00000000,
    0x00000258, 0x001FFFFF, 0x000005A8, 0x00000000, 0x000001A8, 0x000FFFFF, 0x00000400, 0x00000000,
    0x0000012C, 0x0007FFFF, 0x000002D4, 0x00000000, 0x000000D4, 0x0003FFFF, 0x00000200, 0x00000000,
    0x00000096, 0x0001FFFF, 0x0000016A, 0x00000000, 0x0000006A, 0x0000FFFF, 0x00000100, 0x00000000,
    0x0000004B, 0x00007FFF, 0x000000B5, 0x00000000, 0x00000035, 0x00003FFF, 0x00000080, 0x00000000,
    0x00000025, 0x00001FFF, 0x0000005B, 0x00000000, 0x0000001B, 0x00000FFF, 0x00000040, 0x00000000,
    0x00000013, 0x000007FF, 0x0000002D, 0x00000000, 0x0000000D, 0x000003FF, 0x00000020, 0x00000000,
    0x00000009, 0x000001FF, 0x00000017, 0x00000000, 0x00000007, 0x000000FF, 0x00000010, 0x00000000,
    0x00000005, 0x0000007F, 0x0000000B, 0x00000003, 0x0000003F, 0x00000008, 0x00000002, 0x0000001F,
    0x00000006, 0x00000002, 0x0000000F, 0x00000004, 0x00000001, 0x00000007, 0x00000003, 0x00000001,
    0x00000003, 0x00000002, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000,
};

#ifdef NON_MATCHING
void UpdateGemRandomSparkleEffect() {}
#else
INCLUDE_ASM("asm/nonmatchings/asm_light_effect", UpdateGemRandomSparkleEffect);
#endif

#ifdef NON_MATCHING
void ProcessLightEffects() {}
#else
INCLUDE_ASM("asm/nonmatchings/asm_light_effect", ProcessLightEffects);
#endif

#ifdef NON_MATCHING
void UpdateQuadColor() {}
#else
INCLUDE_ASM("asm/nonmatchings/asm_light_effect", UpdateQuadColor);
#endif

#ifdef NON_MATCHING
void ProcessEnabledLightEffects() {}
#else
INCLUDE_ASM("asm/nonmatchings/asm_light_effect", ProcessEnabledLightEffects);
#endif

#ifdef NON_MATCHING
void ProcessDisabledLightEffects() {}
#else
INCLUDE_ASM("asm/nonmatchings/asm_light_effect", ProcessDisabledLightEffects);
#endif

#ifdef NON_MATCHING
void ProcessInvisibleBlockVisibility() {}
#else
INCLUDE_ASM("asm/nonmatchings/asm_light_effect", ProcessInvisibleBlockVisibility);
#endif
