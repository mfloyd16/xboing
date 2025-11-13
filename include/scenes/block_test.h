#ifndef XBOING_SCENE_BLOCK_TEST_H
#define XBOING_SCENE_BLOCK_TEST_H

#include <raylib.h>

void BlockTestScene_Init(Rectangle bounds);
void BlockTestScene_Update(float dt, Rectangle bounds);
void BlockTestScene_Draw(Rectangle bounds);
void BlockTestScene_Cleanup(void);

#endif /* XBOING_SCENE_BLOCK_TEST_H */
