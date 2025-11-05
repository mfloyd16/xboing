/**
 * @file demo_ball.h
 * @brief Ball system interface for XBoing
 * 
 * Manages ball state, movement, collision detection, and rendering.
 */

#ifndef DEMO_BALL_H
#define DEMO_BALL_H

#include <stdbool.h>
#include <raylib.h>

// =============================================================================
// Public Functions
// =============================================================================

/**
 * @brief Initialize the ball system and load textures
 * @return true if successful, false otherwise
 */
bool Ball_Initialize(void);

/**
 * @brief Reset ball to starting position on paddle
 */
void Ball_Reset(void);

/**
 * @brief Free all ball resources
 */
void Ball_Shutdown(void);

/**
 * @brief Update ball position and handle collisions
 */
void Ball_Update(void);

/**
 * @brief Render the ball
 */
void Ball_Draw(void);

/**
 * @brief Release the ball from paddle or sticky state
 */
void Ball_Release(void);

/**
 * @brief Enable sticky mode for next paddle collision
 */
void Ball_SetSticky(void);

/**
 * @brief Increase ball speed
 */
void Ball_IncreaseSpeed(void);

/**
 * @brief Get ball collision rectangle
 * @return Rectangle representing ball bounds
 */
Rectangle Ball_GetCollisionRect(void);

// =============================================================================
// Legacy Compatibility Functions
// =============================================================================
#define InitializeBall() Ball_Initialize()
#define ResetBall() Ball_Reset()
#define FreeBall() Ball_Shutdown()
#define MoveBall() Ball_Update()
#define DrawBall() Ball_Draw()
#define ReleaseBall() Ball_Release()
#define SetBallSticky() Ball_SetSticky()
#define IncreaseBallSpeed() Ball_IncreaseSpeed()

#endif // DEMO_BALL_H