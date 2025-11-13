#include "../../include/scenes/intro.h"
#include "../../include/assets.h"

static int tileWidth(void) { return mnbgrnd.width; }
static int tileHeight(void) { return mnbgrnd.height; }

void IntroScene_Init(Rectangle bounds) { (void)bounds; }
void IntroScene_Update(float dt, Rectangle bounds) { (void)dt; (void)bounds; }

void IntroScene_Draw(Rectangle bounds)
{
    for (int y = (int)bounds.y; y < (int)(bounds.y + bounds.height); y += tileHeight()) {
        for (int x = (int)bounds.x; x < (int)(bounds.x + bounds.width); x += tileWidth()) {
            int drawWidth = tileWidth();
            int drawHeight = tileHeight();
            if (x + drawWidth > bounds.x + bounds.width) drawWidth = (int)((bounds.x + bounds.width) - x);
            if (y + drawHeight > bounds.y + bounds.height) drawHeight = (int)((bounds.y + bounds.height) - y);
            DrawTextureRec(mnbgrnd, (Rectangle){0,0,(float)drawWidth,(float)drawHeight}, (Vector2){(float)x,(float)y}, WHITE);
        }
    }
}

void IntroScene_Cleanup(void) {}
