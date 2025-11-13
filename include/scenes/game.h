#ifndef XBOING_SCENE_GAME_H
#define XBOING_SCENE_GAME_H

#include <raylib.h>

/* Initialize game scene with level and game objects */
void GameScene_Init(Rectangle bounds);

/* Update game logic: paddle, ball, collisions */
void GameScene_Update(float dt, Rectangle bounds);

/* Draw all game objects: blocks, paddle, ball */
void GameScene_Draw(Rectangle bounds);

/* Cleanup and reset game scene state */
void GameScene_Cleanup(void);

#endif /* XBOING_SCENE_GAME_H */
