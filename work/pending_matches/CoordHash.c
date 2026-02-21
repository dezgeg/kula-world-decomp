#include "common.h"

// non-gprel-used variables (extern)


int CoordHash(int x,int y,int z,int dir,int div,int mod) {
    switch(dir) {
    case 0:
        z = z + -0x100;
        break;
    case 1:
        x = x + 0x100;
        break;
    case 2:
        y = y + 0x100;
        break;
    case 3:
        y = y + -0x100;
        break;
    case 4:
        x = x + -0x100;
        break;
    case 5:
        z = z + 0x100;
    }
    return ((x + y + z) / (div * 3)) % mod;
}


