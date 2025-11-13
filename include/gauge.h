#ifndef XBOING_GAUGE_H
#define XBOING_GAUGE_H

#include <raylib.h>
#include <stdbool.h>

/* Launch gauge state and rendering */
typedef struct {
    int frame;          /* 1..11 current gauge frame */
    int direction;      /* 1 forward, -1 backward */
    float timer;        /* seconds accumulator */
    bool showDebugLine; /* draw direction line */
} Gauge;

/* Initialize gauge to default state */
void Gauge_Init(Gauge* gauge);

/* Update gauge animation */
void Gauge_Update(Gauge* gauge, float dt);

/* Reset gauge to starting position */
void Gauge_Reset(Gauge* gauge);

/* Toggle debug line on/off */
void Gauge_ToggleDebug(Gauge* gauge);

/* Get current launch angle in radians based on gauge frame */
float Gauge_GetLaunchAngle(const Gauge* gauge);

/* Draw gauge texture above paddle */
void Gauge_Draw(const Gauge* gauge, Rectangle paddleRect);

/* Draw debug line from ball showing launch direction */
void Gauge_DrawDebugLine(const Gauge* gauge, Rectangle ballRect);

#endif /* XBOING_GAUGE_H */
