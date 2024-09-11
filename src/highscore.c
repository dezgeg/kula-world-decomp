#include "common.h"

#include <string.h>

extern uint Rand(int param_1);

Highscore highscores[6];

extern int highscoreLevelScores[6][150];

void InitHighscores(void) {
    extern char S_LEIF[];
    extern char S_SAVE_1[];
    extern char S_SAVE_2[];
    extern char S_SAVE_3[];
    extern char S_SAVE_4[];
    extern char S_PLAYER[];
    int levelScore;
    int i;
    int j;

    highscores[0].score = 50000;
    highscores[0].fruits = 25;
    highscores[0].time = 2110;
    highscores[0].level = 50;
    levelScore = 0;
    for (j = 0; j < 150; j++) {
        levelScore += Rand(1000) - (j * 500) / 30;
        if (levelScore > 50000) {
            levelScore = 50000;
        }
        if (levelScore < 0) {
            break;
        }
        highscoreLevelScores[0][j] = levelScore;
    }

    for (i = 1; i < 6; i++) {
        highscores[i].score = 0;
        highscores[i].fruits = 0;
        highscores[i].time = 0;
        highscores[i].level = 0;
        for (j = 0; j < 150; j++) {
            highscoreLevelScores[i][j] = 0;
        }
    }

    sprintf(highscores[0].name, S_LEIF);
    sprintf(highscores[1].name, S_SAVE_1);
    sprintf(highscores[2].name, S_SAVE_2);
    sprintf(highscores[3].name, S_SAVE_3);
    sprintf(highscores[4].name, S_SAVE_4);
    sprintf(highscores[5].name, S_PLAYER);
}

INCLUDE_ASM("asm/nonmatchings/highscore", HighScoreUi);
