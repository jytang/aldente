#include "basic_shader.h"

#include "util/color.h"
#include "shader_manager.h"
#include "scene/light/directional_light.h"
#include "scene/light/point_light.h"
#include "scene/light/spot_light.h"

#include <cstdio>

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
    char buf[128]; // hold uniform name as a char *, for perfomance when sending to shader.

    /* BONES */
    std::vector<glm::mat4> &bones = mesh->model_filter.skeleton.bones_final;
    for (int i = 0; i < bones.size(); i++) {
        sprintf(buf, "bones[%d]", i);
        set_uni(buf, bones[i]);
    }

    /* MESH UNIFORMS */
    // Send mesh local transformation matrix.
    set_uni("mesh_model", mesh->local_transform);

    // Send material.
    // Override from model filter.
    if (mesh->model_filter.material_filter) {
        set_uni("material.diffuse", mesh->model_filter.material.diffuse.to_vec());
        set_uni("material.specular", mesh->model_filter.material.specular.to_vec());
        set_uni("material.shininess", mesh->model_filter.material.shininess);
        set_uni("material.shadows_enabled", mesh->model_filter.material.shadows);
        set_uni("material.alpha", mesh->model_filter.material.alpha);
    } else {
        set_uni("material.diffuse", mesh->material->diffuse.to_vec());
        set_uni("material.specular", mesh->material->specular.to_vec());
        set_uni("material.shininess", mesh->material->shininess);
        set_uni("material.shadows_enabled", mesh->material->shadows);
        set_uni("material.alpha", mesh->material->alpha);
    }

    // Send texture uniforms.
    set_uni("texture_enabled", mesh->geometry->has_texture);
    // Bind Texture.
    if (mesh->geometry->has_texture) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mesh->geometry->texture);
        set_uni("texture_map", 1); // ID of this texture=1
    }

    /* SHADOW MATRICES */
    // Send shadow uniforms and bind its texture.
    set_uni("light_matrix", ShaderManager::shadow.light_matrix);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ShaderManager::shadow.shadow_map_tex);
    set_uni("shadow_map", 0); // ID of this texture=0

    /* LIGHTS */
    // Send directional lights.
    char *uni_prefix = "dir_lights["; // Uniform prefix for directional light array.
    set_uni("num_dir_lights", (int) scene_info.dir_lights.size());
    for (int i = 0; i < scene_info.dir_lights.size(); ++i) {
        DirectionalLight *l = scene_info.dir_lights[i];
        sprintf(buf, "%s%d].direction", uni_prefix, i);
        set_uni(buf, l->get_direction());
        sprintf(buf, "%s%d].color", uni_prefix, i);
        set_uni(buf, l->color.to_vec());
        sprintf(buf, "%s%d].ambient_coeff", uni_prefix, i);
        set_uni(buf, l->ambient_coeff);
        sprintf(buf, "%s%d].intensity", uni_prefix, i);
        set_uni(buf, l->intensity);
    }

    // Send point lights.
    set_uni("num_point_lights", (int) scene_info.point_lights.size());
    uni_prefix = "point_lights["; // Uniform prefix for point light array.
    for (int i = 0; i < scene_info.point_lights.size(); ++i) {
        PointLight *l = scene_info.point_lights[i];
        sprintf(buf, "%s%d].position", uni_prefix, i);
        set_uni(buf, l->get_position());
        sprintf(buf, "%s%d].color", uni_prefix, i);
        set_uni(buf, l->color.to_vec());
        sprintf(buf, "%s%d].ambient_coeff", uni_prefix, i);
        set_uni(buf, l->ambient_coeff);
        sprintf(buf, "%s%d].intensity", uni_prefix, i);
        set_uni(buf, l->intensity);
        sprintf(buf, "%s%d].quadratic", uni_prefix, i);
        set_uni(buf, l->quadratic);
    }

    // Send spot lights.
    set_uni("num_spot_lights", (int) scene_info.spot_lights.size());
    uni_prefix = "spot_lights["; // Uniform prefix for spot light array.
    for (int i = 0; i < scene_info.spot_lights.size(); ++i) {
        SpotLight *l = scene_info.spot_lights[i];
        sprintf(buf, "%s%d].position", uni_prefix, i);
        set_uni(buf, l->get_position());
        sprintf(buf, "%s%d].direction", uni_prefix, i);
        set_uni(buf, l->get_direction());
        sprintf(buf, "%s%d].color", uni_prefix, i);
        set_uni(buf, l->color.to_vec());
        sprintf(buf, "%s%d].angle", uni_prefix, i);
        set_uni(buf, l->angle);
        sprintf(buf, "%s%d].taper", uni_prefix, i);
        set_uni(buf, l->taper);
        sprintf(buf, "%s%d].intensity", uni_prefix, i);
        set_uni(buf, l->intensity);
        sprintf(buf, "%s%d].quadratic", uni_prefix, i);
        set_uni(buf, l->quadratic);
        sprintf(buf, "%s%d].ambient_coeff", uni_prefix, i);
        set_uni(buf, l->ambient_coeff);
    }

    /* TRANSFORMATION MATRICES */
    // Send camera position uniform.
    set_uni("cam_pos", scene_info.camera.cam_pos);
    // Send projection and view matrices of camera.
    set_uni("projection", scene_info.camera.P);
    set_uni("view", scene_info.camera.V);

    // Send model_to_world matrix.
    set_uni("model", to_world);

    /* ALL DONE. START DRAWING */
    // Bind mesh geometry and draw.
    mesh->geometry->bind();
    mesh->geometry->draw();
    // Unbind.
    glBindVertexArray(0);
}
