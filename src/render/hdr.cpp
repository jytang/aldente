#include "hdr.h"

#include "shaders/shader.h"

void HDR::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, ShaderManager::hdr.FBO);
}

void HDR::render(Scene *scene) {
    // Unbind framebuffer.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ShaderManager::hdr.use();
    ShaderManager::hdr.draw(NULL, scene->info);
}