#include <raylib.h>
#include <stdio.h>

/* Include the scene API implemented in src/scene.c. Use a relative include
   path because this project compiles files from the src/ directory. */
#include "../include/scene.h"
#include "../include/config.h"
#include "../include/level.h"

/* Global window dimensions - change these to resize the entire game */
const int WINDOW_WIDTH = 575;
const int WINDOW_HEIGHT = 720;

int main(void)
{
	/* Disable raylib tracing logs (suppress debug/info output). */
	SetTraceLogLevel(LOG_NONE);

	/* Initialize window/audio and target FPS. */
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Xboing");
	InitAudioDevice();
	SetTargetFPS(60);

	/* Initialize scene resources (load textures) */
	Scene_Init();

	SceneId current = SCENE_INTRO;

	/* Main loop: Scene_Update returns false to request exit. */
	while (!WindowShouldClose() && Scene_Update(&current))
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);

		Scene_Draw(current);

		EndDrawing();
	}

	/* Clean up scene resources and close. */
	Scene_Cleanup();
	CloseWindow();
	return 0;
}
