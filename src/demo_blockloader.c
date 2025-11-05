/**
 * @file demo_blockloader.c
 * @brief Block system implementation for XBoing
 * 
 * Manages level loading, block textures, rendering, and collision.
 */

#include "demo_blockloader.h"
#include "core/constants.h"
#include "core/types.h"
#include "demo_gamemodes.h"
#include "demo_ball.h"
#include "paddle.h"
#include "audio.h"
#include <stdio.h>
#include <raylib.h>

// =============================================================================
// Global State
// =============================================================================
static PlayArea g_playArea = {0};
static Block g_blocks[BLOCK_ROWS_MAX][BLOCK_COLS_MAX];
static int g_blocksRemaining = 0;
static char g_levelName[256];
static int g_timeBonus = 0;

// =============================================================================
// Block Textures
// =============================================================================
static Texture2D g_texHyperspace;
static Texture2D g_texBullet;
static Texture2D g_texMaxAmmo;
static Texture2D g_texRed;
static Texture2D g_texGreen;
static Texture2D g_texBlue;
static Texture2D g_texTan;
static Texture2D g_texPurple;
static Texture2D g_texYellow;
static Texture2D g_texBlack;
static Texture2D g_texRoamer;
static Texture2D g_texBomb;
static Texture2D g_texDeath;
static Texture2D g_texExtraBall;
static Texture2D g_texMGun;
static Texture2D g_texWallOff;
static Texture2D g_texRandom;
static Texture2D g_texDrop;
static Texture2D g_texTimer;
static Texture2D g_texMultiBall;
static Texture2D g_texSticky;
static Texture2D g_texReverse;
static Texture2D g_texPadShrink;
static Texture2D g_texPadExpand;
static Texture2D g_texCounter[6];

// =============================================================================
// Forward Declarations
// =============================================================================
static void AddBlock(int row, int col, char type);
static void DeactivateBlock(int row, int col);
static bool IsBlockTypeIndestructible(char type);
static Vector2 GetPlayCorner(Corner corner);

// =============================================================================
// Public Functions - Initialization
// =============================================================================

void Blocks_InitializePlayArea(void) {
    g_playArea.playWidth = SCREEN_WIDTH - (PLAY_X_PADDING * 2);
    g_playArea.playHeight = SCREEN_HEIGHT - (PLAY_Y_PADDING * 2);
    g_playArea.colWidth = g_playArea.playWidth / BLOCK_COLS_MAX;
    g_playArea.rowHeight = g_playArea.playHeight / (BLOCK_ROWS_MAX + PADDLE_ROWS);
    
    fprintf(stdout, "[Blocks] Play area initialized: %dx%d\n", 
            g_playArea.playWidth, g_playArea.playHeight);
}




// =============================================================================
// Public Functions - Level Loading
// =============================================================================

bool Blocks_LoadLevel(const char *filename) {
    g_blocksRemaining = 0;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "[Blocks] Failed to open level file: %s\n", filename);
        return false;
    }

    // Read level metadata
    fgets(g_levelName, sizeof(g_levelName), fp);
    fscanf(fp, "%d", &g_timeBonus);
    getc(fp); // Consume newline

    // Read block data
    int row = 0;
    int col = 0;
    int ch;

    while ((ch = getc(fp)) != EOF) {
        if (ch != '\n') {
            AddBlock(row, col, (char)ch);
            col++;
            if (col >= BLOCK_COLS_MAX) {
                col = 0;
                row++;
            }
        }
    }

    fclose(fp);
    
    fprintf(stdout, "[Blocks] Loaded level '%s' with %d blocks\n", 
            g_levelName, g_blocksRemaining);
    
    return true;
}


// =============================================================================
// Public Functions - Rendering
// =============================================================================

