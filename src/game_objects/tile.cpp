#include "tile.h"
#include "util/color.h"
#include "asset_loader.h"

Tile::Tile(int id) :
    GameObject(id),
    construct(nullptr) {
    tag = "TILE";
    // Currently, Tile IDs are not being set by the server, and is being done independently
    // for the client and server, so they aren't properly in sync. 
    // This is fine for now, but will need to be changed later if we want dynamic tile changes. 
}

FloorTile::FloorTile(int x, int z) : Tile::Tile() {
    this->x = x;
    this->z = z;
    buildable = true;
    traversable = true;

    set_position({ x, 0.0f, z });
}

void FloorTile::setup_instanced_model(int num_instances, std::vector<glm::mat4> instance_matrix){
    Mesh* mesh = new Mesh();

    // Set's the mesh's location relative to the model
    mesh->local_transform = glm::mat4(1.0f);
    mesh->geometry = GeometryGenerator::generate_plane(1.f, true, num_instances);
    mesh->geometry->bind_instance_matrix(instance_matrix); // pass instance matrix to bind to buffer
    Material *mat = new Material(Color::WHITE);
    mesh->material = mat;
    mesh->geometry->attach_texture(AssetLoader::get_texture("grass.png"));

    model->add_mesh(mesh);
}

EmptyTile::EmptyTile(int x, int z) : Tile::Tile(-1) {
    this->x = x;
    this->z = z;
    buildable = true;
    traversable = true;
    set_position({ x, 0.0f, z });

    transform.set_scale({ 0.004f, 0.005f, 0.004f });
}

// Sets up a model if designated by an environment object
void EmptyTile::setup_instanced_model(int num_instances, std::vector<glm::mat4> instance_matrix) {

    if (model_name.empty())
        return;

    attach_model(AssetLoader::get_model(model_name));

    for (Mesh *mesh : model->meshes) {
        // Set's the mesh's location relative to the model
        mesh->geometry->set_num_instances((num_instances));
        mesh->geometry->populate_buffers();
        mesh->geometry->bind_instance_matrix(instance_matrix); // pass instance matrix to bind to buffer
    }
}

TreeTile::TreeTile(int x, int z) : Tile::Tile() {
    this->x = x;
    this->z = z;
    buildable = false;
    traversable = false;

    events::RigidBodyData rigid;
    rigid.object = this;
    rigid.shape = hit_box;
    events::add_rigidbody_event(rigid);
    set_position({ x, 0.0f, z });

    transform.set_scale({ 0.004f, 0.005f, 0.004f });
}

void TreeTile::setup_instanced_model(int num_instances, std::vector<glm::mat4> instance_matrix){
    attach_model(AssetLoader::get_model("tree"));

    for (Mesh *mesh : model->meshes) {
        // Set's the mesh's location relative to the model
        mesh->geometry->set_num_instances((num_instances));
        mesh->geometry->populate_buffers();
        mesh->geometry->bind_instance_matrix(instance_matrix); // pass instance matrix to bind to buffer
    }
}
