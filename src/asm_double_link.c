#include "common.h"

int saveRegsMakeOtagDoublyLinked[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int DAT_00073844[] = { 0, 0, 0, 0, 0, 0 };
int saveRegsUnusedAsmNoop[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

INCLUDE_ASM("asm/nonmatchings/asm_double_link", MakeOtagDoublyLinked);

INCLUDE_ASM("asm/nonmatchings/asm_double_link", FUN_00050468);

INCLUDE_ASM("asm/nonmatchings/asm_double_link", UnusedAsmNoop);
