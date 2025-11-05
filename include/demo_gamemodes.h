/**
 * @file demo_gamemodes.h
 * @brief Game mode state machine interface for XBoing
 * 
 * Manages game state transitions and mode-specific logic.
 */

#ifndef DEMO_GAMEMODES_H
#define DEMO_GAMEMODES_H

#include "core/types.h"

// =============================================================================
// State Machine Functions
// =============================================================================

/**
 * @brief Get the current game mode
 * @return Current game mode
 */
GameMode GameMode_GetCurrent(void);

/**
 * @brief Set the game mode
 * @param mode New game mode to transition to
 */
void GameMode_SetCurrent(GameMode mode);

// =============================================================================
// Mode Update Functions
// =============================================================================

/**
 * @brief Run initialization/restart mode logic
 * @param levelFile Path to level file to load
 */
void GameMode_RunInit(const char *levelFile);

/**
 * @brief Run active gameplay mode logic
 */
void GameMode_RunPlay(void);

/**
 * @brief Run end-of-level mode logic (win/lose/cancel)
 */
void GameMode_RunEnd(void);

// =============================================================================
// Legacy Compatibility
// =============================================================================
#define GetGameMode() GameMode_GetCurrent()
#define SetGameMode(m) GameMode_SetCurrent(m)
#define RunInitGameMode(f) GameMode_RunInit(f)
#define RunPlayMode() GameMode_RunPlay()
#define RunEndMode() GameMode_RunEnd()

#endif // DEMO_GAMEMODES_H