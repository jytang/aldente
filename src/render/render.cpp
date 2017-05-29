#include "render.h"

Render::Render(Window &window, SceneManager &scene_manager)
    : window(window), scene_manager(scene_manager) {}

// Render pipeline
// Alternatively can acquire current scene via some ChangeScene event
void Render::update() {
    // First pass: shadowmap.
    Scene *curr_scene = scene_manager.get_current_scene();
    shadows.shadow_pass(curr_scene);

    // Bind HDR framebuffer.
    hdr.bind();

    // Second pass: usual rendering.
    window.clear();

    curr_scene->draw();
    curr_scene->draw_skybox(); // Skybox rendered last for optimization

    // Apply HDR. Render as a quad.
    hdr.render(curr_scene);

    // Debug shadows as necessary.
    shadows.debug_shadows(curr_scene);
}
