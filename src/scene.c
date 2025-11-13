#include "../include/scene.h"
#include <raylib.h>
#include "../include/assets.h"
#include "../include/anim.h"
#include "../include/config.h"
#include "../include/level.h"
#include "../include/paddle.h"
#include "../include/ball.h"
#include "../include/block.h"
#include "../include/scenes/game.h"
#include <math.h>

/* Inner gameplay window constants */
static const int INNER_GAP_TOP = 60;
static const int INNER_GAP_BOTTOM = 75;
static const int INNER_GAP_LEFT = 35;
static const int INNER_GAP_RIGHT = 40;

/* Helper functions for inner gameplay window */
static int GetInnerWindowX(void) { return INNER_GAP_LEFT; }
static int GetInnerWindowY(void) { return INNER_GAP_TOP; }
static int GetInnerWindowWidth(void) { return WINDOW_WIDTH - INNER_GAP_LEFT - INNER_GAP_RIGHT; }
static int GetInnerWindowHeight(void) { return WINDOW_HEIGHT - INNER_GAP_TOP - INNER_GAP_BOTTOM; }

/* Draw bottom-left text in the outer window depending on current scene */
static void DrawSceneFooter(SceneId current)
{
    const int margin = 60;
    const int fontSize = 19;
    const Color neonGreen = { 0, 255, 0, 255 };

    const char* msg = "";
    switch (current)
    {
        case SCENE_INTRO:  msg = "Welcome to XBoing"; break;
        case SCENE_HOWTO:  msg = "Demonstration"; break;
        case SCENE_GAME:   msg = Level_GetActiveTitle(); break;
        default:           msg = ""; break;
    }

    if (msg[0] != '\0')
    {
        int y = WINDOW_HEIGHT - margin;
        /* If gameplay, prefix with 'Level: ' if not already present */
        if (current == SCENE_GAME) {
            const char* title = msg;
            if (title[0] != '\0') {
                DrawText("Level: ", margin, y, fontSize, neonGreen);
                int off = MeasureText("Level: ", fontSize);
                DrawText(title, margin + off, y, fontSize, neonGreen);
            }
        } else {
            DrawText(msg, margin + 19, y, fontSize, neonGreen);
        }
    }
}

/* Draw two-digit level counter in the top-right of the outer window */
static void DrawLevelCounter(SceneId current)
{
    int number = 0;
    if (current == SCENE_GAME) {
        number = Level_GetActiveNumber();
        if (number < 0) number = 0;
    }
    if (number > 99) number = 99; /* two-digit clamp */

    int tens = number / 10;
    int ones = number % 10;

    /* Position near top-right corner */
    const int margin = 12;
    int wT = digits[tens].width;
    int hT = digits[tens].height;
    int wO = digits[ones].width;
    int x = WINDOW_WIDTH - margin - wT - wO;
    int y = margin;

    DrawTexture(digits[tens], x, y, WHITE);
    DrawTexture(digits[ones], x + wT, y, WHITE);
}

void Scene_Init(void)
{
    /* Initialize scene resources via modules */
    Assets_Init();
    Anim_Init();
}

void Scene_Cleanup(void)
{
    Assets_Cleanup();
}

bool Scene_Update(SceneId* current)
{
    /* Update animation module */
    Anim_Update(GetFrameTime());
    
    switch (*current)
    {
        case SCENE_INTRO:
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) ||
                IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                *current = SCENE_HOWTO;
            }
            if (IsKeyPressed(KEY_G)) { *current = SCENE_PADDLE_TEST; }
            if (IsKeyPressed(KEY_B)) { *current = SCENE_BALL_TEST; }
            if (IsKeyPressed(KEY_L)) { *current = SCENE_BLOCK_TEST; }
            if (IsKeyPressed(KEY_S)) { *current = SCENE_GAME; }
            return true;

        case SCENE_HOWTO:
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) ||
                IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ESCAPE))
            {
                return false; /* request exit */
            }
            if (IsKeyPressed(KEY_G)) { *current = SCENE_PADDLE_TEST; }
            if (IsKeyPressed(KEY_B)) { *current = SCENE_BALL_TEST; }
            if (IsKeyPressed(KEY_L)) { *current = SCENE_BLOCK_TEST; }
            if (IsKeyPressed(KEY_S)) { *current = SCENE_GAME; }
            return true;

        case SCENE_PADDLE_TEST:
            if (IsKeyPressed(KEY_ESCAPE)) { *current = SCENE_INTRO; }
            return true;

        case SCENE_BALL_TEST:
            if (IsKeyPressed(KEY_ESCAPE)) { *current = SCENE_INTRO; }
            return true;

        case SCENE_BLOCK_TEST:
            if (IsKeyPressed(KEY_ESCAPE)) { *current = SCENE_INTRO; }
            return true;

        case SCENE_GAME:
            if (IsKeyPressed(KEY_ESCAPE)) { *current = SCENE_INTRO; }
            return true;

        default:
            return true;
    }
}

