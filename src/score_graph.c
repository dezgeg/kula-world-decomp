#include "common.h"

extern void TSpritePrim(TSprite* ts, int dfe, int dtd, int tpage);

extern int whichDrawDispEnv;
extern PrimList primLists[2];
extern Texture textures[150];
extern TSprite scoreGraphSprites[2][2];

int doDrawGraph;
int drawScoreGraphHeight;
int* drawScoreGraphHeightsBuf;
int* drawScoreGraphLevelScores;
int* drawScoreGraphUnusedScorePtr;
int drawScoreGraphWidth;
int drawScoreGraphY;
void* drawScoreGraphPrims;
uint firstGuiTexture;

INCLUDE_ASM("asm/nonmatchings/score_graph", InitScoreGraph);

INCLUDE_ASM("asm/nonmatchings/score_graph", DrawScoreGraph);
