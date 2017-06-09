#include "hotair_balloon.h"
#include "asset_loader.h"
#include "events.h"

HotAirBalloon::HotAirBalloon(int id, BalloonColor to_set) : GameObject(id) {
    tag = "BALLOON";
    color = to_set;
    scale = INITIAL_SCALE;
}

void HotAirBalloon::setup_model() {
    switch (color) {
        case BalloonColor::BALLOON_RED:
            attach_model(AssetLoader::get_model("red_balloon"));
            break;
        case BalloonColor::BALLOON_BLUE:
            attach_model(AssetLoader::get_model("blue_balloon"));
            break;
        default: break;
    }
    //model->set_shader(&ShaderManager::unlit);
    transform.set_scale({ INITIAL_SCALE, INITIAL_SCALE, INITIAL_SCALE });
    initial_transform.set_scale(transform.get_scale());
    anim_player.set_anim("up", 1.0f, false);
}

void HotAirBalloon::c_update_state(glm::mat4 mat, bool enab) {
    if (enab)
        enable();
    else
        disable();

    if (enabled) {
        transform.set_world_mat(mat);
        set_rb_transform();
    }
}

void HotAirBalloon::inflate() {
    scale += 0.000025f;
    transform.set_scale(scale, scale, scale);
}

void HotAirBalloon::reset() {
    transform.set_scale(glm::vec3(INITIAL_SCALE, INITIAL_SCALE, INITIAL_SCALE));
    scale = INITIAL_SCALE;
    anim_player.stop();
}