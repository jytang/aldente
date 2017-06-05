#include "platform.h"
#include "asset_loader.h"
#include "events.h"

Platform::Platform() : GameObject(-1) {
    tag = "PLATFORM";

    events::RigidBodyData platform_rigid;
    platform_rigid.object = this;
    platform_rigid.shape = new btBoxShape(btVector3(10, 0.5f, 5));
    platform_rigid.mass = 0;
    events::add_rigidbody_event(platform_rigid);
}

void Platform::setup_model() {
    attach_model(AssetLoader::get_model("cube"));
}