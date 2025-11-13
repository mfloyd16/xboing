#include "../../include/scenes/game.h"
#include "../../include/paddle.h"
#include "../../include/ball.h"
#include "../../include/block.h"
#include "../../include/level.h"
#include "../../include/assets.h"
#include <math.h>
#include <stdbool.h>

/* Game scene state */
static bool gInitialized = false;
static bool gBallLaunched = false;
static Level gLevel;
static BlockGrid gGrid;
static Paddle gPaddle;
static Ball gBall;

/* Gauge animation state */
static int gGuideFrame = 11;     /* 11..1 (guide11..guide1) - start at right */
static int gGuideDir = -1;       /* -1 backward (11->1), 1 forward (1->11) */
static float gGuideTimer = 0.0f; /* seconds accumulator */

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

/* Helper: compute block rectangle in window coordinates */
static Rectangle ComputeBlockRect(const Block* blk, Rectangle bounds)
{
    Texture2D tex = Block_GetTexture(blk->type);
    Rectangle r;
    r.x = bounds.x + (float)(blk->col * gGrid.cellWidth + blk->offsetX);
    r.y = bounds.y + (float)(blk->row * gGrid.cellHeight + blk->offsetY);
    r.width = (float)tex.width;
    r.height = (float)tex.height;
    return r;
}

void GameScene_Init(Rectangle bounds)
{
    if (!gInitialized) {
        if (Level_Load("resource/levels/level01.data", &gLevel)) {
            Level_SetActive(&gLevel);
            Level_SetActiveNumber(1);
            BlockGrid_Load(&gGrid, &gLevel, bounds);
            Paddle_Init(&gPaddle, bounds);
            Ball_Init(&gBall, bounds);
            gBallLaunched = false;
            gInitialized = true;
        }
    }
}

void GameScene_Update(float dt, Rectangle bounds)
{
    if (!gInitialized) return;

    /* Update paddle */
    Paddle_Update(&gPaddle, dt, bounds);

    /* If ball not launched, stick it to paddle and wait for launch input */
    if (!gBallLaunched) {
        Rectangle rPad = Paddle_GetRect(&gPaddle);
        /* Center ball above paddle */
        gBall.pos.x = rPad.x + (rPad.width - Ball_GetRect(&gBall).width) * 0.5f;
        gBall.pos.y = rPad.y - Ball_GetRect(&gBall).height - 2.0f;

        /* Update gauge animation (ping-pong 11..1..11) - slower sweep at 8 FPS for frame changes */
        gGuideTimer += dt;
        const float frameTime = 1.0f / 8.0f;  /* change frames slower */
        while (gGuideTimer >= frameTime) {
            gGuideTimer -= frameTime;
            gGuideFrame += gGuideDir;
            if (gGuideFrame > 11) { gGuideFrame = 10; gGuideDir = -1; }
            else if (gGuideFrame < 1) { gGuideFrame = 2; gGuideDir = 1; }
        }

        /* Launch on space or left click */
        if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            gBallLaunched = true;
            /* Map gauge frame (1..11) to launch angle: frame 11 = left (45°), frame 6 = up (90°), frame 1 = right (135°) */
            /* Angle range: 45° to 135° (π/4 to 3π/4) - reversed mapping */
            const float minAngle = 3.14159265f / 4.0f;      /* 45° (left-upward) */
            const float maxAngle = 3.0f * 3.14159265f / 4.0f; /* 135° (right-upward) */
            float t = (float)(11 - gGuideFrame) / 10.0f;    /* normalize frame 11..1 to 0..1 (reversed) */
            float releaseAngle = minAngle + t * (maxAngle - minAngle);
            
            float speed = gBall.speed;
            gBall.vel.x = cosf(releaseAngle) * speed;
            gBall.vel.y = -sinf(releaseAngle) * speed; /* negate Y for screen coords (up is negative) */
        }
    } else {
        /* Ball is launched, update normally */
        Ball_Update(&gBall, dt, bounds);
        /* Reset gauge animation so it starts fresh next time */
        gGuideTimer = 0.0f;
        gGuideFrame = 11;
        gGuideDir = -1;
    }

    /* Collisions: ball vs paddle (only when launched) */
    if (gBallLaunched) {
        Rectangle rBall = Ball_GetRect(&gBall);
        Rectangle rPad = Paddle_GetRect(&gPaddle);
        bool ballDownward = gBall.vel.y > 0.0f;
        if (CheckCollisionRecs(rBall, rPad) && ballDownward) {
            /* Position ball above paddle */
            gBall.pos.y = rPad.y - rBall.height;

            /* Compute bounce angle from hit position with a max of 60 degrees from vertical */
            float padCenter = rPad.x + rPad.width * 0.5f;
            float ballCenter = rBall.x + rBall.width * 0.5f;
            float t = (ballCenter - padCenter) / (rPad.width * 0.5f); /* [-1, 1] */
            if (t < -1.0f) t = -1.0f; if (t > 1.0f) t = 1.0f;

            float speed = gBall.speed; /* enforce constant scalar speed */
            const float maxAngleSin = 0.8660254f; /* sin(60 deg) */
            gBall.vel.x = t * (speed * maxAngleSin);
            /* Upward Y so negative; guarantees |vy| >= speed*cos(60)=0.5*speed */
            float vy = -sqrtf(fmaxf(0.0f, speed * speed - gBall.vel.x * gBall.vel.x));
            /* Safety: ensure a minimum vertical component in case of fp errors */
            if (fabsf(vy) < speed * 0.4f) {
                float sign = -1.0f; /* always upwards after paddle */
                vy = sign * speed * 0.4f;
                /* Recompute vx to preserve speed */
                float vxMag = sqrtf(fmaxf(0.0f, speed * speed - vy * vy));
                gBall.vel.x = (gBall.vel.x >= 0.0f ? vxMag : -vxMag);
            }
            gBall.vel.y = vy;
        }

        /* Collisions: ball vs blocks (first hit per frame) */
        rBall = Ball_GetRect(&gBall);
        for (int i = 0; i < gGrid.count; i++) {
            Block* blk = &gGrid.blocks[i];
            if (!blk->active) continue;
            Rectangle rBlk = ComputeBlockRect(blk, bounds);
            if (CheckCollisionRecs(rBall, rBlk)) {
                /* Deactivate block and reflect ball on shallow axis */
                blk->active = false;

                float overlapLeft = (rBall.x + rBall.width) - rBlk.x;
                float overlapRight = (rBlk.x + rBlk.width) - rBall.x;
                float overlapTop = (rBall.y + rBall.height) - rBlk.y;
                float overlapBottom = (rBlk.y + rBlk.height) - rBall.y;
                float minHoriz = fminf(overlapLeft, overlapRight);
                float minVert = fminf(overlapTop, overlapBottom);

                if (minHoriz < minVert) {
                    /* Reflect X */
                    if (overlapLeft < overlapRight) {
                        gBall.pos.x = rBlk.x - rBall.width; /* place to left */
                    } else {
                        gBall.pos.x = rBlk.x + rBlk.width; /* place to right */
                    }
                    gBall.vel.x = -gBall.vel.x;
                } else {
                    /* Reflect Y */
                    if (overlapTop < overlapBottom) {
                        gBall.pos.y = rBlk.y - rBall.height; /* above */
                    } else {
                        gBall.pos.y = rBlk.y + rBlk.height; /* below */
                    }
                    gBall.vel.y = -gBall.vel.y;
                }

                break; /* only one block per frame */
            }
        }
    }
}

