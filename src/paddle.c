#include "../include/paddle.h"
#include "../include/assets.h"
#include <math.h>

void Paddle_Init(Paddle* p, Rectangle bounds)
{
    if (!p) return;
    p->speed = 360.0f;
    /* Center horizontally, near bottom inside bounds */
    int pw = padmed.width;
    int ph = padmed.height;
    p->pos.x = bounds.x + (bounds.width - pw) * 0.5f;
    p->pos.y = bounds.y + bounds.height - ph - 10.0f;
}

static float clampf(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }

void Paddle_Update(Paddle* p, float dt, Rectangle bounds)
{
    if (!p) return;

    float vx = 0.0f;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))  vx -= 1.0f;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) vx += 1.0f;

    /* Keyboard move */
    if (vx != 0.0f) {
        p->pos.x += vx * p->speed * dt;
    }

    /* Mouse follow (optional): when left button pressed, track X */
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        float targetX = GetMouseX() - padmed.width * 0.5f;
        p->pos.x = targetX;
    }

    /* Clamp within bounds */
    float minX = bounds.x;
    float maxX = bounds.x + bounds.width - padmed.width;
    p->pos.x = clampf(p->pos.x, minX, maxX);
}

void Paddle_Draw(const Paddle* p)
{
    if (!p) return;
    DrawTexture(padmed, (int)lroundf(p->pos.x), (int)lroundf(p->pos.y), WHITE);
}

Rectangle Paddle_GetRect(const Paddle* p)
{
    Rectangle r = {0};
    if (!p) return r;
    r.x = p->pos.x;
    r.y = p->pos.y;
    r.width = (float)padmed.width;
    r.height = (float)padmed.height;
    return r;
}
