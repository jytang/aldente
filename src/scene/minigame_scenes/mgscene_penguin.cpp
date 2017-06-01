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
    // Set up penguins. Initially disabled.
    for (int i = 0; i < MAX_PENGUINS; i++) {
        Penguin* to_add = new Penguin();
        to_add->disable();
        penguins.push_back(to_add);
        objs.push_back(to_add);
    }

    // Set up the platform
    GameObject* platform = new GameObject(-1);

    events::RigidBodyData platform_rigid;
    platform_rigid.object = platform;
    platform_rigid.shape = new btBoxShape(btVector3(10,0.5f,5));
    platform_rigid.mass = 0;
    events::add_rigidbody_event(platform_rigid);

    platform->transform.set_scale(glm::vec3(2, 1, 0.5f));
    platform->set_position(glm::vec3(0, -0.5f, 0));
}

void MGScenePenguin::c_setup() {
    // Set up penguins. Initially disabled.
    for (int i = 0; i < MAX_PENGUINS; i++) {
        Penguin* to_add = new Penguin();
        to_add->disable();
        to_add->setup_model();
        penguins.push_back(to_add);
        objs.push_back(to_add);
    }

    // Set up the platform
    GameObject* platform = new GameObject(-1);
    platform->attach_model(AssetLoader::get_model("cube"));
    platform->transform.set_scale(glm::vec3(10, 1, 5));
    platform->set_position(glm::vec3(0, -0.5f, 0));
    objs.push_back(platform);

    events::RigidBodyData platform_rigid;
    platform_rigid.object = platform;
    platform_rigid.shape = new btBoxShape(btVector3(10, 0.5f, 5));
    platform_rigid.mass = 0;
    events::add_rigidbody_event(platform_rigid);

    // Setup lights.
    DirectionalLight *sun = new DirectionalLight(glm::vec3(0.f, -1.f, -1.f),
        Color::WHITE, 0.5f);
    add_light(sun);

    for (GameObject *obj : objs) {
        obj->setup_model();
    }
    
    info.camera.cam_pos = glm::vec3(0, 12, 5);
    info.camera.rotate_cam(glm::vec3(1, 0, 0), -70.0f);
}

void MGScenePenguin::connect_listeners() {
}

void MGScenePenguin::disconnect_listeners() {
}
