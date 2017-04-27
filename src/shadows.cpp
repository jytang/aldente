#include "shadows.h"

#include "aldente_client.h"
#include "shaders/shadow_shader.h"
#include "window.h"
#include "util/config.h"
#include "events.h"

#include <iostream>

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
    float far_plane;
    Config::config->get_value(Config::str_far_plane, far_plane);

    // Disgusting.
    scene->camera.update_frustum_corners(screen_width, screen_height, far_plane);

    ShadowShader *ss = (ShadowShader *) ShaderManager::get_shader_program("shadow");
    // Set resolution of shadow map.
    glViewport(0, 0, ss->size, ss->size);
    // Render to the shadow shader's FBO.
    glBindFramebuffer(GL_FRAMEBUFFER, ss->FBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    ss->use();
    ss->light_pos = scene->light_pos;
    // Calculate light projection matrix to use based on frustum intersection
    ss->light_proj = scene->camera.frustum_ortho(scene->light_pos);

    // Disable back face culling for shadow accuracy
    glDisable(GL_CULL_FACE);
    // Render using scene graph.
    scene->pass(ss);
    // Re-enable back face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Debug shadows by rendering the shadow map texture to a quad.
void Shadows::debug_shadows() {
    if (!debug_shadows_on) return;
    glViewport(0, 0, screen_width / 3, screen_height / 3);
    ShaderManager::get_shader_program("debug_shadow")->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ((ShadowShader *) ShaderManager::get_shader_program("shadow"))->shadow_map_tex);
    Util::render_quad();
}
