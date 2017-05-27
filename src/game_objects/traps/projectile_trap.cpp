#include "projectile_trap.h"

ProjectileTrap::ProjectileTrap(int x, int z, int id) : Trap(x, z, id) {
}

// Setup to shoot every interval
void ProjectileTrap::setup_timer(long long time_interval_ms) {
    // cancels any previous timer, if exists.
    if (cancel_timer)
        cancel_timer();

    cancel_timer = Timer::get()->do_every(
        std::chrono::milliseconds(time_interval_ms),
        [&]() {
        shoot();
    });
}

void ProjectileTrap::attach_projectile(Projectile *projectile) {
    this->projectile = projectile;
    projectile->set_parent(id);
    projectile->disable();
}

void ProjectileTrap::shoot() {
    if (projectile)
        projectile->fire(transform);
}