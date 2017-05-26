#include  "projectile.h"

Projectile::Projectile(int id) : GameObject(id) {
    if (id == ON_SERVER) {
        notify_on_collision = true;
    }
}

void Projectile::s_update_this() {

}

void Projectile::s_on_collision(GameObject *other) {

}

void Projectile::c_on_collision(GameObject *other) {

}

void Projectile::handle_movement() {

}

void Projectile::fire(Transform transform_dir) {

}

void Projectile::setup_timer() {
    // cancels any previous timer, if exists.
    if (cancel_timer)
        cancel_timer();

    // Disable object if it times_out
    cancel_timer = Timer::get()->do_after(
        std::chrono::milliseconds(time_out_ms),
        [&]() {
        disable();
    });
}