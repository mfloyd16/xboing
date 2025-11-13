#include "../include/scene.h"
#include <raylib.h>
#include "../include/assets.h"
#include "../include/anim.h"
#include "../include/config.h"
#include "../include/level.h"

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
            return true;

        case SCENE_HOWTO:
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) ||
                IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ESCAPE))
            {
                return false; /* request exit */
            }
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
    
    /* Draw thick dark red border around inner window - drawn last so it appears on top */
    int borderThickness = 2;
    Color darkRed = {139, 0, 0, 255};  /* Dark red color (RGB: 139, 0, 0) */
    DrawRectangle(innerX, innerY, innerWidth, borderThickness, darkRed);  /* Top */
    DrawRectangle(innerX, innerY + innerHeight - borderThickness, innerWidth, borderThickness, darkRed);  /* Bottom */
    DrawRectangle(innerX, innerY, borderThickness, innerHeight, darkRed);  /* Left */
    DrawRectangle(innerX + innerWidth - borderThickness, innerY, borderThickness, innerHeight, darkRed);  /* Right */

    /* Draw bottom-left outer window text last so it's visible */
    DrawSceneFooter(current);
}
