#ifndef XBOING_BLOCK_H
#define XBOING_BLOCK_H

#include <raylib.h>
#include <stdbool.h>
#include "../include/level.h"

/* Block types matching the level file character codes */
typedef enum BlockType {
    BLOCK_NONE = 0,       /* ' ' - Empty cell (no block) */
    BLOCK_RED,            /* 'r' - Standard red block */
    BLOCK_GREEN,          /* 'g' - Standard green block */
    BLOCK_BLUE,           /* 'b' - Standard blue block */
    BLOCK_TAN,            /* 't' - Standard tan block */
    BLOCK_PURPLE,         /* 'p' - Standard purple block */
    BLOCK_YELLOW,         /* 'y' - Standard yellow block */
    BLOCK_BLACK,          /* 'w' - Black indestructible wall */
    BLOCK_COUNTER_0,      /* '0' - Hit 0 times (instant destroy) */
    BLOCK_COUNTER_1,      /* '1' - Hit 1 time to destroy */
    BLOCK_COUNTER_2,      /* '2' - Hit 2 times to destroy */
    BLOCK_COUNTER_3,      /* '3' - Hit 3 times to destroy */
    BLOCK_COUNTER_4,      /* '4' - Hit 4 times to destroy */
    BLOCK_COUNTER_5,      /* '5' - Hit 5 times to destroy */
    BLOCK_HYPERSPACE,     /* 'H' - Hyperspace powerup (teleport) */
    BLOCK_SPEED,          /* 'B' - Bullet powerup (fast ball) */
    BLOCK_MAXAMMO,        /* 'c' - Lots of ammo powerup */
    BLOCK_ROAMER,         /* '+' - Moving enemy block */
    BLOCK_BOMB,           /* 'X' - Bomb (explodes nearby blocks) */
    BLOCK_DEATH,          /* 'D' - Death block (kills ball) */
    BLOCK_EXTRABALL,      /* 'L' - Extra ball powerup */
    BLOCK_MACHINEGUN,     /* 'M' - Machine gun powerup */
    BLOCK_WALLOFF,        /* 'W' - Wall off powerup (removes walls) */
    BLOCK_RANDOM,         /* '?' - Random powerup block */
    BLOCK_DROP,           /* 'd' - Drop block (falls when hit) */
    BLOCK_TIMER,          /* 'T' - Timer powerup (extra time) */
    BLOCK_MULTIBALL,      /* 'm' - Multiball powerup */
    BLOCK_STICKY,         /* 's' - Sticky paddle powerup */
    BLOCK_REVERSE,        /* 'R' - Reverse controls powerup */
    BLOCK_PAD_SHRINK,     /* '<' - Shrink paddle powerup */
    BLOCK_PAD_EXPAND      /* '>' - Expand paddle powerup */
} BlockType;

/* Individual block instance */
typedef struct Block {
    BlockType type;
    int row;
    int col;
    bool active;
    int offsetX;  /* X offset to center texture in cell */
    int offsetY;  /* Y offset to center texture in cell */
} Block;

/* Block grid manager */
typedef struct BlockGrid {
    Block blocks[MAX_LEVEL_HEIGHT * MAX_LEVEL_WIDTH];
    int count;
    int cellWidth;   /* Grid cell width */
    int cellHeight;  /* Grid cell height */
} BlockGrid;

/* Load blocks from a Level into a BlockGrid, positioned within bounds */
void BlockGrid_Load(BlockGrid* grid, const Level* level, Rectangle bounds);

/* Draw all active blocks in the grid */
void BlockGrid_Draw(const BlockGrid* grid, Rectangle bounds);

/* Free block grid resources */
void BlockGrid_Free(BlockGrid* grid);

/* Helper: map level character to block type */
BlockType Block_CharToType(char c);

/* Helper: get texture for a block type */
Texture2D Block_GetTexture(BlockType type);

#endif /* XBOING_BLOCK_H */
