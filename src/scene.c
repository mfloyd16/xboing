#include "../include/scene.h"
#include <raylib.h>

bool Scene_Update(SceneId* current)
{
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
    if (current == SCENE_INTRO)
    {
        DrawText("Xboing", 20, 20, 40, DARKGRAY);
        DrawText("Intro Scene", 320, 220, 30, BLACK);
        DrawText("Press Enter / Space / Click to continue", 180, 300, 20, GRAY);
    }
    else if (current == SCENE_HOWTO)
    {
        DrawText("How To Play", 300, 20, 40, DARKGRAY);
        DrawText("- Move the paddle to bounce the ball", 120, 160, 20, BLACK);
        DrawText("- Hit blocks to score points", 120, 200, 20, BLACK);
        DrawText("Press Enter / Space / Click to exit", 200, 340, 20, GRAY);
    }
}
