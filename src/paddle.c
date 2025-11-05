/**
 * @file paddle.c
 * @brief Paddle system implementation for XBoing
 * 
 * Manages paddle state, movement, size changes, and reverse controls.
 */

#include "paddle.h"
#include "demo_blockloader.h"
#include "core/constants.h"
#include "core/types.h"
#include <stdio.h>
#include <raylib.h>

// =============================================================================
// Global State
// =============================================================================
static Paddle g_paddles[PADDLE_COUNT];
static int g_currentPaddleIndex = PADDLE_INITIAL_INDEX;
static int g_paddlePosition = 0;
static bool g_reverseMode = false;

// =============================================================================
// Public Functions - Initialization
// =============================================================================

bool Paddle_Initialize(void) {
    if (WindowShouldClose()) {
        return false;
    }

    // Define paddle configurations (must load smallest to largest)
    const char *paddleFiles[PADDLE_COUNT] = {
        PATH_PADDLE_TEXTURES "padsml.png",
        PATH_PADDLE_TEXTURES "padmed.png",
        PATH_PADDLE_TEXTURES "padhuge.png"
    };
    
    const char *paddleDescs[PADDLE_COUNT] = {
        "Small",
        "Medium", 
        "Huge"
    };
    
    const int paddleSizes[PADDLE_COUNT] = {40, 50, 70};

    // Load paddle textures
    bool success = true;
    for (int i = 0; i < PADDLE_COUNT; i++) {
        g_paddles[i].filepath = paddleFiles[i];
        g_paddles[i].description = paddleDescs[i];
        g_paddles[i].size = paddleSizes[i];
        
        Image img = LoadImage(g_paddles[i].filepath);
        g_paddles[i].img = LoadTextureFromImage(img);
        UnloadImage(img);

        if (g_paddles[i].img.id == 0) {
            fprintf(stderr, "[Paddle] Failed to load texture: %s\n", g_paddles[i].filepath);
            success = false;
        }
    }

    if (success) {
        fprintf(stdout, "[Paddle] Successfully loaded %d paddle textures\n", PADDLE_COUNT);
    }
    
    return success;
}

void Paddle_Shutdown(void) {
    for (int i = 0; i < PADDLE_COUNT; i++) {
        UnloadTexture(g_paddles[i].img);
    }
    fprintf(stdout, "[Paddle] Resources freed\n");
}

void Paddle_Reset(void) {
    g_currentPaddleIndex = PADDLE_INITIAL_INDEX;
    g_paddlePosition = (SCREEN_WIDTH - g_paddles[g_currentPaddleIndex].size) / 2;
    g_reverseMode = false;
}

// =============================================================================
// Public Functions - Rendering
// =============================================================================

void Paddle_Draw(void) {
    DrawTexture(g_paddles[g_currentPaddleIndex].img,
                g_paddlePosition,
                Paddle_GetPositionY(),
                WHITE);
}

// =============================================================================
// Public Functions - Movement
// =============================================================================

void Paddle_Move(PaddleDirection direction) {
    if (direction == PADDLE_NONE) {
        // Just clamp to boundaries
        int minX = Blocks_GetWall(WALL_LEFT).width;
        int maxX = Blocks_GetWall(WALL_RIGHT).x - g_paddles[g_currentPaddleIndex].size;
        
        if (g_paddlePosition < minX) g_paddlePosition = minX;
        if (g_paddlePosition > maxX) g_paddlePosition = maxX;
        return;
    }

    // Calculate movement with reverse mode consideration
    int distance = PADDLE_VELOCITY * GetFrameTime();
    if (g_reverseMode) distance = -distance;

    // Apply movement
    if (direction == PADDLE_LEFT) {
        g_paddlePosition -= distance;
    } else if (direction == PADDLE_RIGHT) {
        g_paddlePosition += distance;
    }

    // Clamp to boundaries
    int minX = Blocks_GetWall(WALL_LEFT).width;
    int maxX = Blocks_GetWall(WALL_RIGHT).x - g_paddles[g_currentPaddleIndex].size;
    
    if (g_paddlePosition < minX) g_paddlePosition = minX;
    if (g_paddlePosition > maxX) g_paddlePosition = maxX;
}

void Paddle_SetPosition(float x) {
    int minX = Blocks_GetWall(WALL_LEFT).width;
    int maxX = Blocks_GetWall(WALL_RIGHT).x - g_paddles[g_currentPaddleIndex].size;

    if (x < minX) x = minX;
    if (x > maxX) x = maxX;

    g_paddlePosition = (int)x;
}

// =============================================================================
// Public Functions - State Queries
// =============================================================================

int Paddle_GetWidth(void) {
    return g_paddles[g_currentPaddleIndex].size;
}

int Paddle_GetPositionX(void) {
    return g_paddlePosition;
}

int Paddle_GetPositionY(void) {
    return Blocks_GetWall(WALL_BOTTOM).y - PADDLE_DIST_BASE;
}

Rectangle Paddle_GetCollisionRect(void) {
    return (Rectangle){
        g_paddlePosition,
        Paddle_GetPositionY(),
        g_paddles[g_currentPaddleIndex].img.width,
        g_paddles[g_currentPaddleIndex].img.height
    };
}

Vector2 Paddle_GetBallSpawnPoint(void) {
    return (Vector2){
        g_paddlePosition + g_paddles[g_currentPaddleIndex].size / 2,
        Paddle_GetPositionY()
    };
}

bool Paddle_IsReversed(void) {
    return g_reverseMode;
}

const char *Paddle_GetSizeDescription(void) {
    return g_paddles[g_currentPaddleIndex].description;
}

// =============================================================================
// Public Functions - State Modification
// =============================================================================

void Paddle_ToggleReverse(void) {
    g_reverseMode = !g_reverseMode;
}

void Paddle_SetReverseOff(void) {
    g_reverseMode = false;
}

void Paddle_ChangeSize(SizeChange direction) {
    int oldSize = g_paddles[g_currentPaddleIndex].size;

    // Change paddle index
    if (direction == SIZE_UP && g_currentPaddleIndex < PADDLE_COUNT - 1) {
        g_currentPaddleIndex++;
    } else if (direction == SIZE_DOWN && g_currentPaddleIndex > 0) {
        g_currentPaddleIndex--;
    }

    // Adjust position to center the size change
    int newSize = g_paddles[g_currentPaddleIndex].size;
    g_paddlePosition -= (newSize - oldSize) / 2;

    // Ensure paddle stays within boundaries
    Paddle_Move(PADDLE_NONE);
}
