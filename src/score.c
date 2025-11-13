#include "../include/score.h"

static int gScore = 0;

void Score_Init(void)
{
    gScore = 0;
}

void Score_Reset(void)
{
    gScore = 0;
}

void Score_Add(int points)
{
    if (points > 0) {
        gScore += points;
        if (gScore < 0) gScore = 0; /* safety */
    }
}

int Score_Get(void)
{
    return gScore;
}
