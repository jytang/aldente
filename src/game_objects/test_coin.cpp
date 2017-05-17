#include "test_coin.h"
#include <iostream>

TestCoin::TestCoin() {
}

// Test code for playing animation for the boy
void TestCoin::update_this() {
    anim_player.update();
}

void TestCoin::spin_coin() {
    anim_player.set_anim("spin");
    anim_player.set_loop(true);
    anim_player.play();
}