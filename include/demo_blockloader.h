/**
 * @file demo_blockloader.h
 * @brief Block system interface for XBoing
 * 
 * Manages level loading, block rendering, and block collision detection.
 */

#ifndef DEMO_BLOCKLOADER_H
#define DEMO_BLOCKLOADER_H

#include <raylib.h>
#include <stdbool.h>
#include "core/types.h"

// =============================================================================
// Public Functions - Initialization
// =============================================================================

/**
 * @brief Initialize the play area dimensions
 */
void Blocks_InitializePlayArea(void);

/**
 * @brief Load all block textures
 * @return true if successful, false otherwise
 */
bool Blocks_LoadTextures(void);

/**
 * @brief Free all block textures
 */
void Blocks_FreeTextures(void);

// =============================================================================
// Public Functions - Level Loading
// =============================================================================

/**
 * @brief Load a level from a file
 * @param filename Path to the level file
 * @return true if successful, false otherwise
 */
bool Blocks_LoadLevel(const char *filename);

// =============================================================================
// Public Functions - Rendering
// =============================================================================

/**
 * @brief Draw all active blocks
 */
void Blocks_Draw(void);

/**
 * @brief Draw the play area border
 */
void Blocks_DrawBorder(void);

/**
 * @brief Draw the wall areas (gray regions outside play area)
 */
void Blocks_DrawWalls(void);

// =============================================================================
// Public Functions - Block Queries
// =============================================================================

/**
 * @brief Get the collision rectangle for a block
 * @param row Block row
 * @param col Block column
 * @return Rectangle representing block bounds
 */
Rectangle Blocks_GetCollisionRect(int row, int col);

/**
 * @brief Check if a block is active
 * @param row Block row
 * @param col Block column
 * @return true if block exists and is active
 */
bool Blocks_IsActive(int row, int col);

/**
 * @brief Get the maximum number of rows
 * @return Maximum row count
 */
int Blocks_GetRowMax(void);

/**
 * @brief Get the maximum number of columns
 * @return Maximum column count
 */
int Blocks_GetColMax(void);

/**
 * @brief Get the count of remaining destructible blocks
 * @return Number of blocks remaining
 */
int Blocks_GetRemainingCount(void);

// =============================================================================
// Public Functions - Block Interaction
// =============================================================================

/**
 * @brief Activate a block (handle collision with ball)
 * @param row Block row
 * @param col Block column
 */
void Blocks_Activate(int row, int col);

// =============================================================================
// Public Functions - Wall Queries
// =============================================================================

/**
 * @brief Get the rectangle for a wall area
 * @param wall Which wall to query
 * @return Rectangle representing the wall area
 */
Rectangle Blocks_GetWall(Wall wall);

// =============================================================================
// Legacy Compatibility
// =============================================================================
#define initializePlayArea() Blocks_InitializePlayArea()
#define loadBlockTextures() Blocks_LoadTextures()
#define freeBlockTextures() Blocks_FreeTextures()
#define loadBlocks(f) Blocks_LoadLevel(f)
#define drawBlocks() Blocks_Draw()
#define drawBorder() Blocks_DrawBorder()
#define drawWalls() Blocks_DrawWalls()
#define getBlockCollisionRec(r,c) Blocks_GetCollisionRect(r,c)
#define isBlockActive(r,c) Blocks_IsActive(r,c)
#define getBlockRowMax() Blocks_GetRowMax()
#define getBlockColMax() Blocks_GetColMax()
#define getBlockCount() Blocks_GetRemainingCount()
#define activateBlock(r,c) Blocks_Activate(r,c)
#define getPlayWall(w) Blocks_GetWall(w)

#endif // DEMO_BLOCKLOADER_H