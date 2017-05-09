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

            // Passes bones to shader manually to show that this model is animated
            for (int i = 0; i < bones.size(); i++) {
                char uni_name[128];
                snprintf(uni_name, sizeof(uni_name), "bones[%d]", i);
                model_shader->set_uni(uni_name, bones[i]);
            }

            model_shader->draw(mesh, scene_info, to_world);
        }
    }
}
