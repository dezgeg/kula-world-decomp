#include "common.h"

void FUN_000298e0(AnimatedTextureChain *textureChain) {
    TextureChainEntry *end;
    TextureChainEntry *p;
    
    end = (TextureChainEntry *)textureChain->entries;
    for (p = (TextureChainEntry *)(textureChain + 1); p < end; p++) {
        p->quadPtr = (Quad **)*p->quadPtr;
    }
}
