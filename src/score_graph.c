#include "common.h"

typedef struct ScoreGraph {
    DR_TPAGE tpages[2];          // 16 bytes  (offset 0)
    u_long lines[2][3300];       // 26400 bytes (offset 16)
    POLY_G4 polys[2][300];       // 21600 bytes (offset 26416)
    POLY_G4 unusedPolys[2][301]; // 21672 bytes (offset 48016)
    DR_TPAGE tpages2[2];         // 16 bytes (offset 69688)
    POLY_G4 polys3[2][3];        // 216 bytes (offset 69704)
} ScoreGraph;

extern void TSpritePrim(TSprite* ts, int dfe, int dtd, int tpage);

extern PrimList primLists[2];
extern TSprite scoreGraphSprites[2][2];
extern int whichDrawDispEnv;
extern Texture textures[150];

int doDrawGraph;
int drawScoreGraphHeight;
int* drawScoreGraphHeightsBuf;
int* drawScoreGraphLevelScores;
ScoreGraph* drawScoreGraphPrims;
int* drawScoreGraphUnusedScorePtr;
int drawScoreGraphWidth;
int drawScoreGraphY;
uint firstGuiTexture;

int InitScoreGraph(void* pBuf, int* levelScores, int* unusedScorePtr, int numLevels, int maxScore, int x, int y, int graphWidth, int graphHeight, int isHighscore) {
    int i;
    int j;
    int val;
    int isOddSection;
    int elemWidth;
    int* pDst;

    drawScoreGraphHeightsBuf = pBuf;
    drawScoreGraphY = y;
    drawScoreGraphHeight = graphHeight;
    drawScoreGraphPrims = (ScoreGraph*)((char*)pBuf + 3600);
    drawScoreGraphLevelScores = levelScores;
    drawScoreGraphUnusedScorePtr = unusedScorePtr;
    drawScoreGraphWidth = graphWidth;

    if (maxScore == -1) {
        maxScore = 0;
        for (i = 0; i < graphWidth; i++) {
            if (levelScores != (int*)-1 && levelScores[i] > maxScore) {
                maxScore = levelScores[i];
            }
            if (drawScoreGraphUnusedScorePtr != (int*)-1 && unusedScorePtr[i] > maxScore) {
                maxScore = unusedScorePtr[i];
            }
        }
    }

    if (maxScore < 1)
        maxScore = 1;

    for (j = 0; j < 2; j++) {
        SetDrawTPage(&drawScoreGraphPrims->tpages[j], 0, 0, GetTPage(0, 1, 0, 0));
        SetDrawTPage(&drawScoreGraphPrims->tpages2[j], 0, 0, GetTPage(0, 2, 0, 0));
    }

    elemWidth = graphWidth / (numLevels - 1);
    if (elemWidth < 1)
        elemWidth = 1;

    if (drawScoreGraphLevelScores != (int*)-1) {
        pDst = drawScoreGraphHeightsBuf;
        for (i = 0; i < graphWidth; i++) {
            int section = (i * 10) / graphWidth;
            isOddSection = section % 2;
            val = drawScoreGraphLevelScores[(i * (numLevels - 1)) / graphWidth] +
                ((drawScoreGraphLevelScores[(i * (numLevels - 1)) / graphWidth + 1] -
                  drawScoreGraphLevelScores[(i * (numLevels - 1)) / graphWidth]) *
                 (i % elemWidth)) /
                    elemWidth;
            if (val < 0)
                val = 0;

            val = (val * (graphHeight << 1)) / maxScore;
            if (val & 1)
                val += 2;

            val >>= 1;

            *pDst++ = (y + graphHeight) << 12;
            *pDst++ = ((-val) << 12) / 25;
            *pDst++ = (y + graphHeight - val) << 12;

            for (j = 0; j < 2; j++) {
                drawScoreGraphPrims->lines[j][i * 11 + 0] = 0x0a000000;
                drawScoreGraphPrims->lines[j][i * 11 + 2] = x + i;
                drawScoreGraphPrims->lines[j][i * 11 + 4] = x + i + 8;
                drawScoreGraphPrims->lines[j][i * 11 + 5] = !isOddSection ? 0x50802080 : 0x506c1b6c;
                drawScoreGraphPrims->lines[j][i * 11 + 6] = (y + graphHeight) << 16 | (x + i);
                drawScoreGraphPrims->lines[j][i * 11 + 7] = !isOddSection ? 0x00b02020 : 0x00951b1b;
                drawScoreGraphPrims->lines[j][i * 11 + 8] = x + i;
                drawScoreGraphPrims->lines[j][i * 11 + 10] = x + i;

                *(u_long*)&(drawScoreGraphPrims->polys[j] + i)->r2 =
                    !isOddSection ? 0x00802020 : 0x006c1b1b;
                *(u_long*)&(drawScoreGraphPrims->polys[j] + i)->r3 =
                    !isOddSection ? 0x00802020 : 0x006c1b1b;

                (drawScoreGraphPrims->polys[j] + i)->x0 = x + i;
                (drawScoreGraphPrims->polys[j] + i)->y0 = y + graphHeight + 1;
                (drawScoreGraphPrims->polys[j] + i)->x1 = x + i + 9;
                (drawScoreGraphPrims->polys[j] + i)->y1 = y + graphHeight - 7;
                (drawScoreGraphPrims->polys[j] + i)->x2 = x + i;
                (drawScoreGraphPrims->polys[j] + i)->y2 = 0;
                (drawScoreGraphPrims->polys[j] + i)->x3 = x + i + 9;
                (drawScoreGraphPrims->polys[j] + i)->y3 = 0;

                SetPolyG4(drawScoreGraphPrims->polys[j] + i);
                SetSemiTrans(drawScoreGraphPrims->polys[j] + i, 0);
            }
        }
    }

    if (drawScoreGraphUnusedScorePtr != (int*)-1) {
        for (i = 0; i < graphWidth; i++) {
            val = drawScoreGraphUnusedScorePtr[i];
            if (val < 0)
                val = 0;
            val = (val * (graphHeight << 1)) / maxScore;
            if (val & 1)
                val += 2;
            val >>= 1;
            for (j = 0; j < 2; j++) {
                drawScoreGraphPrims->unusedPolys[j][i].x0 = x + i * 2 - 8;
                drawScoreGraphPrims->unusedPolys[j][i].x1 = x + i * 2 - 6;
                drawScoreGraphPrims->unusedPolys[j][i].x2 = x + i * 2;
                drawScoreGraphPrims->unusedPolys[j][i].x3 = x + i * 2 + 2;

                drawScoreGraphPrims->unusedPolys[j][i].y1 = y + graphHeight - val + 8;
                drawScoreGraphPrims->unusedPolys[j][i].y3 = y + graphHeight - val;
                drawScoreGraphPrims->unusedPolys[j][i + 1].y0 = y + graphHeight - val + 8;
                drawScoreGraphPrims->unusedPolys[j][i + 1].y2 = y + graphHeight - val;

                if (drawScoreGraphPrims->unusedPolys[0][i].y0 >=
                    drawScoreGraphPrims->unusedPolys[0][i].y1 + 2) {
                    *(u_long*)&drawScoreGraphPrims->unusedPolys[j][i].r0 = 0x404040;
                    *(u_long*)&drawScoreGraphPrims->unusedPolys[j][i].r1 = 0x404040;
                    *(u_long*)&drawScoreGraphPrims->unusedPolys[j][i].r2 = 0;
                    *(u_long*)&drawScoreGraphPrims->unusedPolys[j][i].r3 = 0;
                } else {
                    *(u_long*)&drawScoreGraphPrims->unusedPolys[j][i].r0 = 0xa0a0a0;
                    *(u_long*)&drawScoreGraphPrims->unusedPolys[j][i].r1 = 0xa0a0a0;
                    *(u_long*)&drawScoreGraphPrims->unusedPolys[j][i].r2 = 0;
                    *(u_long*)&drawScoreGraphPrims->unusedPolys[j][i].r3 = 0;
                }

                SetPolyG4(&drawScoreGraphPrims->unusedPolys[j][i]);
                SetSemiTrans(&drawScoreGraphPrims->unusedPolys[j][i], 1);
            }
        }
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 2; j++) {
            switch (i) {
                case 0:
                    if (isHighscore == 0) {
                        (drawScoreGraphPrims->polys3[j] + i)->x0 = x;
                        (drawScoreGraphPrims->polys3[j] + i)->y0 = y + graphHeight + 1;
                        (drawScoreGraphPrims->polys3[j] + i)->x1 = x + graphWidth + 1;
                        (drawScoreGraphPrims->polys3[j] + i)->y1 = y + graphHeight + 1;
                        (drawScoreGraphPrims->polys3[j] + i)->x2 = x + 8;
                        (drawScoreGraphPrims->polys3[j] + i)->y2 = y + graphHeight - 8;
                        (drawScoreGraphPrims->polys3[j] + i)->x3 = x + graphWidth + 9;
                        (drawScoreGraphPrims->polys3[j] + i)->y3 = y + graphHeight - 8;
                    } else {
                        (drawScoreGraphPrims->polys3[j] + i)->x0 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->y0 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->x1 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->y1 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->x2 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->y2 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->x3 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->y3 = 0;
                    }
                    *(u_long*)&(drawScoreGraphPrims->polys3[j] + i)->r0 = 0;
                    *(u_long*)&(drawScoreGraphPrims->polys3[j] + i)->r1 = 0;
                    *(u_long*)&(drawScoreGraphPrims->polys3[j] + i)->r2 = 0;
                    *(u_long*)&(drawScoreGraphPrims->polys3[j] + i)->r3 = 0;
                    SetPolyG4(drawScoreGraphPrims->polys3[j] + i);
                    SetSemiTrans(drawScoreGraphPrims->polys3[j] + i, 0);
                    break;
                case 1:
                    if (isHighscore == 0) {
                        (drawScoreGraphPrims->polys3[j] + i)->x0 = x + 8;
                        (drawScoreGraphPrims->polys3[j] + i)->y0 = y + graphHeight - 7;
                        (drawScoreGraphPrims->polys3[j] + i)->x1 = x + graphWidth + 9;
                        (drawScoreGraphPrims->polys3[j] + i)->y1 = y + graphHeight - 7;
                        (drawScoreGraphPrims->polys3[j] + i)->x2 = x + 8;
                        (drawScoreGraphPrims->polys3[j] + i)->y2 = y - 8;
                        (drawScoreGraphPrims->polys3[j] + i)->x3 = x + graphWidth + 9;
                        (drawScoreGraphPrims->polys3[j] + i)->y3 = y - 8;
                    } else {
                        (drawScoreGraphPrims->polys3[j] + i)->x0 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->y0 = 256;
                        (drawScoreGraphPrims->polys3[j] + i)->x1 = 320;
                        (drawScoreGraphPrims->polys3[j] + i)->y1 = 256;
                        (drawScoreGraphPrims->polys3[j] + i)->x2 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->y2 = 128;
                        (drawScoreGraphPrims->polys3[j] + i)->x3 = 320;
                        (drawScoreGraphPrims->polys3[j] + i)->y3 = 128;
                    }
                    *(u_long*)&(drawScoreGraphPrims->polys3[j] + i)->r0 = 0x00d0d0d0;
                    *(u_long*)&(drawScoreGraphPrims->polys3[j] + i)->r1 = 0x00d0d0d0;
                    *(u_long*)&(drawScoreGraphPrims->polys3[j] + i)->r2 = 0;
                    *(u_long*)&(drawScoreGraphPrims->polys3[j] + i)->r3 = 0;
                    SetPolyG4(drawScoreGraphPrims->polys3[j] + i);
                    SetSemiTrans(drawScoreGraphPrims->polys3[j] + i, 1);
                    break;
                case 2:
                    if (isHighscore == 0) {
                        (drawScoreGraphPrims->polys3[j] + i)->x0 = x;
                        (drawScoreGraphPrims->polys3[j] + i)->y0 = y + graphHeight + 1;
                        (drawScoreGraphPrims->polys3[j] + i)->x1 = x + 9;
                        (drawScoreGraphPrims->polys3[j] + i)->y1 = y + graphHeight - 7;
                        (drawScoreGraphPrims->polys3[j] + i)->x2 = x;
                        (drawScoreGraphPrims->polys3[j] + i)->y2 = y;
                        (drawScoreGraphPrims->polys3[j] + i)->x3 = x + 9;
                        (drawScoreGraphPrims->polys3[j] + i)->y3 = y - 8;
                    } else {
                        (drawScoreGraphPrims->polys3[j] + i)->x0 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->y0 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->x1 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->y1 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->x2 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->y2 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->x3 = 0;
                        (drawScoreGraphPrims->polys3[j] + i)->y3 = 0;
                    }
                    *(u_long*)&(drawScoreGraphPrims->polys3[j] + i)->r0 = 0x00d0d0d0;
                    *(u_long*)&(drawScoreGraphPrims->polys3[j] + i)->r1 = 0x00d0d0d0;
                    *(u_long*)&(drawScoreGraphPrims->polys3[j] + i)->r2 = 0;
                    *(u_long*)&(drawScoreGraphPrims->polys3[j] + i)->r3 = 0;
                    SetPolyG4(drawScoreGraphPrims->polys3[j] + i);
                    SetSemiTrans(drawScoreGraphPrims->polys3[j] + i, 1);
                    break;
            }
        }
    }

    if (drawScoreGraphLevelScores != (int*)-1) {
        doDrawGraph = 1;
    } else {
        doDrawGraph = 0;
    }

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            TSpritePrim(&scoreGraphSprites[i][j], 0, 0,
                        textures[firstGuiTexture + 20 + j].tpage);

            if (isHighscore == 0) {
                if (j == 0) {
                    setRGB0(&scoreGraphSprites[i][0].sprt, 64, 64, 64);
                } else {
                    setRGB0(&scoreGraphSprites[i][j].sprt, 72, 72, 72);
                }
            } else {
                setRGB0(&scoreGraphSprites[i][j].sprt, 72, 72, 72);
            }

            if (j == 0) {
                setXY0(&scoreGraphSprites[i][0].sprt, x + 2, y + 1);
            } else {
                setXY0(&scoreGraphSprites[i][1].sprt, x + graphWidth - 50, y + graphHeight - 6);
            }

            SetSemiTrans(&scoreGraphSprites[i][j].sprt,
                         textures[firstGuiTexture + 20 + j].semitrans);
            SetShadeTex(&scoreGraphSprites[i][j].sprt, 0);

            scoreGraphSprites[i][j].sprt.clut = textures[firstGuiTexture + 20 + j].clut;
            scoreGraphSprites[i][j].sprt.w = textures[firstGuiTexture + 20 + j].w;
            scoreGraphSprites[i][j].sprt.h = textures[firstGuiTexture + 20 + j].h;
            setUV0(&scoreGraphSprites[i][j].sprt, textures[firstGuiTexture + 20 + j].u,
                   textures[firstGuiTexture + 20 + j].v);
        }
    }
    return 3600 + sizeof(ScoreGraph);
}

