#include "../include/anim.h"
#include "../include/assets.h"

typedef struct AnimPreset {
    const Texture2D* frames; /* pointer to first frame */
    int frameCount;          /* number of frames */
    float fps;               /* frames per second */
    bool loop;               /* true: wrap, false: clamp at last */
} AnimPreset;

static AnimPreset gAnimPresets[ANIM_COUNT];
static float s_animTime = 0.0f;

static Texture2D* BuildBallCycleArray(void)
{
    static Texture2D arr[4];
    arr[0] = ball1; arr[1] = ball2; arr[2] = ball3; arr[3] = ball4;
    return arr;
}

void Anim_Init(void)
{
    gAnimPresets[ANIM_STAR]       = (AnimPreset){ stars,    11, 10.0f, true  };
    gAnimPresets[ANIM_BALL_BIRTH] = (AnimPreset){ bbirth,    8,  8.0f, false };
    gAnimPresets[ANIM_BALL_CYCLE] = (AnimPreset){ BuildBallCycleArray(), 4, 4.0f, true };
    gAnimPresets[ANIM_BONUS_MULTI]= (AnimPreset){ bonus,     4,  6.0f, true  };
    gAnimPresets[ANIM_X2BONUS]    = (AnimPreset){ x2bonus,   4,  6.0f, true  };
    gAnimPresets[ANIM_X4BONUS]    = (AnimPreset){ x4bonus,   4,  6.0f, true  };
}

void Anim_Update(float dt)
{
    s_animTime += dt;
}

Texture2D Anim_GetFrame(AnimId id, float phaseOffset)
{
    const AnimPreset* p = &gAnimPresets[id];
    float t = s_animTime + phaseOffset;
    int frame = (int)(t * p->fps);
    if (p->loop)
    {
        frame %= p->frameCount;
    }
    else if (frame >= p->frameCount)
    {
        frame = p->frameCount - 1; /* clamp */
    }
    return p->frames[frame];
}
