#include "render.h"

Render::Render(Window &window, SceneManager &scene_manager)
    : window(window), scene_manager(scene_manager) {}

// Render pipeline
// Alternatively can acquire current scene via some ChangeScene event
void Render::update() {
    // First pass: shadowmap.
    Scene *curr_scene = scene_manager.get_current_scene();
    shadows.shadow_pass(curr_scene);

    // Second pass: usual rendering.
    window.clear();

    curr_scene->draw_skybox(); // Skybox first so no overdrawing of things.
    curr_scene->draw();

    // Debug shadows as necessary.
    shadows.debug_shadows();
}
