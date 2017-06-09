#include "hotair_basket.h"
#include "asset_loader.h"
#include "events.h"

HotAirBasket::HotAirBasket(int id) : GameObject(-1) {
    tag = "BASKET";
}

void HotAirBasket::setup_model() {
    attach_model(AssetLoader::get_model("basket"));
    model->set_shader(&ShaderManager::unlit);
    transform.set_scale({ 0.006f, 0.006f, 0.006f });
    initial_transform.set_scale(transform.get_scale());

    anim_player.set_anim("up", 1.0f, false);
}

void HotAirBasket::reset() {
    anim_player.stop();
}