#include "../include/level.h"
#include <stdio.h>
#include <string.h>

/* Optional active-level pointer for UI usage */
static const Level* gActiveLevel = NULL;
static int gActiveLevelNumber = 0;

bool Level_Load(const char* filepath, Level* level)
{
    if (!filepath || !level) {
        return false;
    }

    FILE* file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "Failed to open level file: %s\n", filepath);
        return false;
    }

    /* Clear the level structure */
    memset(level, 0, sizeof(Level));

    /* Read title (first line) */
    if (!fgets(level->title, MAX_LEVEL_TITLE, file)) {
        fprintf(stderr, "Failed to read level title from: %s\n", filepath);
        fclose(file);
        return false;
    }
    /* Remove trailing newline */
    size_t len = strlen(level->title);
    if (len > 0 && level->title[len - 1] == '\n') {
        level->title[len - 1] = '\0';
    }

    /* Read time limit (second line) */
    char timeBuffer[32];
    if (!fgets(timeBuffer, sizeof(timeBuffer), file)) {
        fprintf(stderr, "Failed to read time limit from: %s\n", filepath);
        fclose(file);
        return false;
    }
    if (sscanf(timeBuffer, "%d", &level->timeLimit) != 1) {
        fprintf(stderr, "Invalid time format in: %s\n", filepath);
        fclose(file);
        return false;
    }

    /* Read block grid (remaining lines, max MAX_LEVEL_HEIGHT rows) */
    level->rows = 0;
    while (level->rows < MAX_LEVEL_HEIGHT && 
           fgets(level->grid[level->rows], MAX_LEVEL_WIDTH + 2, file)) {
        /* Remove trailing newline */
        len = strlen(level->grid[level->rows]);
        if (len > 0 && level->grid[level->rows][len - 1] == '\n') {
            level->grid[level->rows][len - 1] = '\0';
        }
        
        /* Pad with dots if line is shorter than MAX_LEVEL_WIDTH */
        len = strlen(level->grid[level->rows]);
        while (len < MAX_LEVEL_WIDTH) {
            level->grid[level->rows][len] = '.';
            len++;
        }
        level->grid[level->rows][MAX_LEVEL_WIDTH] = '\0';
        
        level->rows++;
    }

    fclose(file);
    return true;
}

void Level_Free(Level* level)
{
    /* Currently nothing to free, but keep for future extensions */
    (void)level;
}

const char* Level_GetTitle(const Level* level)
{
    return level ? level->title : "";
}

int Level_GetTime(const Level* level)
{
    return level ? level->timeLimit : 0;
}

int Level_GetRows(const Level* level)
{
    return level ? level->rows : 0;
}

int Level_GetCols(const Level* level)
{
    return level ? MAX_LEVEL_WIDTH : 0;
}

char Level_GetBlockAt(const Level* level, int row, int col)
{
    if (!level || row < 0 || row >= level->rows || 
        col < 0 || col >= MAX_LEVEL_WIDTH) {
        return '.';  /* Empty */
    }
    return level->grid[row][col];
}

void Level_Print(const Level* level)
{
    if (!level) {
        printf("Level: NULL\n");
        return;
    }

    printf("=== LEVEL INFO ===\n");
    printf("Title: %s\n", level->title);
    printf("Time: %d seconds\n", level->timeLimit);
    printf("Rows: %d\n", level->rows);
    printf("Grid:\n");
    for (int i = 0; i < level->rows; i++) {
        printf("  [%s]\n", level->grid[i]);
    }
    printf("==================\n");
}

void Level_SetActive(const Level* level)
{
    gActiveLevel = level;
}

const char* Level_GetActiveTitle(void)
{
    if (gActiveLevel && gActiveLevel->title[0] != '\0') return gActiveLevel->title;
    return "";
}

void Level_SetActiveNumber(int number)
{
    if (number < 0) number = 0;
    gActiveLevelNumber = number;
}

int Level_GetActiveNumber(void)
{
    return gActiveLevelNumber;
}
