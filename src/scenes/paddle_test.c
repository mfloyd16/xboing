#include "../../include/scenes/paddle_test.h"
#include "../../include/paddle.h"

static int gInit = 0;
static Paddle gPaddle;

void PaddleTestScene_Init(Rectangle bounds)
{
    if (!gInit) {
        Paddle_Init(&gPaddle, bounds);
        gInit = 1;
    }
}

void PaddleTestScene_Update(float dt, Rectangle bounds)
{
    if (!gInit) return;
    Paddle_Update(&gPaddle, dt, bounds);
}

void PaddleTestScene_Draw(Rectangle bounds)
{
    (void)bounds;
    if (!gInit) return;
    Paddle_Draw(&gPaddle);
}

void PaddleTestScene_Cleanup(void)
{
    gInit = 0;
}
