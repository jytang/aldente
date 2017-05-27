#include "arrow_trap.h"
#include "arrow.h"

ArrowTrap::ArrowTrap(int x, int z, int id) : ProjectileTrap(x, z, id) {
    if (id == ON_SERVER) {
        // Creates Rigid Body
        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = hit_box;
        rigid.position = { x, 0.0f, z };
        events::add_rigidbody_event(rigid);

        Arrow *arrow = new Arrow();
    }
}