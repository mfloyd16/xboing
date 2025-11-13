#include "../include/scene.h"
#include <raylib.h>
#include "../include/assets.h"
#include "../include/config.h"
#include "../include/level.h"
#include "../include/scenes/game.h"
#include "../include/scenes/intro.h"
#include "../include/scenes/howto.h"
#include "../include/scenes/paddle_test.h"
#include "../include/scenes/ball_test.h"
#include "../include/scenes/block_test.h"

/* Inner gameplay window constants */
static const int INNER_GAP_TOP = 60;
static const int INNER_GAP_BOTTOM = 75;
static const int INNER_GAP_LEFT = 35;
static const int INNER_GAP_RIGHT = 40;

/* Short loading delay between scenes to ignore previous inputs */
static const float SCENE_TRANSITION_DELAY = 0.30f; /* seconds */
static const float SCENE_FADE_HALFWAY = 0.15f; /* half the transition for fade-out, half for fade-in */
static bool gTransitionActive = false;
static float gTransitionTimer = 0.0f;
static SceneId gTargetScene = SCENE_INTRO;

/* Helper functions for inner gameplay window */
static int GetInnerWindowX(void) { return INNER_GAP_LEFT; }
static int GetInnerWindowY(void) { return INNER_GAP_TOP; }
static int GetInnerWindowWidth(void) { return WINDOW_WIDTH - INNER_GAP_LEFT - INNER_GAP_RIGHT; }
static int GetInnerWindowHeight(void) { return WINDOW_HEIGHT - INNER_GAP_TOP - INNER_GAP_BOTTOM; }

/* Cleanup a scene when leaving it */
static void CleanupSceneById(SceneId id)
{
    switch (id) {
        case SCENE_INTRO:       IntroScene_Cleanup(); break;
        case SCENE_HOWTO:       HowToScene_Cleanup(); break;
        case SCENE_PADDLE_TEST: PaddleTestScene_Cleanup(); break;
        case SCENE_BALL_TEST:   BallTestScene_Cleanup(); break;
        case SCENE_BLOCK_TEST:  BlockTestScene_Cleanup(); break;
        case SCENE_GAME:        GameScene_Cleanup(); break;
        default: break;
    }
}

/* Drain any queued key/char presses to avoid leaking into next scene */
static void FlushInputQueues(void)
{
    while (GetKeyPressed() != 0) { /* drain */ }
    while (GetCharPressed() != 0) { /* drain */ }
}

/* Begin a timed transition to the next scene */
static void StartSceneTransition(SceneId from, SceneId to)
{
    gTransitionActive = true;
    gTransitionTimer = SCENE_TRANSITION_DELAY;
    gTargetScene = to;
    CleanupSceneById(from);
    FlushInputQueues();
}

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
}

void Scene_Cleanup(void)
{
    Assets_Cleanup();
}

bool Scene_Update(SceneId* current)
{
    SceneId old = *current;
    bool running = true;

    /* If in transition, count down and finalize change when timer elapses */
    if (gTransitionActive) {
        gTransitionTimer -= GetFrameTime();
        if (gTransitionTimer <= 0.0f) {
            *current = gTargetScene;
            gTransitionActive = false;
            /* Extra safety: ensure no stale presses carry over */
            FlushInputQueues();
        }
        return running; /* Skip input while transitioning */
    }

    switch (*current)
    {
        case SCENE_INTRO:
            /* Intro controls: H = How To, SPACE = Start Game */
            if (IsKeyPressed(KEY_H)) { *current = SCENE_HOWTO; }
            if (IsKeyPressed(KEY_SPACE)) { StartSceneTransition(*current, SCENE_GAME); }
            /* Dev/test shortcuts remain */
            if (IsKeyPressed(KEY_G)) { StartSceneTransition(*current, SCENE_PADDLE_TEST); }
            if (IsKeyPressed(KEY_B)) { StartSceneTransition(*current, SCENE_BALL_TEST); }
            if (IsKeyPressed(KEY_L)) { StartSceneTransition(*current, SCENE_BLOCK_TEST); }
            break;

        case SCENE_HOWTO:
            if (IsKeyPressed(KEY_B)) { *current = SCENE_INTRO; }
            break;

        case SCENE_PADDLE_TEST:
            if (IsKeyPressed(KEY_ESCAPE)) { StartSceneTransition(*current, SCENE_INTRO); }
            break;

        case SCENE_BALL_TEST:
            if (IsKeyPressed(KEY_ESCAPE)) { StartSceneTransition(*current, SCENE_INTRO); }
            break;

        case SCENE_BLOCK_TEST:
            if (IsKeyPressed(KEY_ESCAPE)) { StartSceneTransition(*current, SCENE_INTRO); }
            break;

        case SCENE_GAME:
            if (IsKeyPressed(KEY_ESCAPE)) { StartSceneTransition(*current, SCENE_INTRO); }
            break;

        default:
            break;
    }

    return running;
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
    Rectangle inner = { (float)innerX, (float)innerY, (float)innerWidth, (float)innerHeight };
    if (gTransitionActive)
    {
        /* Fade effect: first half fades out (black overlay grows), second half fades in (overlay shrinks) */
        float fadeProgress = 1.0f - (gTransitionTimer / SCENE_TRANSITION_DELAY);
        int alpha;
        if (fadeProgress < 0.5f) {
            /* First half: fade OUT (0 -> 255) */
            alpha = (int)(fadeProgress * 2.0f * 255.0f);
        } else {
            /* Second half: fade IN (255 -> 0) */
            alpha = (int)((1.0f - fadeProgress) * 2.0f * 255.0f);
        }
        if (alpha < 0) alpha = 0;
        if (alpha > 255) alpha = 255;
        
        /* Draw black overlay with calculated alpha over the inner window */
        DrawRectangle(innerX, innerY, innerWidth, innerHeight, (Color){0, 0, 0, (unsigned char)alpha});
    }
    else if (current == SCENE_INTRO)
    {
        IntroScene_Init(inner);
        IntroScene_Update(GetFrameTime(), inner);
        IntroScene_Draw(inner);
    }
    else if (current == SCENE_HOWTO)
    {
        HowToScene_Init(inner);
        HowToScene_Update(GetFrameTime(), inner);
        HowToScene_Draw(inner);
    }
    else if (current == SCENE_PADDLE_TEST)
    {
        PaddleTestScene_Init(inner);
        PaddleTestScene_Update(GetFrameTime(), inner);
        PaddleTestScene_Draw(inner);
    }
    else if (current == SCENE_BALL_TEST)
    {
        BallTestScene_Init(inner);
        BallTestScene_Update(GetFrameTime(), inner);
        BallTestScene_Draw(inner);
    }
    else if (current == SCENE_BLOCK_TEST)
    {
        BlockTestScene_Init(inner);
        BlockTestScene_Update(GetFrameTime(), inner);
        BlockTestScene_Draw(inner);
    }
    else if (current == SCENE_GAME)
    {
        GameScene_Init(inner);
        GameScene_Update(GetFrameTime(), inner);
        GameScene_Draw(inner);
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

    /* Test scenes delegated to their modules above. */

    /* Scene drawing complete */
}
