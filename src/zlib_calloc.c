#include "common.h"

#include <libetc.h>

extern int whichDrawDispEnv;
extern char kulaZAllocatorHeap[];

void z_error(const char* msg) {
  VSyncCallback(NULL);
  SetupDisplay(1,128,0,0,0,0);
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
    __asm__(
        "\tlui        $a0,0xffe0\n"
        "\tori        $a0,$a0,0xe000\n"
        "\tlui        $v1,0x1f\n"
        "\tlw         $v0,kulaZAllocatorPointer\n"
        "\tori        $v1,$v1,0x2000\n"
        "\tsw         $v1,kulaZAllocatorPointer\n"
        "\taddu       $v0,$v0,$a0\n"
        "\tsw         $v0,kulaZAllocatorHeapUsage\n"
    );
}

void* zcallocUnused(unsigned num, int size) {
  char* ret = kulaZAllocatorPointer;
  char* next = kulaZAllocatorPointer + num * size;
  int i;

  kulaZAllocatorPointer = next;
  if ((unsigned)next > 0x1fe000) {
    VSyncCallback(NULL);
    SetupDisplay(1,128,0,0,0,0);
    FntFlush(-1);
    DrawSync(0);
    whichDrawDispEnv = 0;
    PutDrawAndDispEnvs();
    FntPrint("Alloc error:\n\n");
    FntPrint("error in alloc, SIZE_ALLOC too small!\n");
    FntFlush(-1);
    whichDrawDispEnv = 1;
    PutDrawAndDispEnvs();
    while(1)
        ;
  }

  for(i = 0; i < (int)num * size; i++)
      ret[i] = 0;

  return ret;
}

void* zcalloc(void* opaque, unsigned num, int size) {
  char* ret = kulaZAllocatorPointer;
  char* next = kulaZAllocatorPointer + num * size;
  int i;

  kulaZAllocatorPointer = next;
  if ((unsigned)next > 0x1fe000) {
    VSyncCallback(NULL);
    SetupDisplay(1,128,0,0,0,0);
    FntFlush(-1);
    DrawSync(0);
    whichDrawDispEnv = 0;
    PutDrawAndDispEnvs();
    FntPrint("Alloc error:\n\n");
    FntPrint("error in alloc, SIZE_ALLOC too small!\n");
    FntFlush(-1);
    whichDrawDispEnv = 1;
    PutDrawAndDispEnvs();
    while(1)
        ;
  }

  for(i = 0; i < (int)num * size; i++)
      ret[i] = 0;

  return ret;
}

void zcfree(void* ptr) {
}
