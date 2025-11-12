#include <raylib.h>
#include <stdio.h>

/* Include the scene API implemented in src/scene.c. Use a relative include
   path because this project compiles files from the src/ directory. */
#include "../include/scene.h"

int main(void)
{
	/* Disable raylib tracing logs (suppress debug/info output). */
	SetTraceLogLevel(LOG_NONE);

	/* Initialize window/audio and target FPS. */
	InitWindow(800, 600, "Xboing");
	InitAudioDevice();
	SetTargetFPS(60);

	SceneId current = SCENE_INTRO;

	/* Main loop: Scene_Update returns false to request exit. */
	while (!WindowShouldClose() && Scene_Update(&current))
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);

		Scene_Draw(current);

		EndDrawing();
	}

	/* Clean up and close. */
	CloseWindow();
	return 0;
}
