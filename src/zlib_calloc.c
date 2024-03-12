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
