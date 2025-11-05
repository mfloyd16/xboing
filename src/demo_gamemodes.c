/**
 * @file demo_gamemodes.c
 * @brief Game mode state machine implementation for XBoing
 * 
 * Manages game state transitions, life tracking, and level progression.
 */

#include "demo_gamemodes.h"
#include "core/constants.h"
#include "core/types.h"
#include "paddle.h"
#include "demo_controls.h"
#include "demo_blockloader.h"
#include "demo_ball.h"
#include "audio.h"
#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

// =============================================================================
// Global State
// =============================================================================
static GameMode g_currentMode = MODE_EXIT;
static int g_livesRemaining = 0;
static char g_currentLevelFile[512] = {0};

// =============================================================================
// Forward Declarations
// =============================================================================
static void RenderGameScreen(void);
static void DrawStatusText(const char *text);
static bool TryLoadNextLevel(void);

// =============================================================================
// State Machine Functions
// =============================================================================

GameMode GameMode_GetCurrent(void) {
    return g_currentMode;
}

void GameMode_SetCurrent(GameMode mode) {
    g_currentMode = mode;
}

// =============================================================================
// Mode Update Functions
// =============================================================================

void GameMode_RunInit(const char *levelFile) {
    // Determine if we need to reload level data
    bool needsReload = (g_livesRemaining <= 0) ||
                       (levelFile && g_currentLevelFile[0] != '\0' && strcmp(levelFile, g_currentLevelFile) != 0) ||
                       (levelFile && g_currentLevelFile[0] == '\0');

    if (needsReload) {
        Blocks_LoadLevel(levelFile);
        
        if (levelFile) {
            strncpy(g_currentLevelFile, levelFile, sizeof(g_currentLevelFile) - 1);
            g_currentLevelFile[sizeof(g_currentLevelFile) - 1] = '\0';
        }
        
        g_livesRemaining = INITIAL_LIVES;
    }

    g_livesRemaining--;
    
    Paddle_Reset();
    Ball_Reset();
    
    RenderGameScreen();
    
    GameMode_SetCurrent(MODE_PLAY);
}

void GameMode_RunPlay(void) {
    // Handle keyboard controls
    if (IsInputPaddleLeft()) {
        Paddle_Move(PADDLE_LEFT);
    }
    if (IsInputPaddleRight()) {
        Paddle_Move(PADDLE_RIGHT);
    }

    // Handle mouse controls
    Vector2 mousePos = GetMousePosition();
    Paddle_SetPosition(mousePos.x);

    // Handle ball release
    if (IsInputReleaseBall()) {
        Ball_Release();
    }

    // Update game objects
    Ball_Update();

    // Render everything
    RenderGameScreen();

    // Handle quit
    if (IsInputQuitGame()) {
        g_livesRemaining = 0;
        GameMode_SetCurrent(MODE_CANCEL);
    }
}

void GameMode_RunEnd(void) {
    RenderGameScreen();

    // Handle quit request
    if (IsInputQuitGame()) {
        GameMode_SetCurrent(MODE_EXIT);
        return;
    }

    // Handle win screen - allow advancing to next level
    if (GameMode_GetCurrent() == MODE_WIN) {
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) || IsKeyPressed(KEY_SPACE)) {
            if (TryLoadNextLevel()) {
                return;  // Successfully advanced
            }
            // No next level found, exit
            GameMode_SetCurrent(MODE_EXIT);
            return;
        }
    }

    // Handle lose/cancel screens - allow restart
    if (IsInputRestartAfterEnd()) {
        if (g_currentLevelFile[0] != '\0') {
            GameMode_RunInit(g_currentLevelFile);
        } else {
            GameMode_SetCurrent(MODE_INITGAME);
        }
    }
}

// =============================================================================
// Private Helper Functions
// =============================================================================

