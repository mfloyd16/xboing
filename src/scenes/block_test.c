#include "../../include/scenes/block_test.h"
#include "../../include/block.h"
#include "../../include/level.h"

static int gInit = 0;
static Level gLevel;
static BlockGrid gGrid;

void BlockTestScene_Init(Rectangle bounds)
{
    if (!gInit) {
        if (Level_Load("resource/levels/level01.data", &gLevel)) {
            BlockGrid_Load(&gGrid, &gLevel, bounds);
            gInit = 1;
        }
    }
}

void BlockTestScene_Update(float dt, Rectangle bounds)
{
    (void)dt; (void)bounds;
}

void BlockTestScene_Draw(Rectangle bounds)
{
    (void)bounds;
    if (!gInit) return;
    BlockGrid_Draw(&gGrid, bounds);
}

void BlockTestScene_Cleanup(void)
{
    if (gInit) {
        BlockGrid_Free(&gGrid);
        Level_Free(&gLevel);
        gInit = 0;
    }
}
