#include "model.h"
#include "util/util.h"

void Model::add_mesh(Mesh *m) {
    meshes.push_back(m);
}

void Model::draw(SceneCamera &camera, glm::mat4 to_world) {
    // Loop over meshes and their respective shader programs.
    for (Mesh *mesh : meshes) {
        mesh->shader->use();
        mesh->shader->set_VP(camera.V, camera.P);
        mesh->shader->send_cam_pos(camera.cam_pos);
        mesh->shader->send_mesh_model(mesh->to_world);
        mesh->shader->set_material(mesh->material);
        mesh->shader->set_culling(!mesh->no_culling);
        mesh->shader->draw(mesh->geometry, to_world);
    }
}

void Model::pass(Shader *s, glm::mat4 to_world) {
    for (Mesh *mesh : meshes) {
        if (mesh->geometry) {
            s->send_mesh_model(mesh->to_world);
            s->draw(mesh->geometry, to_world);
        }
    }
}