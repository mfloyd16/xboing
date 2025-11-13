#include "../../include/scenes/howto.h"
#include "../../include/assets.h"

static int tileWidth(void) { return bgrnd.width; }
static int tileHeight(void) { return bgrnd.height; }

void HowToScene_Init(Rectangle bounds) { (void)bounds; }
void HowToScene_Update(float dt, Rectangle bounds) { (void)dt; (void)bounds; }

void HowToScene_Draw(Rectangle bounds)
{
    for (int y = (int)bounds.y; y < (int)(bounds.y + bounds.height); y += tileHeight()) {
        for (int x = (int)bounds.x; x < (int)(bounds.x + bounds.width); x += tileWidth()) {
            int drawWidth = tileWidth();
            int drawHeight = tileHeight();
            if (x + drawWidth > bounds.x + bounds.width) drawWidth = (int)((bounds.x + bounds.width) - x);
            if (y + drawHeight > bounds.y + bounds.height) drawHeight = (int)((bounds.y + bounds.height) - y);
            DrawTextureRec(bgrnd, (Rectangle){0,0,(float)drawWidth,(float)drawHeight}, (Vector2){(float)x,(float)y}, WHITE);
        }
    }
}

void HowToScene_Cleanup(void) {}
