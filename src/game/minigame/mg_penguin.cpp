#include <input/modal_input.h>
#include "mg_penguin.h"
#include "events.h"
#include "game_objects/player.h"
#include <iostream>

PenguinMG::PenguinMG(Context& to_set) : Minigame(to_set) {
    time = PENGUINMG_TIME;
    scene = nullptr;
    info = MinigameInfo {
        {
            { "xboxControllerLeftThumbStick.png", "Move" },
        }, // input legend
        std::string("Don't get pushed off!"), // Objective
    };
}

void PenguinMG::s_setup() {
    collision_conn = events::dungeon::network_collision_event.connect([&](int dispatcher, int other) {
        context.collisions.emplace(dispatcher, other);
    });

    flag_conn = events::minigame::player_died_event.connect([&](int player_id) {
        Player *player = dynamic_cast<Player*>(GameObject::game_objects[player_id]);
        assert(player);
        player->disable();
        dead_player_flags[player_id] = true;

        /*proto::ServerMessage msg;
        msg.set_player_finished(player_id);
        events::server::announce(msg);*/
    });

    for (int id : context.player_ids) {
        dead_player_flags[id] = false;
        Player* player = dynamic_cast<Player*>(GameObject::game_objects[id]);
        assert(player);
        
        // Set player start location here
        //player->reset_position();
        //player->enable();
    }
}

void PenguinMG::s_teardown() {
    collision_conn.disconnect();
    flag_conn.disconnect();
}

void PenguinMG::c_setup() {
    joystick_conn = input::ModalInput::get()->with_mode(input::ModalInput::NORMAL).sticks.connect([&](const events::StickData &d) {
        // Left stick
        if (d.input == events::STICK_LEFT) {
            // Should move this event out of dungeon namespace
            events::dungeon::network_player_move_event(d);
        }
    });
}

void PenguinMG::c_teardown() {
    joystick_conn.disconnect();
}

bool PenguinMG::is_finished() {
    for (auto const &kv : dead_player_flags) {
        if (!kv.second)
            return false;
    }
    return true;
}