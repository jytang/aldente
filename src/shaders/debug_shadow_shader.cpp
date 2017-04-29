#include "debug_shadow_shader.h"

void DebugShadowShader::init() {
    GLfloat quad_vertices[] = {
            // Positions        // Texture Coords
            -1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
            1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
    };
    // Setup quad VAO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (3 * sizeof(GLfloat)));
}

void DebugShadowShader::pre_draw(SceneInfo &scene_info) {}
void DebugShadowShader::post_draw() {}

void DebugShadowShader::draw(Mesh *mesh, SceneInfo &scene_info, glm::mat4 model_to_world) {
    // Set texture to be shadow map texture.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ShaderManager::shadow.shadow_map_tex);
    // Draw quad.
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
