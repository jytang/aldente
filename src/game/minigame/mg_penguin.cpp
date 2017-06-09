#include <input/modal_input.h>
#include "mg_penguin.h"
#include "events.h"
#include "game/game_state.h"
#include "game_objects/player.h"
#include <iostream>

PenguinMG::PenguinMG(Context& to_set) : Minigame(to_set) {
    time = PENGUINMG_TIME;
    info = MinigameInfo {
        {
            { "xboxControllerLeftThumbstick.png", "Move" },
        }, // input legend
        std::string("Don't get pushed off!"), // Objective
    };

    context = to_set;
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

        proto::ServerMessage msg;
        msg.set_player_finished(player_id);
        events::server::announce(msg);
    });

    for (int id : context.player_ids) {
        dead_player_flags[id] = false;
        Player* player = dynamic_cast<Player*>(GameObject::game_objects[id]);
        assert(player);
        
        // Set player start location here
        player->set_position({ 0, 0, 0 });
        player->enable();
    }

    // Enable gravity on players
    for (int id : context.player_ids) {
        Player *player = dynamic_cast<Player*>(GameObject::game_objects[id]);
        assert(player);
        player->get_rigid()->setLinearFactor(btVector3(1, 1.5f, 1));
        player->get_rigid()->setLinearVelocity(btVector3(0, 0, 0));
        glm::vec3 pos = player->transform.get_position();
        pos.y = 1.f;
        player->set_speed(1.0f);
        player->set_position(pos);
    }

    GameState::set_scene(context.minigame_scenes["penguin"]);
    scene = context.minigame_scenes["penguin"];
}

void PenguinMG::s_teardown() {
    collision_conn.disconnect();
    flag_conn.disconnect();

    for (int id : context.player_ids) {
        Player *player = dynamic_cast<Player*>(GameObject::game_objects[id]);
        assert(player);
        player->get_rigid()->setLinearFactor(btVector3(1, 0, 1));
        player->get_rigid()->setLinearVelocity(btVector3(0, 0, 0));
        player->set_speed(2.0f);
    }

    // Assigns rewards to alive players
    Player *curr_player;
    for (auto const &kv : dead_player_flags) {
        if (!kv.second) {
            curr_player = dynamic_cast<Player*>(GameObject::game_objects[kv.first]);
            assert(curr_player);
            curr_player->s_modify_stats([&, kv](PlayerStats &stats) {
                stats.add_coins(PENGUINMG_REWARD);
            });
        }
    }
}

void PenguinMG::c_setup() {
    joystick_conn = input::ModalInput::get()->with_mode(input::ModalInput::NORMAL).sticks.connect([&](const events::StickData &d) {
        // Left stick
        if (d.input == events::STICK_LEFT) {
            // Should move this event out of dungeon namespace
            events::dungeon::network_player_move_event(d);
        }
    });

    GameState::set_scene(context.minigame_scenes["penguin"]);

    context.post_anim = []() {};
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