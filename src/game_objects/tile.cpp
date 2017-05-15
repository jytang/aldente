#include "tile.h"
#include "util/color.h"
#include "asset_loader.h"

Tile::Tile() :
    GameObject(),
    construct(nullptr) {
    tag = "TILE";
}

FloorTile::FloorTile(int x, int z) : Tile::Tile() {
    width = 1;
    height = 1;
    this->x = x;
    this->z = z;
    buildable = true;

    set_position({ x, 0.0f, z });
}

void FloorTile::setup_instanced_model(int num_instances, std::vector<glm::mat4> &instance_matrix){
    Mesh* mesh = new Mesh();

    // Set's the mesh's location relative to the model
    mesh->local_transform = glm::mat4(1.0f);
    mesh->geometry = GeometryGenerator::generate_plane(1.f, true, num_instances);
    mesh->geometry->bind_instance_matrix(instance_matrix); // pass instance matrix to bind to buffer
    Material *mat = new Material(Color::WHITE);
    mesh->material = mat;
    mesh->geometry->attach_texture(AssetLoader::get_texture("cobblestone.png"));

    model->add_mesh(mesh);
}

WallTile::WallTile(int x, int z) : Tile::Tile() {
    width = 1;
    height = 1;
    this->x = x;
    this->z = z;
    buildable = false;

    set_position({x, 0.5f, z});

    events::RigidBodyData rigid = {
        glm::vec3(x,0.5f,z), //position
        0, //mass
        hit_box, //btshape
        glm::vec3(0,0,0), //inertia
        this //the gameobject
    };
    events::add_rigidbody_event(rigid);
}

void WallTile::setup_instanced_model(int num_instances, std::vector<glm::mat4> &instance_matrix){
    Mesh* mesh = new Mesh();

    // Set's the mesh's location relative to the model
    mesh->local_transform = glm::mat4(1.0f);
    mesh->geometry = GeometryGenerator::generate_cube(1.0f, num_instances);
    mesh->geometry->bind_instance_matrix(instance_matrix); // pass instance matrix to bind to buffer
    Material *mat = new Material(Color::WHITE);
    mesh->material = mat;
    mesh->geometry->attach_texture(AssetLoader::get_texture("wall.png"));

    model->add_mesh(mesh);
}
