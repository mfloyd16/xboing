/**
 * @file demo_ball.c
 * @brief Ball system implementation for XBoing
 * 
 * Manages ball state, movement, collision detection with blocks/walls/paddle.
 */

#include "demo_ball.h"
#include "core/constants.h"
#include "core/types.h"
#include "demo_gamemodes.h"
#include "demo_blockloader.h"
#include "paddle.h"
#include "audio.h"
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// =============================================================================
// Constants
// =============================================================================
static const float BOUNCE_ANGLE_VARIANCE_DEG = 10.0f;
static const float RELEASE_ANGLE_MIN = PI / 4.0f;    // 45 degrees
static const float RELEASE_ANGLE_MAX = 3.0f * PI / 4.0f; // 135 degrees
static const float GUIDE_ROTATION_SPEED = PI / 2.0f; // radians per second

// =============================================================================
// Global State
// =============================================================================
static Ball g_ball = {0};
static float g_releaseAngle = 0.0f;

// =============================================================================
// Forward Declarations
// =============================================================================
static Vector2 GetBallSpawnPoint(void);
static void AnimateBall(void);
static void DrawGuide(void);
static void HandleWallCollisions(bool *flipX, bool *flipY, bool *stepBack);
static void HandlePaddleCollision(bool *flipY);
static void HandleBlockCollisions(bool *flipX, bool *flipY, bool *stepBack);

// =============================================================================
// Public Functions
// =============================================================================

bool Ball_Initialize(void) {
    for (int i = 0; i < MAX_BALL_IMG_COUNT; i++) {
        char fileName[64];
        snprintf(fileName, sizeof(fileName), PATH_BALL_TEXTURES "ball%d.png", i + 1);

        g_ball.img[i] = LoadTexture(fileName);
        
        if (g_ball.img[i].id == 0) {
            fprintf(stderr, "[Ball] Failed to load texture: %s\n", fileName);
            return false;
        }
    }

    fprintf(stdout, "[Ball] Successfully loaded %d ball textures\n", MAX_BALL_IMG_COUNT);
    return true;
}

void Ball_Shutdown(void) {
    for (int i = 0; i < MAX_BALL_IMG_COUNT; i++) {
        UnloadTexture(g_ball.img[i]);
    }
    fprintf(stdout, "[Ball] Resources freed\n");
}

void Ball_Reset(void) {
    g_ball.position = GetBallSpawnPoint();
    g_ball.sticky = false;
    g_ball.attached = false;
    g_ball.spawned = true;
    g_ball.speed = 0;
    g_ball.velocity = (Vector2){0, 0};
    g_releaseAngle = PI / 2.0f;  // Straight up
}

void Ball_Release(void) {
    if (g_ball.spawned) {
        g_ball.spawned = false;
        g_ball.speed = INITIAL_BALL_SPEED;
        g_ball.velocity.x = cos(g_releaseAngle) * g_ball.speed;
        g_ball.velocity.y = sin(g_releaseAngle) * g_ball.speed;
        Audio_PlaySound(SND_BALLSHOT);
    }

    if (g_ball.attached) {
        g_ball.attached = false;
        Audio_PlaySound(SND_BALLSHOT);
    }
}

void Ball_Draw(void) {
    AnimateBall();
    
    if (g_ball.spawned) {
        DrawGuide();
    }
    
    DrawTexture(g_ball.img[g_ball.imgIndex], 
                (int)g_ball.position.x, 
                (int)g_ball.position.y, 
                WHITE);
}

