#include "construct.h"
#include "asset_loader.h"

Crate::Crate(int x, int z) {
    model = AssetLoader::asset_loader->get_model("textured.fbx");
    transform.set_position(x, 0.5f, z);
    transform.set_scale(0.5f, 0.5f, 0.5f);
}