void Blocks_Draw(void) {
    for (int row = 0; row < BLOCK_ROWS_MAX; row++) {
        for (int col = 0; col < BLOCK_COLS_MAX; col++) {
            if (g_blocks[row][col].active) {
                DrawTexture(g_blocks[row][col].texture,
                           (int)g_blocks[row][col].position.x,
                           (int)g_blocks[row][col].position.y,
                           WHITE);
            }
        }
    }
}

void Blocks_DrawBorder(void) {
    Vector2 upperLeft = GetPlayCorner(CORNER_UPPER_LEFT);
    Vector2 lowerRight = GetPlayCorner(CORNER_LOWER_RIGHT);
    DrawRectangleLinesEx((Rectangle){upperLeft.x, upperLeft.y, 
                                      lowerRight.x, lowerRight.y},
                         PLAY_BORDER_WIDTH, RED);
}

void Blocks_DrawWalls(void) {
    DrawRectangleRec(Blocks_GetWall(WALL_LEFT), GRAY);
    DrawRectangleRec(Blocks_GetWall(WALL_RIGHT), GRAY);
    DrawRectangleRec(Blocks_GetWall(WALL_TOP), GRAY);
    DrawRectangleRec(Blocks_GetWall(WALL_BOTTOM), GRAY);
}


// =============================================================================
// Private Helper Functions
// =============================================================================

static void AddBlock(int row, int col, char type) {
    Block *block = &g_blocks[row][col];
    
    // Set default offsets
    block->blockOffsetX = (g_playArea.colWidth - BLOCK_WIDTH) / 2;
    block->blockOffsetY = (g_playArea.rowHeight - BLOCK_HEIGHT) / 2;
    block->type = type;
    
    // Assign texture and custom offsets based on block type
    switch (type) {
        case 'H': block->texture = g_texHyperspace; block->blockOffsetX = (g_playArea.colWidth - 31) / 2; block->blockOffsetY = (g_playArea.rowHeight - 31) / 2; break;
        case 'B': block->texture = g_texBullet; break;
        case 'c': block->texture = g_texMaxAmmo; break;
        case 'r': block->texture = g_texRed; break;
        case 'g': block->texture = g_texGreen; break;
        case 'b': block->texture = g_texBlue; break;
        case 't': block->texture = g_texTan; break;
        case 'p': block->texture = g_texPurple; break;
        case 'y': block->texture = g_texYellow; break;
        case 'w': block->texture = g_texBlack; block->blockOffsetX = (g_playArea.colWidth - 50) / 2; block->blockOffsetY = (g_playArea.rowHeight - 30) / 2; break;
        case '0': block->texture = g_texCounter[0]; break;
        case '1': block->texture = g_texCounter[1]; break;
        case '2': block->texture = g_texCounter[2]; break;
        case '3': block->texture = g_texCounter[3]; break;
        case '4': block->texture = g_texCounter[4]; break;
        case '5': block->texture = g_texCounter[5]; break;
        case '+': block->texture = g_texRoamer; block->blockOffsetX = (g_playArea.colWidth - 25) / 2; block->blockOffsetY = (g_playArea.rowHeight - 27) / 2; break;
        case 'X': block->texture = g_texBomb; block->blockOffsetX = (g_playArea.colWidth - 30) / 2; block->blockOffsetY = (g_playArea.rowHeight - 30) / 2; break;
        case 'D': block->texture = g_texDeath; block->blockOffsetX = (g_playArea.colWidth - 30) / 2; block->blockOffsetY = (g_playArea.rowHeight - 30) / 2; break;
        case 'L': block->texture = g_texExtraBall; block->blockOffsetX = (g_playArea.colWidth - 30) / 2; block->blockOffsetY = (g_playArea.rowHeight - 19) / 2; break;
        case 'M': block->texture = g_texMGun; block->blockOffsetX = (g_playArea.colWidth - 35) / 2; block->blockOffsetY = (g_playArea.rowHeight - 15) / 2; break;
        case 'W': block->texture = g_texWallOff; block->blockOffsetX = (g_playArea.colWidth - 27) / 2; block->blockOffsetY = (g_playArea.rowHeight - 23) / 2; break;
        case '?': block->texture = g_texRandom; break;
        case 'd': block->texture = g_texDrop; break;
        case 'T': block->texture = g_texTimer; block->blockOffsetX = (g_playArea.colWidth - 21) / 2; block->blockOffsetY = (g_playArea.rowHeight - 21) / 2; break;
        case 'm': block->texture = g_texMultiBall; break;
        case 's': block->texture = g_texSticky; block->blockOffsetX = (g_playArea.colWidth - 32) / 2; block->blockOffsetY = (g_playArea.rowHeight - 27) / 2; break;
        case 'R': block->texture = g_texReverse; block->blockOffsetX = (g_playArea.colWidth - 33) / 2; block->blockOffsetY = (g_playArea.rowHeight - 16) / 2; break;
        case '<': block->texture = g_texPadShrink; block->blockOffsetX = (g_playArea.colWidth - 40) / 2; block->blockOffsetY = (g_playArea.rowHeight - 15) / 2; break;
        case '>': block->texture = g_texPadExpand; block->blockOffsetX = (g_playArea.colWidth - 40) / 2; block->blockOffsetY = (g_playArea.rowHeight - 15) / 2; break;
        default: block->blockOffsetX = -1; break;
    }
    
    // Calculate final position
    block->position = (Vector2){
        (col * g_playArea.colWidth) + block->blockOffsetX + PLAY_X_OFFSET,
        (row * g_playArea.rowHeight) + block->blockOffsetY + PLAY_Y_OFFSET
    };
    
    // Set active state and count destructible blocks
    if (block->blockOffsetX != -1) {
        block->active = true;
        if (type != 'w') {  // Wall blocks don't count
            g_blocksRemaining++;
        }
    } else {
        block->active = false;
    }
}


