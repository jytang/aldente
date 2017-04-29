#include "basic_shader.h"

#include "util/colors.h"

void BasicShader::init() {
    // Nothing to be done. For now.
}

void BasicShader::pre_draw(SceneInfo &scene_info) {
    // Nothing to be done. For now.
}

void BasicShader::post_draw() {
    // Nothing to be done. For now.
}

void BasicShader::draw(Mesh *mesh, SceneInfo &scene_info, glm::mat4 to_world) {
    /* MESH UNIFORMS */
    // Send mesh local transformation matrix.
    glUniformMatrix4fv(uni("mesh_model"), 1, GL_FALSE, &mesh->local_transform[0][0]);

    // Send material.
    uni_vec3("material.diffuse", mesh->material->diffuse);
    uni_vec3("material.specular", mesh->material->specular);
    uni_vec3("material.ambient", mesh->material->ambient);
    glUniform1f(uni("material.shininess"), mesh->material->shininess);
    glUniform1i(uni("shadows_enabled"), mesh->material->shadows);

    // Send texture uniforms.
    glUniform1i(uni("texture_enabled"), mesh->geometry->has_texture);
    // Bind Texture.
    if (mesh->geometry->has_texture) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mesh->geometry->texture);
        glUniform1i(uni("texture_map"), 1); // ID of this texture=1
    }

    /* SHADOW AND TRANSFORMATION MATRICES */
    // Send shadow uniforms and bind its texture.
    glUniformMatrix4fv(uni("light_matrix"), 1, GL_FALSE, &ShaderManager::shadow.light_matrix[0][0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ShaderManager::shadow.shadow_map_tex);
    glUniform1i(uni("shadow_map"), 0); // ID of this texture=0

    // Send lighting uniforms.
    uni_vec3("dir_light.direction", -scene_info.light_pos);
    uni_vec3("dir_light.color", color::white); // white light
    glUniform1f(uni("dir_light.ambient_coeff"), 0.2f);

    // Send camera position uniform.
    uni_vec3("eye_pos", scene_info.camera.cam_pos);
    // Send projection and view matrices of camera.
    glUniformMatrix4fv(uni("projection"), 1, GL_FALSE, &scene_info.camera.P[0][0]);
    glUniformMatrix4fv(uni("view"), 1, GL_FALSE, &scene_info.camera.V[0][0]);

    // Send model_to_world matrix.
    glUniformMatrix4fv(uni("model"), 1, GL_FALSE, &to_world[0][0]);

    /* ALL DONE. START DRAWING */
    // Bind mesh geometry and draw.
    mesh->geometry->bind();
    mesh->geometry->draw();
    // Unbind.
    glBindVertexArray(0);
}
