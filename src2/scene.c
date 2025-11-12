/*
 * scene.c
 * Simple scene manager implementation.
 */

#include "scene.h"

static Scene *current_scene = NULL;

void SceneManager_SetScene(Scene *scene) {
    if (current_scene && current_scene->Unload) {
        current_scene->Unload();
    }

    current_scene = scene;

    if (current_scene && current_scene->Init) {
        current_scene->Init();
    }
}

Scene *SceneManager_GetCurrent(void) {
    return current_scene;
}

void SceneManager_Update(void) {
    if (current_scene && current_scene->Update) current_scene->Update();
}

void SceneManager_Draw(void) {
    if (current_scene && current_scene->Draw) current_scene->Draw();
}

void SceneManager_UnloadCurrent(void) {
    if (current_scene && current_scene->Unload) {
        current_scene->Unload();
    }
    current_scene = NULL;
}
