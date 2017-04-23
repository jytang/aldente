#include "aldente.h"
#include "shadows.h"
#include "shaders/shadow_shader.h"
#include "window.h"
#include "util/config.h"

Shadows *Shadows::shadows = new Shadows();

Shadows::Shadows() {}

Shadows::~Shadows() {}

void Shadows::shadow_pass(Scene *scene) {
    float far_plane;
    Config::config->get_value(Config::str_far_plane, far_plane);

    scene->camera->update_frustum_planes();
    scene->camera->update_frustum_corners(Window::window->width, Window::window->height, far_plane);

    ShadowShader *ss = (ShadowShader *) ShaderManager::get_shader_program("shadow");
    glViewport(0, 0, ss->size, ss->size);
    glBindFramebuffer(GL_FRAMEBUFFER, ss->FBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    ss->use();
    ss->light_pos = scene->light_pos;
    if (Aldente::aldente->shadows_on) {
        ss->light_proj = scene->camera->frustum_ortho();
    } else {
        ss->light_proj = glm::ortho(-1.f, 1.f, -1.f, 1.f, 0.f, 0.1f);
    }
    // Render using scene graph.
    glDisable(GL_CULL_FACE);
    scene->pass(ss);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Shadows::debug_shadows() {
    glViewport(0, 0, Window::window->width / 3, Window::window->height / 3);
    ShaderManager::get_shader_program("debug_shadow")->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ((ShadowShader *) ShaderManager::get_shader_program("shadow"))->shadow_map_tex);
    Util::render_quad();
}