void DrawScoreGraph(void) {
    int i;
    int heightVal;
    int isVisible;
    int scale;
    int unk1;
    int unk2;
    int disableDraw;
    int y;
    int *hbuf;
    short *l0;
    if (doDrawGraph == 1) {
        disableDraw = 1;
        hbuf = drawScoreGraphHeightsBuf;
        for (i = 0; i < drawScoreGraphWidth; hbuf += 3, i++) {
            y = hbuf[0];
            y += hbuf[1];
            if (hbuf[2] > y) {
                y = hbuf[2];
                hbuf[0] = y;
                hbuf[1] = 0;
            } else {
                disableDraw = 0;
                hbuf[0] = y;
            }

            y /= 4096;

            l0 = (short *)&drawScoreGraphPrims->lines[0][i * 11];
            l0[5] = y - 1;
            l0[9] = y - 9;
            l0[17] = y;
            l0[21] = y;

            l0 = (short *)&drawScoreGraphPrims->lines[1][i * 11];
            l0[5] = y - 1;
            l0[9] = y - 9;
            l0[17] = y;
            l0[21] = y;

            drawScoreGraphPrims->polys[0][i].y2 = y - 1;
            drawScoreGraphPrims->polys[0][i].y3 = y - 9;
            drawScoreGraphPrims->polys[1][i].y2 = y - 1;
            drawScoreGraphPrims->polys[1][i].y3 = y - 9;

            if (i > 0) {
                drawScoreGraphPrims->polys[0][i - 1].y0 = y - 1;
                drawScoreGraphPrims->polys[0][i - 1].y1 = y - 9;
                drawScoreGraphPrims->polys[1][i - 1].y0 = y - 1;
                drawScoreGraphPrims->polys[1][i - 1].y1 = y - 9;
            }
        }

        if (disableDraw == 1) {
            doDrawGraph = 0;
        }
    }

    addPrim(&primLists[whichDrawDispEnv].gui1, &scoreGraphSprites[whichDrawDispEnv][0]);
    addPrim(&primLists[whichDrawDispEnv].gui1, &scoreGraphSprites[whichDrawDispEnv][1]);

    if (drawScoreGraphUnusedScorePtr != (int *)-1) {
        for (i = drawScoreGraphWidth - 1; i > 0; i--) {
            addPrim(&primLists[whichDrawDispEnv].gui1, &drawScoreGraphPrims->unusedPolys[whichDrawDispEnv][i]);
        }
        addPrim(&primLists[whichDrawDispEnv].gui1, &drawScoreGraphPrims->tpages[whichDrawDispEnv]);
    }

    if (drawScoreGraphLevelScores != (int *)-1) {
        for (i = drawScoreGraphWidth - 1; 0 <= i; i--) {
            heightVal = ((i * 10) / drawScoreGraphWidth) % 2;

            if (((short *)&drawScoreGraphPrims->lines[whichDrawDispEnv][i * 11])[21] < drawScoreGraphY + drawScoreGraphHeight) {
                addPrim(&primLists[whichDrawDispEnv].gui1, &drawScoreGraphPrims->lines[whichDrawDispEnv][i * 11]);
            }

            isVisible = 0;
            if (i == drawScoreGraphWidth - 1) {
                if (((short *)&drawScoreGraphPrims->lines[whichDrawDispEnv][i * 11])[21] < drawScoreGraphY + drawScoreGraphHeight) {
                    addPrim(&primLists[whichDrawDispEnv].gui1, &drawScoreGraphPrims->polys[whichDrawDispEnv][i]);
                    isVisible = 1;

                    drawScoreGraphPrims->lines[whichDrawDispEnv][i * 11 + 1] = !heightVal ? 0x50e03838 : 0x50be2f2f;
                    drawScoreGraphPrims->lines[whichDrawDispEnv][i * 11 + 3] = !heightVal ? 0x00801010 : 0x006c0d0d;
                    drawScoreGraphPrims->lines[whichDrawDispEnv][i * 11 + 9] = !heightVal ? 0x68e03838 : 0x68be2f2f;
                }
            } else {
                if (drawScoreGraphPrims->polys[whichDrawDispEnv][i].y2 < drawScoreGraphPrims->polys[whichDrawDispEnv][i + 1].y2) {
                    addPrim(&primLists[whichDrawDispEnv].gui1, &drawScoreGraphPrims->polys[whichDrawDispEnv][i]);
                    isVisible = 1;
                    drawScoreGraphPrims->lines[whichDrawDispEnv][i * 11 + 1] = !heightVal ? 0x50e03838 : 0x50be2f2f;
                    drawScoreGraphPrims->lines[whichDrawDispEnv][i * 11 + 3] = !heightVal ? 0x00801010 : 0x006c0d0d;
                    drawScoreGraphPrims->lines[whichDrawDispEnv][i * 11 + 9] = !heightVal ? 0x68e03838 : 0x68be2f2f;
                } else {
                    drawScoreGraphPrims->lines[whichDrawDispEnv][i * 11 + 1] = !heightVal ? 0x50f05050 : 0x50cc4444;
                    drawScoreGraphPrims->lines[whichDrawDispEnv][i * 11 + 3] = !heightVal ? 0x00a02020 : 0x00881b1b;
                    drawScoreGraphPrims->lines[whichDrawDispEnv][i * 11 + 9] = !heightVal ? 0x68f87070 : 0x68d25f5f;
                }
            }

            if (isVisible == 1) {
                scale = (drawScoreGraphPrims->polys[whichDrawDispEnv][i].y0 - drawScoreGraphPrims->polys[whichDrawDispEnv][i].y2) * 4096 / ((drawScoreGraphY + drawScoreGraphHeight) - drawScoreGraphPrims->polys[whichDrawDispEnv][i].y2);

                unk1 = (scale * 64) >> 12;
                unk2 = -(scale * 32) >> 12;
                if (heightVal == 1) {
                    unk1 = (unk1 * 85) / 100;
                    unk2 = (unk2 * 85) / 100;
                }
                unk1 += 32;
                unk2 += 128;
                *(u_long *)&drawScoreGraphPrims->polys[whichDrawDispEnv][i].r0 = (unk2 << 16) | (unk1 | 0x38002000);

                unk1 = (scale * 64) >> 12;
                unk2 = -(scale * 32) >> 12;
                if (heightVal == 1) {
                    unk1 = (unk1 * 85) / 100;
                    unk2 = (unk2 * 85) / 100;
                }
                unk1 += 32;
                unk2 += 128;
                *(u_long *)&drawScoreGraphPrims->polys[whichDrawDispEnv][i].r1 = (unk2 << 16) | (unk1 | 0x2000);
            }
        }
    }
    addPrim(&primLists[whichDrawDispEnv].gui1, &drawScoreGraphPrims->polys3[whichDrawDispEnv][0]);
    addPrim(&primLists[whichDrawDispEnv].gui1, &drawScoreGraphPrims->polys3[whichDrawDispEnv][1]);
    addPrim(&primLists[whichDrawDispEnv].gui1, &drawScoreGraphPrims->polys3[whichDrawDispEnv][2]);
    addPrim(&primLists[whichDrawDispEnv].gui1, &drawScoreGraphPrims->tpages2[whichDrawDispEnv]);
}
