#include "empty_collider.h"
#include "events.h"
#include "penguin.h"

EmptyCollider::EmptyCollider(int id) : GameObject(id) {
    tag = "EMPTYCOLLIDER";

    events::RigidBodyData rigid;
    rigid.object = this;
    rigid.shape = hit_box;
    rigid.mass = 0;
    events::add_rigidbody_event(rigid);

    // Set position
    set_position(glm::vec3(0,-3,0));

    notify_on_collision = true;
}

void EmptyCollider::s_on_collision(GameObject *other) {
    if (dynamic_cast<Penguin*>(other)) {
        dynamic_cast<Penguin*>(other)->reset_position();
    }
    else if (dynamic_cast<Player*>(other)) {
        other->disable();
        events::minigame::player_died_event(other->get_id());
    }
}