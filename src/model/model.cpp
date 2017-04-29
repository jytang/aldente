#include "model.h"

void Model::add_mesh(Mesh *m) {
    meshes.push_back(m);
}

void Model::draw(Shader *shader, SceneInfo &scene_info, glm::mat4 to_world) {
    for (Mesh *mesh : meshes) {
        // Use model's attached shader if a NULL shader is passed in.
        if (shader != NULL) {
            shader->use();
            shader->draw(mesh, scene_info, to_world);
        } else {
            model_shader->use();
            model_shader->draw(mesh, scene_info, to_world);
        }
    }
}