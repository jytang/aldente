#include "test_chest.h"

// Test code for playing animation for the boy
void TestChest::update() { 
    anim_player.update();
}

void TestChest::open_chest() {
    anim_player.set_anim(&skel, "open");
    anim_player.play();
}