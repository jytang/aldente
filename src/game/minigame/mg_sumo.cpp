#include <input/modal_input.h>
#include "mg_sumo.h"
#include "events.h"
#include "game/game_state.h"
#include "game_objects/player.h"
#include <iostream>

SumoMG::SumoMG(Context& to_set) : Minigame(to_set) {
    time = SUMOMG_TIME;
    info = MinigameInfo{
        {
            { "xboxControllerLeftThumbstick.png", "Move" },
        }, // input legend
        std::string("Push the other players off the platform!"), // Objective
    };

    context = to_set;
}

void SumoMG::s_setup() {
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

    int player_count = 0;
    // Prep players for the minigame
    for (int id : context.player_ids) {
        dead_player_flags[id] = false;
        Player* player = dynamic_cast<Player*>(GameObject::game_objects[id]);
        assert(player);

        player->enable();
        player->get_rigid()->setLinearFactor(btVector3(1, 1.5f, 1));
        player->get_rigid()->setLinearVelocity(btVector3(0, 0, 0));
        player->set_speed(1.2f);
        player->set_position(player_start_pos[player_count++]);
        player->set_momentum(true);
        player->toggle_sumo_collider();
    }

    GameState::set_scene(context.minigame_scenes["sumo"]);
    scene = context.minigame_scenes["sumo"];
}

void SumoMG::s_teardown() {
    collision_conn.disconnect();
    flag_conn.disconnect();

    for (int id : context.player_ids) {
        Player *player = dynamic_cast<Player*>(GameObject::game_objects[id]);
        assert(player);
        player->get_rigid()->setLinearFactor(btVector3(1, 0, 1));
        player->get_rigid()->setLinearVelocity(btVector3(0, 0, 0));
        player->set_speed(2.0f);
        player->set_momentum(false);
        player->toggle_sumo_collider();
    }

    // Distribute reward evenly to all alive players
    int count = 0;
    for (auto const &kv : dead_player_flags) {
        if (!kv.second)
            count++;
    }

    Player *curr_player;
    for (auto const &kv : dead_player_flags) {
        if (!kv.second) {
            curr_player = dynamic_cast<Player*>(GameObject::game_objects[kv.first]);
            assert(curr_player);
            curr_player->s_modify_stats([&, kv](PlayerStats &stats) {
                stats.add_coins((int)SUMOMG_REWARD/count);
            });
        }
    }
}

void SumoMG::c_setup() {
    joystick_conn = input::ModalInput::get()->with_mode(input::ModalInput::NORMAL).sticks.connect([&](const events::StickData &d) {
        // Left stick
        if (d.input == events::STICK_LEFT) {
            // Should move this event out of dungeon namespace
            events::dungeon::network_player_move_event(d);
        }
    });

    GameState::set_scene(context.minigame_scenes["sumo"]);
    scene = context.minigame_scenes["sumo"];
}

void SumoMG::c_teardown() {
    joystick_conn.disconnect();
}

bool SumoMG::is_finished() {
    int count = 0;
    for (auto const &kv : dead_player_flags) {
        if (!kv.second)
            count++;
    }
    return (count == 1);
}