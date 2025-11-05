/**
 * @file audio.c
 * @brief Audio system implementation for XBoing
 * 
 * Manages loading, playing, and cleanup of all game sound effects.
 */

#include "audio.h"
#include <raylib.h>
#include <stdio.h>

// =============================================================================
// Global State
// =============================================================================
AudioSystem g_audioSystem = {0};

// =============================================================================
// Sound File Paths
// =============================================================================
static const char *s_soundFiles[SOUND_COUNT] = {
    "resource/sounds/ammo.mp3", // when ammo block is destroyed
    "resource/sounds/applause.mp3",
    "resource/sounds/ball2ball.mp3", // when ball hits the paddle
    "resource/sounds/ballLost.mp3", // when ball goes off screen
    "resource/sounds/ballShot.mp3", // when ball is shot
    "resource/sounds/boing.mp3", // when block is destroyed
    "resource/sounds/bomb.mp3",
    "resource/sounds/bonus.mp3",
    "resource/sounds/buzzer.mp3",
    "resource/sounds/click.mp3",
    "resource/sounds/ddloo.mp3",
    "resource/sounds/doh1.mp3",
    "resource/sounds/doh2.mp3",
    "resource/sounds/doh3.mp3",
    "resource/sounds/doh4.mp3",
    "resource/sounds/evilLaugh.mp3",
    "resource/sounds/game_over.mp3", // after you run out of lives
    "resource/sounds/gate.mp3",
    "resource/sounds/hitHere.mp3",
    "resource/sounds/hypspc.mp3",
    "resource/sounds/intro.mp3",
    "resource/sounds/key.mp3",
    "resource/sounds/looksBad.mp3",
    "resource/sounds/metal.mp3",
    "resource/sounds/mgun.mp3",
    "resource/sounds/ouch.mp3",
    "resource/sounds/paddle.mp3",
    "resource/sounds/ping.mp3",
    "resource/sounds/shark.mp3",
    "resource/sounds/shoot.mp3", // bullet shot
    "resource/sounds/shotgun.mp3",
    "resource/sounds/spring.mp3",
    "resource/sounds/stamp.mp3",
    "resource/sounds/sticky.mp3", // when sticky powerup is activated
    "resource/sounds/supbons.mp3",
    "resource/sounds/toggle.mp3",
    "resource/sounds/tone.mp3",
    "resource/sounds/touch.mp3",
    "resource/sounds/wallsOff.mp3",
    "resource/sounds/warp.mp3",
    "resource/sounds/weeek.mp3",
    "resource/sounds/whizzo.mp3",
    "resource/sounds/whoosh.mp3",
    "resource/sounds/wzzz.mp3",
    "resource/sounds/wzzz2.mp3",
    "resource/sounds/youagod.mp3"
};

// =============================================================================
// Public Functions
// =============================================================================

bool Audio_Initialize(void) {
    bool success = true;
    
    for (int i = 0; i < SOUND_COUNT; i++) {
        if (!FileExists(s_soundFiles[i])) {
            fprintf(stderr, "[Audio] Missing sound file: %s\n", s_soundFiles[i]);
            success = false;
            continue;
        }

        g_audioSystem.sounds[i] = LoadSound(s_soundFiles[i]);
        
        if (g_audioSystem.sounds[i].frameCount == 0) {
            fprintf(stderr, "[Audio] Failed to load sound: %s\n", s_soundFiles[i]);
            success = false;
        }
    }

    g_audioSystem.masterVolume = 1.0f;
    
    if (success) {
        fprintf(stdout, "[Audio] Successfully loaded %d sound effects\n", SOUND_COUNT);
    }
    
    return success;
}

void Audio_Shutdown(void) {
    int unloadedCount = 0;
    
    for (int i = 0; i < SOUND_COUNT; i++) {
        if (g_audioSystem.sounds[i].frameCount > 0) {
            UnloadSound(g_audioSystem.sounds[i]);
            g_audioSystem.sounds[i].frameCount = 0;
            unloadedCount++;
        }
    }
    
    fprintf(stdout, "[Audio] Unloaded %d sound effects\n", unloadedCount);
}

void Audio_PlaySound(SoundID soundId) {
    if (soundId >= 0 && soundId < SOUND_COUNT) {
        if (g_audioSystem.sounds[soundId].frameCount > 0) {
            PlaySound(g_audioSystem.sounds[soundId]);
        } else {
            fprintf(stderr, "[Audio] Attempted to play invalid sound ID: %d\n", soundId);
        }
    }
}

void Audio_SetMasterVolume(float volume) {
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;
    g_audioSystem.masterVolume = volume;
}

float Audio_GetMasterVolume(void) {
    return g_audioSystem.masterVolume;
}
