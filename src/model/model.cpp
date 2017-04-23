#include "model.h"
#include "util/util.h"

Model::Model() {
    // No Scene Set. Assumes that it will be manually
    // set later before it can be drawn.
    model_mat = glm::mat4(1.0f);
}

Model::Model(Scene *scene) {
    this->scene = scene;
    model_mat = glm::mat4(1.0f);
}

Model::~Model() {}

void Model::set_scene(Scene *scene) {
    this->scene = scene;
}

void Model::add_mesh(Mesh *m) {
    meshes.push_back(m);
}

void Model::draw() {
    if (!scene)
        return;

    // Loop over meshes and their respective shader programs.
    for (Mesh *mesh : meshes) {
        mesh->shader->use();
        mesh->shader->set_VP(scene->camera->V, scene->camera->P);
        mesh->shader->send_cam_pos(scene->camera->cam_pos);
        mesh->shader->send_mesh_model(mesh->to_world);
        mesh->shader->set_material(mesh->material);
        mesh->shader->set_culling(!mesh->no_culling);
        mesh->shader->draw(mesh->geometry, model_mat);
    }
}

void Model::update() {
}

void Model::pass(Shader *s) {
    for (Mesh *mesh : meshes) {
        if (mesh->geometry) {
            s->send_mesh_model(mesh->to_world);
            s->draw(mesh->geometry, model_mat);
        }
    }
}

void Model::combine_meshes() {
    // Stop if one or zero meshes.
    if (meshes.size() <= 1)
        return;

    Mesh *mega_mesh = meshes.at(0);
    Geometry *mega_geometry = new Geometry();
    // TODO: GARBAGE COLLECT THIS PIECE OF GEOMETRY.
    mega_mesh->geometry = mega_geometry;
    mega_mesh->to_world = glm::mat4(1.f);

    unsigned int index_offset = 0;
    for (Mesh *mesh : meshes) {
        for (glm::vec3 v : mesh->geometry->vertices)
            mega_geometry->vertices.push_back(glm::vec3(mesh->to_world * glm::vec4(v, 1.f)));
        for (glm::vec3 n : mesh->geometry->normals)
            mega_geometry->normals.push_back(
                    glm::normalize(glm::mat3(glm::transpose(glm::inverse(mesh->to_world))) * n));
        for (unsigned int i : mesh->geometry->indices)
            mega_geometry->indices.push_back(i + index_offset);
        index_offset += (unsigned int) mesh->geometry->indices.size();
    }
    mega_geometry->populate_buffers();
    meshes.clear();
    add_mesh(mega_mesh);
}
