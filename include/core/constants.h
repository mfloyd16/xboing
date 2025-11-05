/**
 * @file constants.h
 * @brief Centralized constants for the Rayboing game
 * 
 * This file contains all game-wide constants including screen dimensions,
 * game physics parameters, and file paths for resources.
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

// =============================================================================
// Screen and Window Constants
// =============================================================================
#define SCREEN_WIDTH  575
#define SCREEN_HEIGHT 720

// =============================================================================
// Play Area Constants
// =============================================================================
#define PLAY_X_OFFSET    35
#define PLAY_Y_OFFSET    60
#define PLAY_X_PADDING   40
#define PLAY_Y_PADDING   70
#define PLAY_BORDER_WIDTH 2

// =============================================================================
// Block Constants
// =============================================================================
#define BLOCK_WIDTH       40
#define BLOCK_HEIGHT      20
#define BLOCK_ROWS_MAX    15
#define BLOCK_COLS_MAX     9

// =============================================================================
// Ball Constants
// =============================================================================
#define INITIAL_BALL_SPEED   400  // pixels per second
#define MAX_BALL_IMG_COUNT     4
#define GUIDE_LENGTH         100
#define BOUNCE_VARIANCE       10.0f

// =============================================================================
// Paddle Constants
// =============================================================================
#define PADDLE_COUNT          3
#define PADDLE_INITIAL_INDEX  1
#define PADDLE_VELOCITY     600  // pixels per second
#define PADDLE_DIST_BASE     30
#define PADDLE_ROWS           3

// =============================================================================
// Game Constants
// =============================================================================
#define INITIAL_LIVES         3

// =============================================================================
// Resource Paths
// =============================================================================
#define PATH_BALL_TEXTURES    "resource/textures/balls/"
#define PATH_BLOCK_TEXTURES   "resource/textures/blocks/"
#define PATH_PADDLE_TEXTURES  "resource/textures/paddle/"
#define PATH_SOUNDS           "resource/sounds/"
#define PATH_LEVELS           "resource/levels/"
#define DEFAULT_LEVEL         "resource/levels/level01.data"

#endif // CONSTANTS_H
