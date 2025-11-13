#ifndef XBOING_PADDLE_H
#define XBOING_PADDLE_H

#include <raylib.h>

typedef struct Paddle {
    Vector2 pos;    /* Top-left position in window coordinates */
    float speed;    /* Pixels per second */
} Paddle;

/* Initialize paddle at a sensible starting location within bounds. */
void Paddle_Init(Paddle* p, Rectangle bounds);

/* Update paddle position from input (keyboard/mouse), constrained to bounds. */
void Paddle_Update(Paddle* p, float dt, Rectangle bounds);

/* Draw the paddle using the loaded texture. */
void Paddle_Draw(const Paddle* p);

/* Get paddle bounding box for collisions, based on its texture size. */
Rectangle Paddle_GetRect(const Paddle* p);

#endif /* XBOING_PADDLE_H */