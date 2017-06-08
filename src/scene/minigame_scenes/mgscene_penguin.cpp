#include <timer.h>
#include "mgscene_penguin.h"
#include "game_objects/player.h"
#include "game_objects/minigame/empty_collider.h"
#include "game_objects/minigame/platform.h"
#include "events.h"
#include "util/color.h"
#include "../../asset_loader.h"

MGScenePenguin::MGScenePenguin() :
    cancel_spawn([]() {}) {
}

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

        // Set spawn position and rotation
        to_add->set_position(glm::vec3(10, 1, rand() % 10 - 5));
        to_add->transform.set_rotation(glm::vec3(0, -90, 0));

        // Set penguin speed
        to_add->set_speed(-(rand() % 3 + 4));

        // Set penguins to left direction
        to_add->set_movement(Direction::LEFT);

        to_add->disable();
        penguins.push_back(to_add);
        objs.push_back(to_add);
    }

    // Set up the platform
    Platform* platform = new Platform(PlatformShape::SQUARE);

    platform->set_position(glm::vec3(0, -0.5f, -0.5f));
    objs.push_back(platform);

    // Set up the collider below the platform to detect things falling off.
    EmptyCollider* collider = new EmptyCollider(-1);
    // Set position
    collider->set_position(glm::vec3(0, -3, 0));
    objs.push_back(collider);
}

void MGScenePenguin::c_setup() {
    // Set up penguins. Initially disabled.
    for (int i = 0; i < MAX_PENGUINS; i++) {
        Penguin* to_add = new Penguin();
        to_add->disable();
        penguins.push_back(to_add);
        objs.push_back(to_add);
    }

    // Set up the platform
    Platform* platform = new Platform(PlatformShape::SQUARE);
    
    platform->initial_transform.set_scale(glm::vec3(10, 1, 5));
    platform->transform.set_scale(glm::vec3(10, 0.5f, 5));
    platform->transform.set_position(glm::vec3(0, 0, -0.5f));
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

void MGScenePenguin::reset_camera() {
    info.camera.reset();
    info.camera.rotate_cam(glm::vec3(1, 0, 0), -70.0f);
    info.camera.cam_pos = glm::vec3(0, 12, 5);
    info.camera.recalculate();
}

void MGScenePenguin::reset_scene() {
    for (Penguin *penguin : penguins) {
        penguin->reset_position();
        penguin->disable();
    }
}

void MGScenePenguin::start_timers() {
    int count = 0;

    // Spawn penguins in waves of 10.
    cancel_spawn = Timer::get()->do_every(
        std::chrono::milliseconds(1000),
        [&, count]() mutable {
        for (int i = 0; i < 10; i++) {
            if (count < MAX_PENGUINS)
                penguins[count++]->enable();
        }

        if (count == (MAX_PENGUINS - 20))
            cancel_spawn();
    });

    // Spawn last 20 penguins at halfway point
    Timer::get()->do_after(std::chrono::milliseconds(15000), [&]() {
        for (int i = MAX_PENGUINS - 21; i < MAX_PENGUINS; i++) {
            penguins[i]->enable();
        }
    });
}

void MGScenePenguin::stop_timers() {
    cancel_spawn();
}