bool Blocks_LoadTextures(void) {
    bool success = true;
    
    #define LOAD_TEXTURE(var, file) \
        var = LoadTexture(PATH_BLOCK_TEXTURES file); \
        if (var.id == 0) { \
            fprintf(stderr, "[Blocks] Failed to load: %s\n", file); \
            success = false; \
        }
    
    LOAD_TEXTURE(g_texHyperspace, "hypspc.png");
    LOAD_TEXTURE(g_texBullet, "speed.png");
    LOAD_TEXTURE(g_texMaxAmmo, "lotsammo.png");
    LOAD_TEXTURE(g_texRed, "redblk.png");
    LOAD_TEXTURE(g_texGreen, "grnblk.png");
    LOAD_TEXTURE(g_texBlue, "blueblk.png");
    LOAD_TEXTURE(g_texTan, "tanblk.png");
    LOAD_TEXTURE(g_texPurple, "purpblk.png");
    LOAD_TEXTURE(g_texYellow, "yellblk.png");
    LOAD_TEXTURE(g_texBlack, "blakblk.png");
    LOAD_TEXTURE(g_texRoamer, "roamer.png");
    LOAD_TEXTURE(g_texBomb, "bombblk.png");
    LOAD_TEXTURE(g_texDeath, "death1.png");
    LOAD_TEXTURE(g_texExtraBall, "xtrabal.png");
    LOAD_TEXTURE(g_texMGun, "machgun.png");
    LOAD_TEXTURE(g_texWallOff, "walloff.png");
    LOAD_TEXTURE(g_texRandom, "redblk.png");
    LOAD_TEXTURE(g_texDrop, "grnblk.png");
    LOAD_TEXTURE(g_texTimer, "clock.png");
    LOAD_TEXTURE(g_texMultiBall, "multibal.png");
    LOAD_TEXTURE(g_texSticky, "stkyblk.png");
    LOAD_TEXTURE(g_texReverse, "reverse.png");
    LOAD_TEXTURE(g_texPadShrink, "padshrk.png");
    LOAD_TEXTURE(g_texPadExpand, "padexpn.png");
    
    LOAD_TEXTURE(g_texCounter[0], "cntblk.png");
    LOAD_TEXTURE(g_texCounter[1], "cntblk1.png");
    LOAD_TEXTURE(g_texCounter[2], "cntblk2.png");
    LOAD_TEXTURE(g_texCounter[3], "cntblk3.png");
    LOAD_TEXTURE(g_texCounter[4], "cntblk4.png");
    LOAD_TEXTURE(g_texCounter[5], "cntblk5.png");
    
    #undef LOAD_TEXTURE
    
    if (success) {
        fprintf(stdout, "[Blocks] Successfully loaded all block textures\n");
    }
    
    return success;
}

