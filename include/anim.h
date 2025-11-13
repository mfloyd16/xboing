#ifndef XBOING_ANIM_H
#define XBOING_ANIM_H

#include <raylib.h>

/* Public animation identifiers for preset-driven animations */
typedef enum AnimId {
    ANIM_STAR = 0,        /* 11 looping star frames */
    ANIM_BALL_BIRTH,      /* 8 non-looping birth frames -> clamps at last */
    ANIM_BALL_CYCLE,      /* 4 looping ball variations */
    ANIM_BONUS_MULTI,     /* bonus[0..3] */
    ANIM_X2BONUS,         /* x2bonus[0..3] */
    ANIM_X4BONUS,         /* x4bonus[0..3] */
    ANIM_COUNT
} AnimId;

/* Initialize presets (requires assets already loaded). */
void Anim_Init(void);

/* Advance animation time (call once per frame). */
void Anim_Update(float dt);

/* Get current frame texture for a preset.
 * phaseOffset lets you desync multiple instances (e.g., 0.0, 0.3, 0.6).
 */
Texture2D Anim_GetFrame(AnimId id, float phaseOffset);

#endif /* XBOING_ANIM_H */
