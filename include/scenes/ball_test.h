#ifndef XBOING_SCENE_BALL_TEST_H
#define XBOING_SCENE_BALL_TEST_H

#include <raylib.h>

void BallTestScene_Init(Rectangle bounds);
void BallTestScene_Update(float dt, Rectangle bounds);
void BallTestScene_Draw(Rectangle bounds);
void BallTestScene_Cleanup(void);

#endif /* XBOING_SCENE_BALL_TEST_H */
