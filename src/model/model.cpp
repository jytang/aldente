#include "model.h"

void Model::add_mesh(Mesh *m) {
    meshes.push_back(m);
}

void Model::draw(Shader *shader, SceneInfo &scene_info, glm::mat4 to_world) {
    // Use model's attached shader if a NULL shader is passed in.
    // Otherwise use passed in shader.
    Shader *shader_to_use = shader == nullptr ? model_shader : shader;
    shader_to_use->use();

    for (Mesh *mesh : meshes) {
        shader_to_use->draw(mesh, scene_info, to_world);
    }
}

// Draw multiple instances of this model according to the number of (transformation) mat4s in instance_matrix
void Model::draw_instanced(Shader *shader, SceneInfo &scene_info,
                    std::vector<glm::mat4> &instance_matrix) {
    // Use model's attached shader if a NULL shader is passed in.
    Shader *shader_to_use = shader == nullptr ? model_shader : shader;
    shader_to_use->use();

    for (Mesh *mesh : meshes) {
        mesh->geometry->bind_instance_matrix(instance_matrix); // bind buffers on geometry
        // Pass identity to_world matrix because using instance_matrix for every instance
        shader_to_use->draw(mesh, scene_info, glm::mat4(1.f));
    }
}

void Model::set_model_filter(ModelFilter &filter) {
    // Set all meshes' model filters.
    for (Mesh *mesh : meshes) {
        mesh->model_filter = filter;
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

void Model::save_colors() {
    original_colors.clear();
    for (Mesh *mesh : meshes) {
        original_colors.push_back(mesh->material->diffuse);
    }
}

void Model::reset_colors() {
    if (original_colors.size() != meshes.size())
        return; // Cannot reset color

    // Reset using original colors
    for (int i = 0; i < original_colors.size(); i++) {
        meshes[i]->material->diffuse = original_colors[i];
    }
}

void Model::add_colors(Color color) {
    for (Mesh *mesh : meshes) {
        mesh->material->diffuse.add(color);
    }
}

void Model::multiply_colors(Color color) {
    for (Mesh *mesh : meshes) {
        mesh->material->diffuse.multiply(color);
    }
}