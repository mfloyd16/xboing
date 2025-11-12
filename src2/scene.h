/*
 * scene.h
 * Minimal scene manager API for Raylib-based projects.
 *
 * Each scene implements four optional callbacks:
 *   - Init  : allocate/load scene resources
 *   - Update: update game logic / handle input
 *   - Draw  : render the frame for this scene
 *   - Unload: free scene resources
 *
 * The SceneManager switches scenes by calling Unload on the old scene
 * (if present) and Init on the new scene (if present).
 */

#ifndef XBOING_SCENE_H
#define XBOING_SCENE_H

#include <stdbool.h>

typedef struct Scene {
    void (*Init)(void);   /* called once when scene becomes active */
    void (*Update)(void); /* called every frame to update logic */
    void (*Draw)(void);   /* called every frame to draw */
    void (*Unload)(void); /* called when scene is replaced or program exits */
    const char *name;     /* optional human-friendly name */
} Scene;

/* Set the active scene. This will call Unload() on the previously active
   scene (if any) and then call Init() on the new scene (if not NULL). */
void SceneManager_SetScene(Scene *scene);

/* Return pointer to the currently active scene, or NULL if none. */
Scene *SceneManager_GetCurrent(void);

/* Convenience wrappers called from your main loop to dispatch to the
   current scene's Update and Draw callbacks. They are safe to call if
   no scene is active. */
void SceneManager_Update(void);
void SceneManager_Draw(void);

/* Unload and clear the current scene (calls Unload() if present). */
void SceneManager_UnloadCurrent(void);

#endif /* XBOING_SCENE_H */
