#ifndef XBOING_SCENE_HOWTO_H
#define XBOING_SCENE_HOWTO_H

#include <raylib.h>

void HowToScene_Init(Rectangle bounds);
void HowToScene_Update(float dt, Rectangle bounds);
void HowToScene_Draw(Rectangle bounds);
void HowToScene_Cleanup(void);

#endif /* XBOING_SCENE_HOWTO_H */
