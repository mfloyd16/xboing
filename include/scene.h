/* scene.h - simple scene state machine for the demo
 * Header placed in top-level include/ per workspace convention.
 */
#ifndef XBOING_SCENE_H
#define XBOING_SCENE_H

#include <stdbool.h>

typedef enum SceneId {
    SCENE_INTRO = 0,
    SCENE_HOWTO,
} SceneId;

/* Process input and update the current scene.
 * - current: pointer to the current scene id; may be changed to request a
 *   scene transition.
 * Returns true to continue running, false to request exit.
 */
bool Scene_Update(SceneId* current);

/* Draw the current scene. Must be called between BeginDrawing()/EndDrawing(). */
void Scene_Draw(SceneId current);

#endif /* XBOING_SCENE_H */
