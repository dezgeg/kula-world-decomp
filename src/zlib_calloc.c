#include "common.h"

#include <libetc.h>

extern int whichDrawDispEnv;
extern char kulaZAllocatorHeap[];

void z_error(const char* msg) {
    VSyncCallback(NULL);
    SetupDisplay(1, 128, 0, 0, 0, 0);
    FntFlush(-1);
    DrawSync(0);
    whichDrawDispEnv = 0;
    PutDrawAndDispEnvs();
    FntPrint("Alloc error:\n\n");
    FntPrint(msg);
    FntFlush(-1);
    whichDrawDispEnv = 1;
    PutDrawAndDispEnvs();
    while (1)
        ;
}

void* kulaZAllocatorPointer;
long kulaZAllocatorHeapUsage = 0;

void zcallocInit(void) {
    char* old = kulaZAllocatorPointer;
    kulaZAllocatorHeapUsage = (unsigned)old - 0x1F2000;
    kulaZAllocatorPointer = 0x1F2000;
}

void* zcallocUnused(unsigned num, int size) {
    char* ret = kulaZAllocatorPointer;
    char* next = kulaZAllocatorPointer + num * size;
    int i;

    kulaZAllocatorPointer = next;
    if ((unsigned)next > 0x1fe000) {
        VSyncCallback(NULL);
        SetupDisplay(1, 128, 0, 0, 0, 0);
        FntFlush(-1);
        DrawSync(0);
        whichDrawDispEnv = 0;
        PutDrawAndDispEnvs();
        FntPrint("Alloc error:\n\n");
        FntPrint("error in alloc, SIZE_ALLOC too small!\n");
        FntFlush(-1);
        whichDrawDispEnv = 1;
        PutDrawAndDispEnvs();
        while (1)
            ;
    }

    for (i = 0; i < (int)num * size; i++) ret[i] = 0;

    return ret;
}

void* zcalloc(void* opaque, unsigned num, int size) {
    char* ret = kulaZAllocatorPointer;
    char* next = kulaZAllocatorPointer + num * size;
    int i;

    kulaZAllocatorPointer = next;
    if ((unsigned)next > 0x1fe000) {
        VSyncCallback(NULL);
        SetupDisplay(1, 128, 0, 0, 0, 0);
        FntFlush(-1);
        DrawSync(0);
        whichDrawDispEnv = 0;
        PutDrawAndDispEnvs();
        FntPrint("Alloc error:\n\n");
        FntPrint("error in alloc, SIZE_ALLOC too small!\n");
        FntFlush(-1);
        whichDrawDispEnv = 1;
        PutDrawAndDispEnvs();
        while (1)
            ;
    }

    for (i = 0; i < (int)num * size; i++) ret[i] = 0;

    return ret;
}

void zcfree(void* ptr) {}
