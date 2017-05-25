#include "slime.h"
#include "asset_loader.h"

Slime::Slime(int x, int z, int id) : MobileTrap(x, z, id) {
}

void Slime::setup_model() {
    Model *model = AssetLoader::get_model("slime");
    attach_model(model);
    transform.set_scale({ 0.004f, 0.004f, 0.004f });
}