#include "common.h"

extern int highscoreLevelScores[6][150];
extern Highscore highscores[6];
extern MemcardData memCardData;

void LoadHighscoresFromMemcardData(void) {
    int i;
    int j;

    if (highscores[0].score < memCardData.highscore0.score) {
        highscores[0].score = memCardData.highscore0.score;
        highscores[0].time = memCardData.highscore0.time;
        highscores[0].fruits = memCardData.highscore0.fruits;
        highscores[0].level = memCardData.highscore0.level;
        for (i = 0; i < 8; i++) {
            highscores[0].name[i] = memCardData.highscore0.name[i];
        }

        for (j = 0; j < 150; j++) {
            highscoreLevelScores[0][j] = memCardData.highscore0LevelScores[j];
        }
    }

    if (highscores[0].score > memCardData.highscore0.score) {
        memCardData.highscore0.score = highscores[0].score;
        memCardData.highscore0.time = highscores[0].time;
        memCardData.highscore0.fruits = highscores[0].fruits;
        memCardData.highscore0.level = highscores[0].level;

        for (i = 0; i < 8; i++) {
            memCardData.highscore0.name[i] = highscores[0].name[i];
        }

        for (j = 0; j < 150; j++) {
            memCardData.highscore0LevelScores[j] = highscoreLevelScores[0][j];
        }
    }

    for (i = 0; i < 4; i++) {
        if (memCardData.saveslots[i].valid == 1) {
            highscores[i + 1].score = memCardData.saveslots[i].score;
            highscores[i + 1].time = memCardData.saveslots[i].playtime;
            highscores[i + 1].fruits = memCardData.saveslots[i].fruits;

            if (memCardData.saveslots[i].isFinal == 0) {
                highscores[i + 1].level =
                    memCardData.saveslots[i].curWorld * 15 + memCardData.saveslots[i].curLevel + 1;
                for (j = 0; j < 150; j++) {
                    highscoreLevelScores[i + 1][j] = memCardData.saveslots[i].levelScores[j];
                }
            } else {
                highscores[i + 1].level =
                    memCardData.saveslots[i].curWorld * 2 + memCardData.saveslots[i].curLevel + 151;
                for (j = 0; j < 150; j++) {
                    highscoreLevelScores[i + 1][j] = 0;
                }
            }
        } else {
            highscores[i + 1].score = 0;
            highscores[i + 1].time = 0;
            highscores[i + 1].fruits = 0;
            highscores[i + 1].level = 0;
            for (j = 0; j < 150; j++) {
                highscoreLevelScores[i + 1][j] = 0;
            }
        }
    }
}
