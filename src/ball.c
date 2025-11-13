#include "../include/ball.h"
#include "../include/assets.h"
#include <math.h>

static float clampf(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }

void Ball_Init(Ball* b, Rectangle bounds)
{
    if (!b) return;
    /* Start near center with a 45-degree velocity */
    int bw = ball1.width;
    int bh = ball1.height;
    b->speed = 260.0f;
    b->pos.x = bounds.x + (bounds.width - bw) * 0.5f;
    b->pos.y = bounds.y + (bounds.height - bh) * 0.5f;
    float d = b->speed * sqrtf(0.5f); /* ~speed / sqrt(2) */
    b->vel = (Vector2){ d, -d };
    b->frame = 0;
    b->frameTime = 0.0f;
}

void Ball_Update(Ball* b, float dt, Rectangle bounds)
{
    if (!b) return;

    /* Integrate */
    b->pos.x += b->vel.x * dt;
    b->pos.y += b->vel.y * dt;

    float left = bounds.x;
    float top = bounds.y;
    float right = bounds.x + bounds.width;
    float bottom = bounds.y + bounds.height;

    int bw = ball1.width;
    int bh = ball1.height;

    /* Bounce on X walls */
    if (b->pos.x <= left) {
        b->pos.x = left;
        b->vel.x = fabsf(b->vel.x);
    } else if (b->pos.x + bw >= right) {
        b->pos.x = right - bw;
        b->vel.x = -fabsf(b->vel.x);
    }

    /* Bounce on Y walls */
    if (b->pos.y <= top) {
        b->pos.y = top;
        b->vel.y = fabsf(b->vel.y);
    } else if (b->pos.y + bh >= bottom) {
        b->pos.y = bottom - bh;
        b->vel.y = -fabsf(b->vel.y);
    }

    /* Animate frames: cycle through 4 frames at ~12 FPS */
    const float frameDelay = 1.0f / 12.0f;
    b->frameTime += dt;
    if (b->frameTime >= frameDelay) {
        b->frameTime -= frameDelay;
        b->frame = (b->frame + 1) % 4;
    }
}

void Ball_Draw(const Ball* b)
{
    if (!b) return;
    /* Select texture based on current frame */
    Texture2D tex;
    switch (b->frame) {
        case 0: tex = ball1; break;
        case 1: tex = ball2; break;
        case 2: tex = ball3; break;
        case 3: tex = ball4; break;
        default: tex = ball1; break;
    }
    DrawTexture(tex, (int)lroundf(b->pos.x), (int)lroundf(b->pos.y), WHITE);
}

Rectangle Ball_GetRect(const Ball* b)
{
    Rectangle r = {0};
    if (!b) return r;
    r.x = b->pos.x;
    r.y = b->pos.y;
    r.width = (float)ball1.width;
    r.height = (float)ball1.height;
    return r;
}
