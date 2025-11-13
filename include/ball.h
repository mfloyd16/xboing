#ifndef XBOING_BALL_H
#define XBOING_BALL_H

#include <raylib.h>

typedef struct Ball {
    Vector2 pos;     /* Top-left position in window coordinates */
    Vector2 vel;     /* Pixels per second */
    float speed;     /* Scalar speed for convenience */
    int frame;       /* Current animation frame (0-3) */
    float frameTime; /* Time accumulator for frame changes */
} Ball;

void Ball_Init(Ball* b, Rectangle bounds);
void Ball_Update(Ball* b, float dt, Rectangle bounds);
void Ball_Draw(const Ball* b);
Rectangle Ball_GetRect(const Ball* b);

#endif /* XBOING_BALL_H */