void Scene_Draw(SceneId current)
{
    /* First, draw the outer window with space background tiled across entire window */
    for (int y = 0; y < WINDOW_HEIGHT; y += space.height) {
        for (int x = 0; x < WINDOW_WIDTH; x += space.width) {
            DrawTexture(space, x, y, WHITE);
        }
    }
    
    /* Get inner window dimensions */
    int innerX = GetInnerWindowX();
    int innerY = GetInnerWindowY();
    int innerWidth = GetInnerWindowWidth();
    int innerHeight = GetInnerWindowHeight();
    
    /* Draw inner window background */
    DrawRectangle(innerX, innerY, innerWidth, innerHeight, BLACK);
    
    /* Now draw scene content within the inner window */
    if (current == SCENE_INTRO)
    {
        /* Draw tiled background inside inner window - clip to inner bounds */
        for (int y = innerY; y < innerY + innerHeight; y += mnbgrnd.height) {
            for (int x = innerX; x < innerX + innerWidth; x += mnbgrnd.width) {
                /* Only draw if texture fits within inner window */
                int drawWidth = mnbgrnd.width;
                int drawHeight = mnbgrnd.height;
                if (x + drawWidth > innerX + innerWidth) drawWidth = (innerX + innerWidth) - x;
                if (y + drawHeight > innerY + innerHeight) drawHeight = (innerY + innerHeight) - y;

                DrawTextureRec(mnbgrnd, (Rectangle){0, 0, drawWidth, drawHeight}, (Vector2){x, y}, WHITE);
            }
        }


    }
    else if (current == SCENE_HOWTO)
    {
        /* Draw tiled background inside inner window - clip to inner bounds */
        for (int y = innerY; y < innerY + innerHeight; y += bgrnd.height) {
            for (int x = innerX; x < innerX + innerWidth; x += bgrnd.width) {
                /* Only draw if texture fits within inner window */
                int drawWidth = bgrnd.width;
                int drawHeight = bgrnd.height;
                if (x + drawWidth > innerX + innerWidth) drawWidth = (innerX + innerWidth) - x;
                if (y + drawHeight > innerY + innerHeight) drawHeight = (innerY + innerHeight) - y;
                
                DrawTextureRec(bgrnd, (Rectangle){0, 0, drawWidth, drawHeight}, (Vector2){x, y}, WHITE);
            }
        }


    }
    else if (current == SCENE_PADDLE_TEST)
    {
        /* Just plain black inside already drawn inner window */
    }
    else if (current == SCENE_BALL_TEST)
    {
        /* Just plain black inside already drawn inner window */
    }
    else if (current == SCENE_BLOCK_TEST)
    {
        /* Just plain black inside already drawn inner window */
    }
    else if (current == SCENE_GAME)
    {
        /* Just plain black inside already drawn inner window */
    }
    
    /* Draw thick dark red border around inner window - drawn last so it appears on top */
    int borderThickness = 2;
    Color darkRed = {139, 0, 0, 255};  /* Dark red color (RGB: 139, 0, 0) */
    DrawRectangle(innerX, innerY, innerWidth, borderThickness, darkRed);  /* Top */
    DrawRectangle(innerX, innerY + innerHeight - borderThickness, innerWidth, borderThickness, darkRed);  /* Bottom */
    DrawRectangle(innerX, innerY, borderThickness, innerHeight, darkRed);  /* Left */
    DrawRectangle(innerX + innerWidth - borderThickness, innerY, borderThickness, innerHeight, darkRed);  /* Right */

    /* Draw bottom-left outer window text last so it's visible */
    DrawSceneFooter(current);

    /* Draw top-right outer window level counter */
    DrawLevelCounter(current);

    /* Paddle test overlay (draw paddle within inner window) */
    if (current == SCENE_PADDLE_TEST) {
        static Paddle paddle;
        static bool init = false;
        Rectangle inner = { (float)innerX, (float)innerY, (float)innerWidth, (float)innerHeight };
        if (!init) { Paddle_Init(&paddle, inner); init = true; }
        Paddle_Update(&paddle, GetFrameTime(), inner);
        Paddle_Draw(&paddle);
    }

    /* Ball test overlay (update/draw ball bouncing in inner window) */
    if (current == SCENE_BALL_TEST) {
        static Ball ball;
        static bool initBall = false;
        Rectangle inner = { (float)innerX, (float)innerY, (float)innerWidth, (float)innerHeight };
        if (!initBall) { Ball_Init(&ball, inner); initBall = true; }
        Ball_Update(&ball, GetFrameTime(), inner);
        Ball_Draw(&ball);
    }

    /* Block test overlay (load level and render blocks) */
    if (current == SCENE_BLOCK_TEST) {
        static Level level;
        static BlockGrid grid;
        static bool initBlocks = false;
        Rectangle inner = { (float)innerX, (float)innerY, (float)innerWidth, (float)innerHeight };
        if (!initBlocks) {
            if (Level_Load("resource/levels/level01.data", &level)) {
                BlockGrid_Load(&grid, &level, inner);
                initBlocks = true;
            }
        }
        if (initBlocks) {
            BlockGrid_Draw(&grid, inner);
        }
    }

    /* GAME scene: delegated to game scene module */
    if (current == SCENE_GAME) {
        Rectangle inner = { (float)innerX, (float)innerY, (float)innerWidth, (float)innerHeight };
        GameScene_Init(inner);
        GameScene_Update(GetFrameTime(), inner);
        GameScene_Draw(inner);
    }
}
