#include "mgscene_penguin.h"
#include "game_objects/player.h"
#include "events.h"
#include "util/color.h"
#include "../../asset_loader.h"

void MGScenePenguin::s_update() {
    Scene::s_update();
}

void MGScenePenguin::c_update() {
    Scene::c_update();
}

void MGScenePenguin::s_setup() {
    for (int i = 0; i < MAX_PENGUINS; i++) {
        // init dem penguins
    }

    // Set up the platform
    GameObject* platform = new GameObject(-1);
    //platform->attach_model(AssetLoader::get_model("cube"));

    events::RigidBodyData platform_rigid;
    platform_rigid.object = platform;
    platform_rigid.shape = new btBoxShape(btVector3(5,0.5f,2.5f));
    platform_rigid.mass = 0;
    events::add_rigidbody_event(platform_rigid);

    platform->transform.set_scale(glm::vec3(10, 1, 5));
    platform->set_position(glm::vec3(0, -0.5f, 0));
}

void MGScenePenguin::c_setup() {

    // Set up the platform
    GameObject* platform = new GameObject(-1);
    platform->attach_model(AssetLoader::get_model("cube"));
    platform->transform.set_scale(glm::vec3(10, 1, 5));
    platform->set_position(glm::vec3(0, -0.5f, 0));
    objs.push_back(platform);

    // Setup lights.
    DirectionalLight *sun = new DirectionalLight(glm::vec3(0.f, -1.f, -1.f),
        Color::WHITE, 0.5f);
    add_light(sun);

    for (GameObject *obj : objs) {
        obj->setup_model();
    }
}

void MGScenePenguin::connect_listeners() {
}

void MGScenePenguin::disconnect_listeners() {
}