void Blocks_FreeTextures(void) {
    UnloadTexture(g_texHyperspace);
    UnloadTexture(g_texBullet);
    UnloadTexture(g_texMaxAmmo);
    UnloadTexture(g_texRed);
    UnloadTexture(g_texGreen);
    UnloadTexture(g_texBlue);
    UnloadTexture(g_texTan);
    UnloadTexture(g_texPurple);
    UnloadTexture(g_texYellow);
    UnloadTexture(g_texBlack);
    UnloadTexture(g_texRoamer);
    UnloadTexture(g_texBomb);
    UnloadTexture(g_texDeath);
    UnloadTexture(g_texExtraBall);
    UnloadTexture(g_texMGun);
    UnloadTexture(g_texWallOff);
    UnloadTexture(g_texRandom);
    UnloadTexture(g_texDrop);
    UnloadTexture(g_texTimer);
    UnloadTexture(g_texMultiBall);
    UnloadTexture(g_texSticky);
    UnloadTexture(g_texReverse);
    UnloadTexture(g_texPadShrink);
    UnloadTexture(g_texPadExpand);
    
    for (int i = 0; i < 6; i++) {
        UnloadTexture(g_texCounter[i]);
    }
    
    fprintf(stdout, "[Blocks] Textures freed\n");
}


// =============================================================================
// Public Functions - Block Queries
// =============================================================================

Rectangle Blocks_GetCollisionRect(int row, int col) {
    return (Rectangle){
        g_blocks[row][col].position.x,
        g_blocks[row][col].position.y,
        g_blocks[row][col].texture.width,
        g_blocks[row][col].texture.height
    };
}

bool Blocks_IsActive(int row, int col) {
    if (row < 0 || row >= BLOCK_ROWS_MAX || col < 0 || col >= BLOCK_COLS_MAX) {
        return false;
    }
    return g_blocks[row][col].active;
}

int Blocks_GetRowMax(void) {
    return BLOCK_ROWS_MAX;
}

int Blocks_GetColMax(void) {
    return BLOCK_COLS_MAX;
}

int Blocks_GetRemainingCount(void) {
    return g_blocksRemaining;
}


static void DeactivateBlock(int row, int col) {
    if (!g_blocks[row][col].active || g_blocks[row][col].type == 'w') {
        return;  // Already inactive or indestructible wall
    }
    g_blocks[row][col].active = false;
    g_blocksRemaining--;
}

static bool IsBlockTypeIndestructible(char type) {
    return (type == 'w');
}

static Vector2 GetPlayCorner(Corner corner) {
    switch (corner) {
        case CORNER_UPPER_LEFT:
            return (Vector2){PLAY_X_OFFSET - 1, PLAY_Y_OFFSET - 1};
            
        case CORNER_UPPER_RIGHT:
            return (Vector2){
                PLAY_X_OFFSET + g_playArea.playWidth,
                PLAY_Y_OFFSET - 1
            };
            
        case CORNER_LOWER_LEFT:
            return (Vector2){
                PLAY_X_OFFSET - 1,
                PLAY_Y_OFFSET + g_playArea.playHeight
            };
            
        case CORNER_LOWER_RIGHT:
            return (Vector2){
                g_playArea.playWidth + 1,
                PLAY_Y_OFFSET + g_playArea.playHeight
            };
    }
    
    return (Vector2){0, 0};
}

