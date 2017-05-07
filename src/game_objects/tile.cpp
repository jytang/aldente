#include "tile.h"
#include "util/color.h"
#include "asset_loader.h"

Tile::Tile() :
    GameObject(),
    construct(nullptr) {}

FloorTile::FloorTile(int x, int z) : Tile::Tile() {
    width = 1;
    height = 1;
    this->x = x;
    this->z = z;
    buildable = true;

    transform.set_position((float)x, 0.0f, (float)z);
}

void FloorTile::update() {
}

void FloorTile::setup_model() {
    Mesh* mesh = new Mesh();

    // Set's the mesh's location relative to the model
    mesh->local_transform = glm::mat4(1.0f);
    mesh->geometry = GeometryGenerator::generate_plane(1.f, true);
    Material *mat = new Material(Color::WHITE);
    mesh->material = mat;
    mesh->geometry->attach_texture(AssetLoader::get_texture("cobblestone.png"));

    model->add_mesh(mesh);

    transform.set_position((float)x, 0.0f, (float)z);
}

WallTile::WallTile(int x, int z) : Tile::Tile() {
    width = 1;
    height = 1;
    this->x = x;
    this->z = z;
    buildable = false;

    transform.set_position(x, 0.5f, z);

    events::RigidBodyData rigid = {
        glm::vec3(x,0.5f,z), //position
        0, //mass
        hit_box, //btshape
        glm::vec3(0,0,0), //inertia
        this //the gameobject
    };
    events::add_rigidbody_event(rigid);
}

void WallTile::update() {
}

void WallTile::setup_model() {
    Mesh* mesh = new Mesh();

    // Set's the mesh's location relative to the model
    mesh->local_transform = glm::mat4(1.0f);
    mesh->geometry = GeometryGenerator::generate_cube(1.0f);
    Material *mat = new Material(Color::WHITE);
    mesh->material = mat;
    mesh->geometry->attach_texture(AssetLoader::get_texture("wall.png"));

    model->add_mesh(mesh);
}
