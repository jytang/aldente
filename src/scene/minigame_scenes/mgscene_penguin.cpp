#include "mgscene_penguin.h"
#include "game_objects/player.h"
#include "events.h"
#include "util/color.h"

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
}

void MGScenePenguin::c_setup() {
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
