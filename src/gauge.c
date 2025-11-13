#include "../include/gauge.h"
#include "../include/assets.h"
#include <math.h>

/* Constants */
#define GAUGE_FRAME_RATE 8.0f  /* frames per second */
#define GAUGE_MIN_FRAME 1
#define GAUGE_MAX_FRAME 11
#define GAUGE_START_FRAME 11   /* start at right */
#define GAUGE_START_DIR -1     /* start moving left */

#define GAUGE_GAP_ABOVE_PADDLE 20.0f
#define DEBUG_LINE_LENGTH 80.0f

#define ANGLE_MIN (3.14159265f / 4.0f)        /* 45° left-upward */
#define ANGLE_MAX (3.0f * 3.14159265f / 4.0f) /* 135° right-upward */

/* Helper: get guide texture by frame index */
static Texture2D GetGuideTextureByIndex(int idx)
{
    switch (idx) {
        case 1: return guide1;
        case 2: return guide2;
        case 3: return guide3;
        case 4: return guide4;
        case 5: return guide5;
        case 6: return guide6;
        case 7: return guide7;
        case 8: return guide8;
        case 9: return guide9;
        case 10: return guide10;
        case 11: return guide11;
        default: return guide; /* fallback */
    }
}

void Gauge_Init(Gauge* gauge)
{
    gauge->frame = GAUGE_START_FRAME;
    gauge->direction = GAUGE_START_DIR;
    gauge->timer = 0.0f;
    gauge->showDebugLine = false;
}

void Gauge_Update(Gauge* gauge, float dt)
{
    gauge->timer += dt;
    const float frameTime = 1.0f / GAUGE_FRAME_RATE;
    
    while (gauge->timer >= frameTime) {
        gauge->timer -= frameTime;
        gauge->frame += gauge->direction;
        
        /* Reverse direction at boundaries */
        if (gauge->frame > GAUGE_MAX_FRAME) {
            gauge->frame = GAUGE_MAX_FRAME - 1;
            gauge->direction = -1;
        } else if (gauge->frame < GAUGE_MIN_FRAME) {
            gauge->frame = GAUGE_MIN_FRAME + 1;
            gauge->direction = 1;
        }
    }
}

void Gauge_Reset(Gauge* gauge)
{
    gauge->frame = GAUGE_START_FRAME;
    gauge->direction = GAUGE_START_DIR;
    gauge->timer = 0.0f;
}

void Gauge_ToggleDebug(Gauge* gauge)
{
    gauge->showDebugLine = !gauge->showDebugLine;
}

float Gauge_GetLaunchAngle(const Gauge* gauge)
{
    /* Map gauge frame (1..11) to launch angle: frame 11 = left (45°), frame 1 = right (135°) */
    float t = (float)(GAUGE_MAX_FRAME - gauge->frame) / (float)(GAUGE_MAX_FRAME - GAUGE_MIN_FRAME);
    return ANGLE_MIN + t * (ANGLE_MAX - ANGLE_MIN);
}

void Gauge_Draw(const Gauge* gauge, Rectangle paddleRect)
{
    Texture2D tex = GetGuideTextureByIndex(gauge->frame);
    float padCenterX = paddleRect.x + paddleRect.width * 0.5f;
    float padTopY = paddleRect.y;
    
    int drawX = (int)(padCenterX - tex.width * 0.5f);
    int drawY = (int)(padTopY - GAUGE_GAP_ABOVE_PADDLE - tex.height);
    
    DrawTexture(tex, drawX, drawY, Fade(WHITE, 0.85f));
}

void Gauge_DrawDebugLine(const Gauge* gauge, Rectangle ballRect)
{
    if (!gauge->showDebugLine) return;
    
    float ballCenterX = ballRect.x + ballRect.width * 0.5f;
    float ballCenterY = ballRect.y + ballRect.height * 0.5f;
    
    float releaseAngle = Gauge_GetLaunchAngle(gauge);
    
    Vector2 startPoint = { ballCenterX, ballCenterY };
    Vector2 endPoint = {
        ballCenterX + cosf(releaseAngle) * DEBUG_LINE_LENGTH,
        ballCenterY - sinf(releaseAngle) * DEBUG_LINE_LENGTH /* negate Y for screen coords */
    };
    
    DrawLineEx(startPoint, endPoint, 2.0f, YELLOW);
}