void GameScene_Draw(Rectangle bounds)
{
    if (!gInitialized) return;

    BlockGrid_Draw(&gGrid, bounds);
    Paddle_Draw(&gPaddle);
    
    /* Draw launch gauge when ball is stuck to paddle (pre-launch) */
    if (!gBallLaunched) {
        Texture2D tex = GetGuideTextureByIndex(gGuideFrame);
        Rectangle rPad = Paddle_GetRect(&gPaddle);
        float padCenterX = rPad.x + rPad.width * 0.5f;
        float padTopY = rPad.y;
        float gap = 20.0f; /* small gap above paddle */
        int drawX = (int)(padCenterX - tex.width * 0.5f);
        int drawY = (int)(padTopY - gap - tex.height);
        DrawTexture(tex, drawX, drawY, Fade(WHITE, 0.85f));

        /* Draw direction line showing where ball will launch */
        Rectangle rBall = Ball_GetRect(&gBall);
        float ballCenterX = rBall.x + rBall.width * 0.5f;
        float ballCenterY = rBall.y + rBall.height * 0.5f;
        
        /* Compute launch angle from gauge frame (same as launch logic) */
        const float minAngle = 3.14159265f / 4.0f;      /* 45° */
        const float maxAngle = 3.0f * 3.14159265f / 4.0f; /* 135° */
        float t = (float)(11 - gGuideFrame) / 10.0f;    /* reversed mapping */
        float releaseAngle = minAngle + t * (maxAngle - minAngle);
        
        /* Draw line in launch direction */
        const float lineLength = 80.0f;
        Vector2 startPoint = { ballCenterX, ballCenterY };
        Vector2 endPoint = {
            ballCenterX + cosf(releaseAngle) * lineLength,
            ballCenterY - sinf(releaseAngle) * lineLength /* negate Y for screen coords */
        };
        DrawLineEx(startPoint, endPoint, 2.0f, YELLOW);
    }
    Ball_Draw(&gBall);
}

void GameScene_Cleanup(void)
{
    if (gInitialized) {
        BlockGrid_Free(&gGrid);
        Level_Free(&gLevel);
        gInitialized = false;
        gBallLaunched = false;
    }
}
