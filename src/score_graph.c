#include "common.h"

extern void TSpritePrim(TSprite* ts, int dfe, int dtd, int tpage);

extern int whichDrawDispEnv;
extern PrimList primLists[2];
extern Texture textures[150];
extern TSprite scoreGraphSprites[2][2];

static int doDrawGraph;
static int drawScoreGraphHeight;
static int* drawScoreGraphHeightsBuf;
static int* drawScoreGraphLevelScores;
static int* drawScoreGraphUnusedScorePtr;
static int drawScoreGraphWidth;
static int drawScoreGraphY;
static void* drawScoreGraphPrims;
uint firstGuiTexture;

INCLUDE_ASM("asm/nonmatchings/score_graph", InitScoreGraph);

INCLUDE_ASM("asm/nonmatchings/score_graph", DrawScoreGraph);
