#ifndef LEVEL_H
#define LEVEL_H

#include <stdbool.h>

/* Maximum dimensions for level grid */
#define MAX_LEVEL_WIDTH 9
#define MAX_LEVEL_HEIGHT 15
#define MAX_LEVEL_TITLE 128

/* Level data structure */
typedef struct {
    char title[MAX_LEVEL_TITLE];
    int timeLimit;  /* Time limit in seconds */
    char grid[MAX_LEVEL_HEIGHT][MAX_LEVEL_WIDTH + 1];  /* +1 for null terminator */
    int rows;  /* Actual number of rows loaded */
} Level;

/* Load a level from file - returns true on success */
bool Level_Load(const char* filepath, Level* level);

/* Free any resources associated with the level (currently none, but future-proof) */
void Level_Free(Level* level);

/* Getters for level data */
const char* Level_GetTitle(const Level* level);
int Level_GetTime(const Level* level);
int Level_GetRows(const Level* level);
int Level_GetCols(const Level* level);
char Level_GetBlockAt(const Level* level, int row, int col);

/* Debug: print level info to console */
void Level_Print(const Level* level);

/* Active level helpers (optional): set the current level to expose its title
 * to UI layers (e.g., gameplay HUD). If no active level is set, the getter
 * returns an empty string. */
void Level_SetActive(const Level* level);
const char* Level_GetActiveTitle(void);

#endif /* LEVEL_H */
