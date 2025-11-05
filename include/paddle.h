/**
 * @file paddle.h
 * @brief Paddle system interface for XBoing
 * 
 * Manages paddle state, movement, rendering, and size changes.
 */

#ifndef PADDLE_H
#define PADDLE_H

#include <stdbool.h>
#include <raylib.h>
#include "core/types.h"
#include "core/constants.h"

// =============================================================================
// Public Functions - Initialization
// =============================================================================

/**
 * @brief Load paddle textures
 * @return true if successful, false otherwise
 */
bool Paddle_Initialize(void);

/**
 * @brief Free paddle resources
 */
void Paddle_Shutdown(void);

/**
 * @brief Reset paddle to starting position and size
 */
void Paddle_Reset(void);

// =============================================================================
// Public Functions - Rendering
// =============================================================================

/**
 * @brief Draw the paddle
 */
void Paddle_Draw(void);

// =============================================================================
// Public Functions - Movement
// =============================================================================

/**
 * @brief Move paddle in a direction
 * @param direction PADDLE_LEFT, PADDLE_RIGHT, or PADDLE_NONE
 */
void Paddle_Move(PaddleDirection direction);

/**
 * @brief Set paddle position directly (for mouse control)
 * @param x X coordinate position
 */
void Paddle_SetPosition(float x);

// =============================================================================
// Public Functions - State Queries
// =============================================================================

/**
 * @brief Get paddle width in pixels
 * @return Paddle width
 */
int Paddle_GetWidth(void);

/**
 * @brief Get paddle X position
 * @return X coordinate of paddle's left edge
 */
int Paddle_GetPositionX(void);

/**
 * @brief Get paddle Y position
 * @return Y coordinate of paddle's top edge
 */
int Paddle_GetPositionY(void);

/**
 * @brief Get paddle collision rectangle
 * @return Rectangle representing paddle bounds
 */
Rectangle Paddle_GetCollisionRect(void);

/**
 * @brief Get ball spawn point on paddle
 * @return Center point on top of paddle
 */
Vector2 Paddle_GetBallSpawnPoint(void);

/**
 * @brief Check if reverse controls are active
 * @return true if reversed, false otherwise
 */
bool Paddle_IsReversed(void);

/**
 * @brief Get paddle size description
 * @return String describing current size
 */
const char *Paddle_GetSizeDescription(void);

// =============================================================================
// Public Functions - State Modification
// =============================================================================

/**
 * @brief Toggle reverse control mode
 */
void Paddle_ToggleReverse(void);

/**
 * @brief Turn off reverse mode
 */
void Paddle_SetReverseOff(void);

/**
 * @brief Change paddle size
 * @param direction SIZE_UP or SIZE_DOWN
 */
void Paddle_ChangeSize(SizeChange direction);

// =============================================================================
// Legacy Compatibility
// =============================================================================
#define InitialisePaddle() Paddle_Initialize()
#define FreePaddle() Paddle_Shutdown()
#define DrawPaddle() Paddle_Draw()
#define MovePaddle(d) Paddle_Move(d)
#define SetPaddlePosition(x) Paddle_SetPosition(x)
#define GetPaddleSize() Paddle_GetWidth()
#define GetPaddlePositionX() Paddle_GetPositionX()
#define GetPaddlePositionY() Paddle_GetPositionY()
#define GetPaddleCollisionRec() Paddle_GetCollisionRect()
#define GetBallSpawnPointOnPaddle() Paddle_GetBallSpawnPoint()
#define GetPaddleReverse() Paddle_IsReversed()
#define GetPaddleDescription() Paddle_GetSizeDescription()
#define ResetPaddleStart() Paddle_Reset()
#define ToggleReverse() Paddle_ToggleReverse()
#define SetReverseOff() Paddle_SetReverseOff()
#define ChangePaddleSize(d) Paddle_ChangeSize(d)
#define GetPaddleWidth() ((float)Paddle_GetWidth())

#endif // PADDLE_H
