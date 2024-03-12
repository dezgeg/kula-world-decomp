#include "common.h"

#include <libetc.h>

extern char S_Alloc_error[];
extern int whichDrawDispEnv;

void z_error(const char* msg) {
  __asm__(".extern whichDrawDispEnv,999"); // hack
  VSyncCallback(NULL);
  SetupDisplay(1,128,0,0,0,0);
  FntFlush(-1);
  DrawSync(0);
  whichDrawDispEnv = 0;
  PutDrawAndDispEnvs();
  FntPrint(S_Alloc_error);
  FntPrint(msg);
  FntFlush(-1);
  whichDrawDispEnv = 1;
  PutDrawAndDispEnvs();
  while (1)
      ;
}

extern void* kulaZAllocatorPointer;
extern long kulaZAllocatorHeapUsage;
extern char kulaZAllocatorHeap[];

void zcallocInit(void) {
    __asm__(
        "\t.extern kulaZAllocatorHeapUsage, 4\n"
        "\t.extern kulaZAllocatorPointer, 4\n"
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