void Ball_Update(void) {
    g_ball.oldPosition = g_ball.position;
    bool stepBack = false;
    bool flipX = false;
    bool flipY = false;

    // Keep spawned ball on paddle center
    if (g_ball.spawned) {
        g_ball.position = GetBallSpawnPoint();
        return;
    }

    // Handle attached ball (sticky paddle)
    if (g_ball.attached) {
        g_ball.position = (Vector2){
            GetPaddlePositionX() - g_ball.anchor.x,
            GetPaddlePositionY() - g_ball.anchor.y
        };

        // Keep ball from hanging off edges when paddle hits walls
        int boundaryLeft = getPlayWall(WALL_LEFT).width;
        if (g_ball.position.x < boundaryLeft) {
            g_ball.position.x = boundaryLeft;
            g_ball.anchor.x = GetPaddlePositionX() - g_ball.position.x;
        }

        int boundaryRight = getPlayWall(WALL_RIGHT).x - g_ball.img[0].width;
        if (g_ball.position.x > boundaryRight) {
            g_ball.position.x = boundaryRight;
            g_ball.anchor.x = GetPaddlePositionX() - g_ball.position.x;
        }

        return;
    }

    // Move ball
    float deltaTime = GetFrameTime();
    g_ball.position.x += g_ball.velocity.x * deltaTime;
    g_ball.position.y -= g_ball.velocity.y * deltaTime;

    // Check collisions
    HandleWallCollisions(&flipX, &flipY, &stepBack);
    HandlePaddleCollision(&flipY);
    HandleBlockCollisions(&flipX, &flipY, &stepBack);

    // Apply collision responses
    if (stepBack) {
        g_ball.position = g_ball.oldPosition;
    }
    
    if (flipX) {
        g_ball.velocity.x *= -1;
    }
    
    if (flipY) {
        g_ball.velocity.y *= -1;
    }

    // Add variance to angle on bounce
    if (flipX || flipY) {
        float angle = atan2(g_ball.velocity.y, g_ball.velocity.x);
        float variance = ((rand() % 21) - BOUNCE_ANGLE_VARIANCE_DEG) * (PI / 180.0f);
        angle += variance;
        
        g_ball.velocity.x = cos(angle) * g_ball.speed;
        g_ball.velocity.y = sin(angle) * g_ball.speed;
    }
}

void Ball_SetSticky(void) {
    g_ball.sticky = true;
}

void Ball_IncreaseSpeed(void) {
    g_ball.speed = (int)(g_ball.speed * 1.25f);
    
    // Update velocity to match new speed
    float angle = atan2(g_ball.velocity.y, g_ball.velocity.x);
    g_ball.velocity.x = cos(angle) * g_ball.speed;
    g_ball.velocity.y = sin(angle) * g_ball.speed;
}

Rectangle Ball_GetCollisionRect(void) {
    return (Rectangle){
        g_ball.position.x,
        g_ball.position.y,
        g_ball.img[g_ball.imgIndex].width,
        g_ball.img[g_ball.imgIndex].height
    };
}

// =============================================================================
// Private Helper Functions
// =============================================================================

static Vector2 GetBallSpawnPoint(void) {
    Vector2 spawn = GetBallSpawnPointOnPaddle();
    return (Vector2){
        spawn.x - g_ball.img[g_ball.imgIndex].width / 2,
        spawn.y - g_ball.img[g_ball.imgIndex].height
    };
}

static void AnimateBall(void) {
    static float elapsedTime = 0.0f;
    
    elapsedTime += GetFrameTime();
    
    if (elapsedTime > 0.1f) {
        elapsedTime = 0.0f;
        g_ball.imgIndex = (g_ball.imgIndex + 1) % MAX_BALL_IMG_COUNT;
    }
}

