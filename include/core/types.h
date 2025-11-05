/**
 * @file types.h
 * @brief Centralized type definitions for the Rayboing game
 * 
 * This file contains all custom type definitions, enumerations, and structures
 * used throughout the game, reducing circular dependencies and improving code organization.
 */

#ifndef TYPES_H
#define TYPES_H

#include <raylib.h>
#include <stdbool.h>

// =============================================================================
// Boolean Type (for legacy compatibility)
// =============================================================================
#ifndef True
#define True  1
#define False 0
#endif

// =============================================================================
// Game State Types
// =============================================================================

/**
 * @brief Game mode states for the state machine
 */
typedef enum {
    MODE_INITGAME,  // Initialize/restart a level
    MODE_PLAY,      // Active gameplay
    MODE_WIN,       // Level completed successfully
    MODE_LOSE,      // Lost a life
    MODE_CANCEL,    // Game cancelled by user
    MODE_EXIT       // Exit the application
} GameMode;

// =============================================================================
// Block Types
// =============================================================================

/**
 * @brief Defines the corners of the play area
 */
typedef enum {
    CORNER_UPPER_LEFT,
    CORNER_UPPER_RIGHT,
    CORNER_LOWER_LEFT,
    CORNER_LOWER_RIGHT
} Corner;

/**
 * @brief Defines the walls surrounding the play area
 */
typedef enum {
    WALL_LEFT,
    WALL_RIGHT,
    WALL_TOP,
    WALL_BOTTOM
} Wall;

/**
 * @brief Represents a single block in the game
 */
typedef struct {
    int blockOffsetX;
    int blockOffsetY;
    Vector2 position;
    Texture2D texture;
    char type;
    bool active;
} Block;

/**
 * @brief Play area dimensions and layout information
 */
typedef struct {
    int playWidth;
    int playHeight;
    int colWidth;
    int rowHeight;
} PlayArea;

// =============================================================================
// Paddle Types
// =============================================================================

/**
 * @brief Paddle movement directions
 */
typedef enum {
    PADDLE_NONE = 0,
    PADDLE_LEFT = 1,
    PADDLE_RIGHT = 2
} PaddleDirection;

/**
 * @brief Paddle size change directions
 */
typedef enum {
    SIZE_UP = 1,
    SIZE_DOWN = 2
} SizeChange;

/**
 * @brief Represents a paddle configuration
 */
typedef struct {
    Texture2D img;
    const char *description;
    int size;
    const char *filepath;
} Paddle;

// =============================================================================
// Ball Types
// =============================================================================

/**
 * @brief Represents the ball state
 */
typedef struct {
    Texture2D img[4];
    int imgIndex;
    Vector2 position;
    Vector2 oldPosition;
    Vector2 velocity;
    int speed;
    bool sticky;
    bool attached;
    bool spawned;
    Vector2 anchor;
} Ball;

// =============================================================================
// Audio Types
// =============================================================================

/**
 * @brief Sound effect identifiers
 */
typedef enum {
    SND_AMMO,
    SND_APPLAUSE,
    SND_BALL2BALL,
    SND_BALLLOST,
    SND_BALLSHOT,
    SND_BOING,
    SND_BOMB,
    SND_BONUS,
    SND_BUZZER,
    SND_CLICK,
    SND_DDLOO,
    SND_DOH1,
    SND_DOH2,
    SND_DOH3,
    SND_DOH4,
    SND_EVILLAUGH,
    SND_GAMEOVER,
    SND_GATE,
    SND_HITHERE,
    SND_HYPSPC,
    SND_INTRO,
    SND_KEY,
    SND_LOOKSBAD,
    SND_METAL,
    SND_MGUN,
    SND_OUCH,
    SND_PADDLE,
    SND_PING,
    SND_SHARK,
    SND_SHOOT,
    SND_SHOTGUN,
    SND_SPRING,
    SND_STAMP,
    SND_STICKY,
    SND_SUPBONS,
    SND_TOGGLE,
    SND_TONE,
    SND_TOUCH,
    SND_WALLSOFF,
    SND_WARP,
    SND_WEEEK,
    SND_WHIZZO,
    SND_WHOOSH,
    SND_WZZZ,
    SND_WZZZ2,
    SND_YOUAGOD,
    SOUND_COUNT  // Total number of sounds
} SoundID;

/**
 * @brief Audio system state
 */
typedef struct {
    Sound sounds[46];  // SOUND_COUNT
    float masterVolume;
} AudioSystem;

#endif // TYPES_H
