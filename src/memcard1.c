#include "common.h"

extern MemcardData memCardData;

void ClearMemcardData(void) {
    int i;
    char* p = &memCardData;
    for (i = 0; i < sizeof(memCardData); i++) {
        *p++ = 0;
    }
}

int GetMaxSaveSlotScore(void) {
    int i, j;
    int maxScore = 0;

    for (i = 0; i < 4; i++) {
        if (memCardData.saveslots[i].valid == 1) {
            for (j = 0; j < 150; j++) {
                if (memCardData.saveslots[i].levelScores[j] > maxScore) {
                    maxScore = memCardData.saveslots[i].levelScores[j];
                }
            }
        }
    }
    return maxScore;
}
