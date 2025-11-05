/**
 * @file audio.h
 * @brief Audio system for XBoing
 * 
 * Manages loading, playing, and cleanup of all game sound effects.
 */

#ifndef AUDIO_H
#define AUDIO_H

#include <stdbool.h>
#include "core/types.h"
#include "core/constants.h"

// =============================================================================
// Global Audio System
// =============================================================================
extern AudioSystem g_audioSystem;

// =============================================================================
// Public Functions
// =============================================================================

/**
 * @brief Initialize the audio system and load all sound files
 * @return true if all sounds loaded successfully, false otherwise
 */
bool Audio_Initialize(void);

/**
 * @brief Free all audio resources
 */
void Audio_Shutdown(void);

/**
 * @brief Play a sound effect
 * @param soundId The sound to play
 */
void Audio_PlaySound(SoundID soundId);

/**
 * @brief Set the master volume for all sounds
 * @param volume Volume level (0.0 to 1.0)
 */
void Audio_SetMasterVolume(float volume);

/**
 * @brief Get the current master volume
 * @return Current volume level (0.0 to 1.0)
 */
float Audio_GetMasterVolume(void);

#endif // AUDIO_H
