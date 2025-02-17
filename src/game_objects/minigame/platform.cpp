#include "platform.h"
#include "asset_loader.h"
#include "events.h"

Platform::Platform(PlatformShape shape) : GameObject(-1) {
    tag = "PLATFORM";
    type = shape;

    events::RigidBodyData platform_rigid;
    platform_rigid.object = this;

    switch (type) {
        case PlatformShape::SQUARE:
            platform_rigid.shape = hit_box;
            break;
        case PlatformShape::CIRCLE:
            platform_rigid.shape = hit_cylinder;
            break;
        default: break;
    }

    platform_rigid.mass = 0;
    events::add_rigidbody_event(platform_rigid);
}

void Platform::setup_model() {
    switch (type) {
    case PlatformShape::SQUARE:
        attach_model(AssetLoader::get_model("cloud_platform"));
        model->set_shader(&ShaderManager::unlit);
        model->set_color({ 1.001f, 1.001f, 1.001f, false });
        break;
    case PlatformShape::CIRCLE:
        attach_model(AssetLoader::get_model("roundcloud"));
        model->set_shader(&ShaderManager::unlit);
        model->set_color({ 1.001f, 1.001f, 1.001f, false });
        break;
    default: break;
    }
}