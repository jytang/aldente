#include "empty_collider.h"
#include "events.h"
#include "penguin.h"

EmptyCollider::EmptyCollider(int id) : GameObject(id) {
    tag = "EMPTYCOLLIDER";

    if (id == ON_SERVER) {
        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = hit_box;
        rigid.mass = 0;
        events::add_rigidbody_event(rigid);

        // Set position
        set_position(glm::vec3(0,-3,0));
    }
}

void EmptyCollider::s_on_collision(GameObject *other) {
    if (other->tag == "PENGUIN") {
        dynamic_cast<Penguin*>(other)->reset_position();
    }
}