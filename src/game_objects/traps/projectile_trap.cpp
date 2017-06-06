#include "projectile_trap.h"
#include "arrow.h"

#include <iostream>

#define MAX_PROJECTILES 5

ProjectileTrap::ProjectileTrap(int x, int z, int id) : Trap(x, z, id) {
    can_shoot = true;
}

void ProjectileTrap::s_update_this() {
    if (activation_type == RAYCAST)
        raycast_check();
    else if (activation_type == AI)
        update_ai();
}

void ProjectileTrap::raycast_check() {
    events::dungeon::request_raycast_event(
        transform.get_position(), transform.get_forward(), raycast_range,
        [&](GameObject *bt_hit) {
        Player *player = dynamic_cast<Player*>(bt_hit);
        if (player)
            shoot();
    });
}

// Setup to shoot every interval
void ProjectileTrap::setup_timer(long long time_interval_ms) {
    // cancels any previous timer, if exists.
    if (cancel_timer)
        cancel_timer();

    // TODO : Figure out why trap does not shoot out at proper intervals
    cancel_timer = Timer::get()->do_every(
        std::chrono::milliseconds(time_interval_ms),
        [&]() {
        shoot();
    });
}

// Makes copies of the given projectile to fill the projectile_pool with
void ProjectileTrap::fill_projectile_pool(ProjectileTypes type) {
    // Removes all projectiles from pool
    for(Projectile* proj : projectile_pool) {
        proj->disable();
    }
    projectile_pool.clear();

    // Fills projetile_pool
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        Projectile *projectile;
        switch (type) {
        case ARROW:
            projectile = new Arrow();
            break;
        default:
            return;
            break;
        }

        projectile_pool.push_back(projectile);
        projectile_pool[i]->set_parent(id);
        projectile_pool[i]->disable();
    }
}

// Looks for an available projectile from pool, then shoots it
void ProjectileTrap::shoot() {
    if (!can_shoot)
        return;

    Projectile *to_shoot = nullptr;

    // Get projectile from pool
    for (int i = 0; i < projectile_pool.size(); i++) {
        if (!projectile_pool[i]->is_enabled()) {
            to_shoot = projectile_pool[i];
            break;
        }
    }

    // Shoot if projectile was gained
    if (to_shoot) {
        to_shoot->fire(transform);

        // Send signal to client that a shot was fired.
        events::dungeon::network_collision_event(id, id);

        // Start cooldown timer
        can_shoot = false;
        Timer::get()->do_after(
            std::chrono::milliseconds(shoot_cooldown),
            [&]() {
            can_shoot = true;
        });
    }
}

void ProjectileTrap::c_on_collision(GameObject*) {
    play_trigger_animation();
}