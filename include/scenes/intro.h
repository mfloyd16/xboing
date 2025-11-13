#ifndef XBOING_SCENE_INTRO_H
#define XBOING_SCENE_INTRO_H

#include <raylib.h>

void IntroScene_Init(Rectangle bounds);
void IntroScene_Update(float dt, Rectangle bounds);
void IntroScene_Draw(Rectangle bounds);
void IntroScene_Cleanup(void);

#endif /* XBOING_SCENE_INTRO_H */
