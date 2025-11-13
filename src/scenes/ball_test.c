#include "../../include/scenes/ball_test.h"
#include "../../include/ball.h"

static int gInit = 0;
static Ball gBall;

void BallTestScene_Init(Rectangle bounds)
{
    if (!gInit) {
        Ball_Init(&gBall, bounds);
        gInit = 1;
    }
}

void BallTestScene_Update(float dt, Rectangle bounds)
{
    if (!gInit) return;
    Ball_Update(&gBall, dt, bounds);
}

void BallTestScene_Draw(Rectangle bounds)
{
    (void)bounds;
    if (!gInit) return;
    Ball_Draw(&gBall);
}

void BallTestScene_Cleanup(void)
{
    gInit = 0;
}
