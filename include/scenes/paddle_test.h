#ifndef XBOING_SCENE_PADDLE_TEST_H
#define XBOING_SCENE_PADDLE_TEST_H

#include <raylib.h>

void PaddleTestScene_Init(Rectangle bounds);
void PaddleTestScene_Update(float dt, Rectangle bounds);
void PaddleTestScene_Draw(Rectangle bounds);
void PaddleTestScene_Cleanup(void);

#endif /* XBOING_SCENE_PADDLE_TEST_H */