// =============================================================================
// Public Functions - Wall Queries
// =============================================================================

Rectangle Blocks_GetWall(Wall wall) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    Vector2 upperLeft = GetPlayCorner(CORNER_UPPER_LEFT);
    Vector2 upperRight = GetPlayCorner(CORNER_UPPER_RIGHT);
    Vector2 lowerLeft = GetPlayCorner(CORNER_LOWER_LEFT);
    
    switch (wall) {
        case WALL_LEFT:
            return (Rectangle){0, 0, lowerLeft.x, screenHeight};
            
        case WALL_RIGHT:
            return (Rectangle){
                upperRight.x,
                0,
                screenWidth - upperRight.x,
                screenHeight
            };
            
        case WALL_TOP:
            return (Rectangle){0, 0, screenWidth, upperRight.y};
            
        case WALL_BOTTOM:
            return (Rectangle){
                0,
                lowerLeft.y + upperLeft.y,
                screenWidth,
                screenHeight - lowerLeft.y
            };
    }
    
    return (Rectangle){0, 0, 0, 0};
}

// =============================================================================
// Public Functions - Block Interaction
// =============================================================================

void Blocks_Activate(int row, int col) {
    Block *block = &g_blocks[row][col];
    
    switch (block->type) {
        case 'w':  // Indestructible wall
            break;
            
        case 's':  // Sticky paddle
            Ball_SetSticky();
            Audio_PlaySound(SND_STICKY);
            DeactivateBlock(row, col);
            break;
            
        case 'R':  // Reverse paddle controls
            ToggleReverse();
            Audio_PlaySound(SND_WARP);
            DeactivateBlock(row, col);
            break;
            
        case 'B':  // Increase ball speed
            Ball_IncreaseSpeed();
            Audio_PlaySound(SND_BOING);
            DeactivateBlock(row, col);
            break;
            
        case '<':  // Shrink paddle
            ChangePaddleSize(SIZE_DOWN);
            Audio_PlaySound(SND_WZZZ2);
            DeactivateBlock(row, col);
            break;
            
        case '>':  // Expand paddle
            ChangePaddleSize(SIZE_UP);
            Audio_PlaySound(SND_WZZZ);
            DeactivateBlock(row, col);
            break;
            
        case 'X':  // Bomb - destroy surrounding blocks
            Audio_PlaySound(SND_BOMB);
            for (int i = -1; i <= 1; i++) {
                int r = row + i;
                if (r < 0 || r >= BLOCK_ROWS_MAX) continue;
                
                for (int j = -1; j <= 1; j++) {
                    int c = col + j;
                    if (c < 0 || c >= BLOCK_COLS_MAX) continue;
                    
                    DeactivateBlock(r, c);
                }
            }
            break;
            
        case '5':  // Counter block level 5
            Audio_PlaySound(SND_TOUCH);
            block->texture = g_texCounter[4];
            block->type = '4';
            break;
            
        case '4':  // Counter block level 4
            Audio_PlaySound(SND_TOUCH);
            block->texture = g_texCounter[3];
            block->type = '3';
            break;
            
        case '3':  // Counter block level 3
            Audio_PlaySound(SND_TOUCH);
            block->texture = g_texCounter[2];
            block->type = '2';
            break;
            
        case '2':  // Counter block level 2
            Audio_PlaySound(SND_TOUCH);
            block->texture = g_texCounter[1];
            block->type = '1';
            break;
            
        case '1':  // Counter block level 1
            Audio_PlaySound(SND_TOUCH);
            block->texture = g_texCounter[0];
            block->type = '0';
            break;
            
        default:  // All other blocks - simple destruction
            Audio_PlaySound(SND_TOUCH);
            DeactivateBlock(row, col);
            break;
    }
    
    // Check for level completion
    if (g_blocksRemaining == 0) {
        Audio_PlaySound(SND_APPLAUSE);
        SetGameMode(MODE_WIN);
    }
}