static bool TryLoadNextLevel(void) {
    if (g_currentLevelFile[0] == '\0') {
        return false;
    }

    // Find the last numeric sequence in the filename
    const char *p = g_currentLevelFile + strlen(g_currentLevelFile) - 1;
    while (p >= g_currentLevelFile && !isdigit((unsigned char)*p)) {
        p--;
    }
    
    if (p < g_currentLevelFile) {
        return false;  // No numeric sequence found
    }

    // Find start of digit sequence
    const char *end = p;
    const char *start = end;
    while (start > g_currentLevelFile && isdigit((unsigned char)*(start - 1))) {
        start--;
    }

    // Extract and increment the number
    int len = (int)(end - start) + 1;
    char numberBuf[32] = {0};
    strncpy(numberBuf, start, len);
    numberBuf[len] = '\0';

    int currentNum = atoi(numberBuf);
    int nextNum = currentNum + 1;

    // Build next filename with zero-padding preserved
    char nextFile[512] = {0};
    int prefixLen = (int)(start - g_currentLevelFile);
    
    if (prefixLen > 0) {
        strncpy(nextFile, g_currentLevelFile, prefixLen);
    }

    char fmt[16];
    snprintf(fmt, sizeof(fmt), "%%0%dd", len);
    
    char numStr[32];
    snprintf(numStr, sizeof(numStr), fmt, nextNum);
    strncat(nextFile, numStr, sizeof(nextFile) - strlen(nextFile) - 1);

    const char *rest = end + 1;
    if (*rest) {
        strncat(nextFile, rest, sizeof(nextFile) - strlen(nextFile) - 1);
    }

    // Check if next level exists
    FILE *f = fopen(nextFile, "r");
    if (f) {
        fclose(f);
        g_livesRemaining = 0;  // Force reload
        GameMode_RunInit(nextFile);
        return true;
    }

    return false;
}

static void RenderGameScreen(void) {
    BeginDrawing();
    ClearBackground(BLACK);

    // Draw game objects
    Blocks_DrawWalls();
    Blocks_Draw();
    Ball_Draw();
    Paddle_Draw();
    Blocks_DrawBorder();

    // Draw game mode specific overlays
    GameMode currentMode = GameMode_GetCurrent();
    
    switch (currentMode) {
        case MODE_WIN: {
            DrawStatusText("You Won! Congrats!!!");
            
            // Flashing prompt to advance
            const char *prompt = "Press SPACE or ENTER to Move On";
            const int PROMPT_SIZE = 20;
            int px = (SCREEN_WIDTH - MeasureText(prompt, PROMPT_SIZE)) / 2;
            int py = (SCREEN_HEIGHT / 3) + 80;
            
            double t = GetTime();
            double alpha = 0.5 * (1.0 + sin(6.283185307179586 * t));  // 2*PI
            if (alpha < 0.0) alpha = 0.0;
            if (alpha > 1.0) alpha = 1.0;
            
            Color textColor = {144, 238, 144, (unsigned char)(alpha * 255.0)};
            DrawText(prompt, px, py, PROMPT_SIZE, textColor);
            break;
        }
            
        case MODE_LOSE:
            if (g_livesRemaining > 0) {
                const char *txt = TextFormat("Remaining attempts: %d", g_livesRemaining);
                DrawStatusText(txt);
            } else {
                DrawStatusText("You Lost! Sadface...");
            }
            break;
            
        case MODE_CANCEL:
            DrawStatusText("Game canceled");
            break;
            
        default:
            break;
    }

    // Draw HUD
    const char *lives = TextFormat("Balls Remaining: %d", g_livesRemaining);
    DrawText(lives, 10, 10, 20, WHITE);

    const char *blocks = TextFormat("Blocks Remaining: %d", Blocks_GetRemainingCount());
    DrawText(blocks, SCREEN_WIDTH - MeasureText(blocks, 20) - 10, 10, 20, WHITE);

    if (Paddle_IsReversed()) {
        const char *reversed = "REVERSED!";
        DrawText(reversed, 
                (SCREEN_WIDTH - MeasureText(reversed, 25)) / 2, 
                35, 25, YELLOW);
    }

    EndDrawing();
}

static void DrawStatusText(const char *text) {
    const int FONTSIZE = 40;
    const int PADDING = 20;

    const int width = MeasureText(text, FONTSIZE);
    const int xpos = (SCREEN_WIDTH - width) / 2;
    const int ypos = SCREEN_HEIGHT / 3;

    DrawRectangle(xpos - PADDING, ypos - PADDING, 
                  width + 2 * PADDING, FONTSIZE + 2 * PADDING, 
                  BLACK);

    DrawText(text, xpos - 1, ypos - 1, FONTSIZE, RED);
    DrawText(text, xpos, ypos, FONTSIZE, GREEN);
}