#include "shadows.h"

#include "shaders/shader.h"
#include "window.h"
#include "util/config.h"
#include "events.h"

#include <iostream>
#include "scene/scene_info.h"

Shadows::Shadows()
    : debug_shadows_on(false) {

    // Set up callbacks
    events::toggle_debug_shadows_event.connect([&](void) {
        debug_shadows_on = !debug_shadows_on;
    });

    events::window_buffer_resize_event.connect([&](events::WindowSizeData d) {
        screen_width = d.width;
        screen_height = d.height;
    });
}

void Shadows::shadow_pass(Scene *scene) {
    // Update camera frustum. Disgusting place to do it though.
    // Should be done every frame, but elsewhere.
    float far_plane;
    Config::config->get_value(Config::str_far_plane, far_plane);
    scene->camera.update_frustum_corners(screen_width, screen_height, far_plane);

    // TODO: refactor scene_info as member of scene
    SceneInfo scene_info = { &scene->camera, scene->light_pos };
    Shader::shadow.use();
    Shader::shadow.pre_draw(scene_info);
    // Render using scene graph.
    scene->draw(&Shader::shadow);
    Shader::shadow.post_draw();
}

// Debug shadows by rendering the shadow map texture to a quad.
void Shadows::debug_shadows() {
    if (!debug_shadows_on) return;
    glViewport(0, 0, screen_width / 3, screen_height / 3);
    Shader::debug_shadow.use();
    Shader::debug_shadow.draw(NULL, scene_info);
    // Restore viewport
    glViewport(0, 0, screen_width, screen_height);
}
