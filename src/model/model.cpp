#include "model.h"

void Model::add_mesh(Mesh *m) {
    meshes.push_back(m);
}

void Model::draw(Shader *shader, SceneInfo &scene_info, glm::mat4 to_world) {
    for (Mesh *mesh : meshes) {
        // Use model's attached shader if a NULL shader is passed in.
        if (shader != nullptr) {
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

// Draw multiple instances of this model according to the number of (transformation) mat4s in instance_matrix
void Model::draw_instanced(Shader *shader, SceneInfo &scene_info,
                    std::vector<glm::mat4> &instance_matrix) {
    for (Mesh *mesh : meshes) {
        // Use model's attached shader if a NULL shader is passed in.
        if (shader != nullptr) {
            shader->use();
            mesh->geometry->bind_instance_matrix(instance_matrix); // bind buffers on geometry
            // Pass identity to_world matrix because using instance_matrix for every instance
            shader->draw(mesh, scene_info, glm::mat4(1.f));
        } else {
            model_shader->use();
            mesh->geometry->bind_instance_matrix(instance_matrix); // bind buffers on geometry

            // Passes bones to shader manually to show that this model is animated
            for (int i = 0; i < bones.size(); i++) {
                char uni_name[128];
                snprintf(uni_name, sizeof(uni_name), "bones[%d]", i);
                model_shader->set_uni(uni_name, bones[i]);
            }

            model_shader->draw(mesh, scene_info, glm::mat4(1.f));
        }
    }
}

void Model::set_color(Color color) {
    for (Mesh *mesh : meshes) {
        mesh->material->diffuse = color;
    }
}

void Model::set_alpha(float alpha) {
    for (Mesh *mesh : meshes) {
        mesh->material->alpha = alpha;
    }
}

void Model::set_shadows(bool enable) {
    for (Mesh *mesh : meshes) {
        mesh->material->shadows = enable;
    }
}