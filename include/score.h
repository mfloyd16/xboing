#ifndef SCORE_H
#define SCORE_H

#include <stdint.h>

/* Simple score tracking module */
void Score_Init(void);
void Score_Reset(void);
void Score_Add(int points);
int  Score_Get(void);

#endif /* SCORE_H */