static void DrawGuide(void) {
    static int rotationDirection = 1;
    
    const float centerAngle = PI / 2.0f;
    const float angleRange = PI / 4.0f;
    
    // Rotate the guide line
    g_releaseAngle += GUIDE_ROTATION_SPEED * GetFrameTime() * rotationDirection;
    
    // Reverse direction at boundaries
    if (g_releaseAngle > centerAngle + angleRange && rotationDirection == 1) {
        rotationDirection = -1;
    } else if (g_releaseAngle < centerAngle - angleRange && rotationDirection == -1) {
        rotationDirection = 1;
    }
    
    // Calculate guide line endpoints
    Vector2 startPoint = {
        g_ball.position.x + g_ball.img[0].width / 2,
        g_ball.position.y + g_ball.img[0].height / 2
    };
    
    Vector2 endPoint = {
        startPoint.x + cos(g_releaseAngle) * GUIDE_LENGTH,
        startPoint.y - sin(g_releaseAngle) * GUIDE_LENGTH
    };
    
    DrawLineV(startPoint, endPoint, YELLOW);
}

static void HandleWallCollisions(bool *flipX, bool *flipY, bool *stepBack) {
    Rectangle ballRect = Ball_GetCollisionRect();
    
    // Check bottom wall (ball lost)
    if (CheckCollisionRecs(ballRect, getPlayWall(WALL_BOTTOM))) {
        g_ball.position.y = GetScreenHeight(); // Hide ball
        Audio_PlaySound(SND_BALLLOST);
        SetGameMode(MODE_LOSE);
        return;
    }
    
    // Check top wall
    if (CheckCollisionRecs(ballRect, getPlayWall(WALL_TOP))) {
        Audio_PlaySound(SND_BOING);
        *stepBack = true;
        *flipY = true;
    }
    
    // Check left wall
    if (CheckCollisionRecs(ballRect, getPlayWall(WALL_LEFT))) {
        Audio_PlaySound(SND_BOING);
        *stepBack = true;
        *flipX = true;
    }
    
    // Check right wall
    if (CheckCollisionRecs(ballRect, getPlayWall(WALL_RIGHT))) {
        Audio_PlaySound(SND_BOING);
        *stepBack = true;
        *flipX = true;
    }
}

static void HandlePaddleCollision(bool *flipY) {
    Rectangle ballRect = Ball_GetCollisionRect();
    Rectangle paddleRect = GetPaddleCollisionRec();
    
    if (CheckCollisionRecs(ballRect, paddleRect)) {
        *flipY = true;
        g_ball.position.y = GetPaddlePositionY() - g_ball.img[g_ball.imgIndex].height;
        Audio_PlaySound(SND_PADDLE);
        
        if (g_ball.sticky) {
            g_ball.sticky = false;
            g_ball.attached = true;
            g_ball.anchor = (Vector2){
                GetPaddlePositionX() - g_ball.position.x,
                GetPaddlePositionY() - g_ball.position.y
            };
        }
    }
}

static void HandleBlockCollisions(bool *flipX, bool *flipY, bool *stepBack) {
    Rectangle ballRect = Ball_GetCollisionRect();
    
    for (int row = 0; row < getBlockRowMax(); row++) {
        for (int col = 0; col < getBlockColMax(); col++) {
            if (!isBlockActive(row, col)) {
                continue;
            }

            Rectangle blockRect = getBlockCollisionRec(row, col);
            
            if (CheckCollisionRecs(ballRect, blockRect)) {
                *stepBack = true;
                activateBlock(row, col);

                // Calculate collision side
                float ballCenterX = g_ball.position.x + g_ball.img[0].width / 2;
                float ballCenterY = g_ball.position.y + g_ball.img[0].height / 2;
                float blockCenterX = blockRect.x + blockRect.width / 2;
                float blockCenterY = blockRect.y + blockRect.height / 2;

                float deltaX = ballCenterX - blockCenterX;
                float deltaY = ballCenterY - blockCenterY;

                float overlapX = (g_ball.img[0].width + blockRect.width) / 2 - fabs(deltaX);
                float overlapY = (g_ball.img[0].height + blockRect.height) / 2 - fabs(deltaY);

                // Determine bounce direction based on smallest overlap
                if (overlapX < overlapY) {
                    *flipX = true;
                } else {
                    *flipY = true;
                }
                
                return; // Only handle one block collision per frame
            }
        }
    }
}
