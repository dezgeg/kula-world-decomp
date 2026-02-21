#include "common.h"

int DAT_000a43c4;
extern void *entityData;
extern int FUN_00033720(SVECTOR *vec, int itemdataOff, int param_3);

int FUN_0003382c(Player *player) {
    int blockIndex;
    int scaledIndex;
    
    blockIndex = player->surroundingBlocks[1][1][1];
    scaledIndex = blockIndex - 5;
    DAT_000a43c4 = scaledIndex * 128;
    if (DAT_000a43c4 < 0 || *(short *)(scaledIndex * 256 + (int)entityData) != 5) {
        blockIndex = player->surroundingBlocks[2][1][1];
        scaledIndex = blockIndex - 5;
        DAT_000a43c4 = scaledIndex * 128;
        if (DAT_000a43c4 < 0 || *(short *)(scaledIndex * 256 + (int)entityData) != 5) {
            return 0;
        }
    }
    return FUN_00033720(&player->finePos, DAT_000a43c4, 100);
}
