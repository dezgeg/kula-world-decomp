#include "common.h"

int saveRegsMakeOtagDoublyLinked[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int DAT_00073844[] = { 0, 0, 0, 0, 0, 0 };
int saveRegsUnusedAsmNoop[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

#ifdef NON_MATCHING
void MakeOtagDoublyLinked() {}
#else
INCLUDE_ASM("asm/nonmatchings/asm_double_link", MakeOtagDoublyLinked);
#endif

#ifdef NON_MATCHING
void FUN_00050468() {}
#else
INCLUDE_ASM("asm/nonmatchings/asm_double_link", FUN_00050468);
#endif

#ifdef NON_MATCHING
void UnusedAsmNoop() {}
#else
INCLUDE_ASM("asm/nonmatchings/asm_double_link", UnusedAsmNoop);
#endif
