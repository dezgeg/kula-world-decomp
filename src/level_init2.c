#include "common.h"

extern int retractingSpikeData[64*2];

short* entityData;
short numEntities;
int numRetractableSpikes;

#define IE_ID 1
#define IE_VARIANT 3
#define IE_STATE 4
#define IE_UNK8 8
#define IE_COUNTER 15
#define IE_UNK16 16

void ScanLevelDataForRetractableSpikes(void) {
    int i;
    int j;
    int variant;

    numRetractableSpikes = 0;
    for (i = 0; i < numEntities; i++) {
        if (entityData[i * 128] != 0) {
            continue;
        }
        for (j = 0; j < 6; j++) {
            int offset = j * 16;
            offset += i * 128;
            if (entityData[offset + 1] != 11) {
                continue;
            }
            retractingSpikeData[2 * numRetractableSpikes + 0] = (int)(&entityData[i * 128] + j * 16);
            retractingSpikeData[2 * numRetractableSpikes + 1] = (int)(&entityData[i * 128] + 125);
            numRetractableSpikes++;
            variant = entityData[offset + IE_VARIANT];
            entityData[offset + IE_VARIANT] = 0;
            if (variant * 29 < 52) {
                entityData[offset + IE_STATE] = 2;
                entityData[offset + IE_UNK8] = 0;
                entityData[offset + IE_UNK16] = 0;
                entityData[offset + IE_COUNTER] = 52 - 29 * variant;
            } else if (variant * 29 < 80) {
                entityData[offset + IE_STATE] = 1;
                entityData[offset + IE_UNK8] = 31;
                entityData[offset + IE_UNK16] = 3;
                entityData[offset + IE_COUNTER] = 80 - variant * 29;
            } else if (variant * 29 < 116) {
                entityData[offset + IE_STATE] = 1;
                entityData[offset + IE_UNK8] = 31;
                entityData[offset + IE_UNK16] = 4;
                entityData[offset + IE_COUNTER] = 116 - variant * 29;
            }
        }
    }
}
