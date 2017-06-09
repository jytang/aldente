#include <input/modal_input.h>
#include "mg_pump.h"
#include "events.h"
#include "game/game_state.h"
#include "game_objects/player.h"
#include <iostream>
#include <algorithm>

PumpMG::PumpMG(Context& to_set) : Minigame(to_set) {
    time = PUMPMG_TIME;
    info = MinigameInfo{
        {
            { "xboxControllerButtonA.png", "Pump" },
        }, // input legend
        std::string("Be first to pump up your balloon!"), // Objective
    };

    context = to_set;
}

void PumpMG::s_setup() {
    int count = 0;
    team1.clear();
    team2.clear();
    team1_points = 0;
    team2_points = 0;

    std::vector<int> player_assignments = context.player_ids;
    std::random_shuffle(player_assignments.begin(), player_assignments.end());

    proto::ServerMessage msg;
    proto::PumpAssignment *pump_asg = msg.mutable_pump_assignment();

    // Set up players
    bool curr_team = true;
    for (int id : player_assignments) {
        Player *player = dynamic_cast<Player*>(GameObject::game_objects[id]);
        assert(player);
        player->enable();

        if (curr_team)
            team1.push_back(player);
        else
            team2.push_back(player);

        int position = count++;
        player->get_rigid()->setLinearFactor(btVector3(0, 0, 0));
        player->get_rigid()->setLinearVelocity(btVector3(0, 0, 0));
        player->set_ghost(true);
        player->set_position(player_start_pos[position]);
        player->transform.look_at(glm::vec3(0, 0, 1));

        curr_team = !curr_team;
        proto::PumpPair* pair = pump_asg->add_pairs();
        pair->set_player_id(id);
        pair->set_pump(position);
    }

    events::server::announce(msg);

    inflate_conn = events::minigame::s_inflate_balloon_event.connect([&](Player* player) {
        // Find which team the player is on.
        bool is_team1 = std::find(team1.begin(), team1.end(), player) != team1.end();

        if (is_team1)
            team1_points++;
        else
            team2_points++;

        dynamic_cast<MGScenePump*>(context.minigame_scenes["pump"])->inflate_balloon(is_team1);
    });

    GameState::set_scene(context.minigame_scenes["pump"]);
    scene = context.minigame_scenes["pump"];
}

void PumpMG::s_teardown() {
    inflate_conn.disconnect();

    for (int id : context.player_ids) {
        Player *player = dynamic_cast<Player*>(GameObject::game_objects[id]);
        assert(player);
        player->get_rigid()->setLinearFactor(btVector3(1, 0, 1));
        player->get_rigid()->setLinearVelocity(btVector3(0, 0, 0));
        player->set_ghost(false);
        player->set_speed(2.0f);
    }

    // Assigns rewards to winning team. 
    // No reward if somehow they tie.
    if (team1_points > team2_points) {
        for (Player* player : team1) {
            player->s_modify_stats([&](PlayerStats &stats) {
                stats.add_coins(PUMPMG_REWARD);
            });
        }
    }
    else if (team1_points < team2_points) {
        for (Player* player : team2) {
            player->s_modify_stats([&](PlayerStats &stats) {
                stats.add_coins(PUMPMG_REWARD);
            });
        }
    }
}

void PumpMG::c_setup() {
    int count = 0;

    button_conn = input::ModalInput::get()->with_mode(input::ModalInput::NORMAL).buttons.connect([&](const events::ButtonData &d) {
        if (d.state == 1) {
            switch (d.input) {
            case events::BTN_A: {
                events::sound_effects_event(events::AudioData(AudioManager::PUMP_SOUND, false));

                proto::ClientMessage msg;
                msg.set_pump_request(context.player_id);
                events::client::send(msg);
                break;
            }
            default:
                break;
            }
        }
    });

    for (int player_id : context.player_ids) {
        Player *player = dynamic_cast<Player*>(GameObject::game_objects[player_id]);
        assert(player);
        player->disable();
        player->set_pump();
        player->set_anim_override(true);
        player->transform.look_at(glm::vec3(0, 0, 1));
    }

    GameState::set_scene(context.minigame_scenes["pump"]);
    scene = context.minigame_scenes["pump"];
    scene->connect_listeners();
}

void PumpMG::c_teardown() {
    button_conn.disconnect();

    for (int player_id : context.player_ids) {
        Player *player = dynamic_cast<Player*>(GameObject::game_objects[player_id]);
        assert(player);
        player->start_walk();
        player->set_anim_override(false);
    }

    scene->disconnect_listeners();
}

bool PumpMG::is_finished() {
    return false;
}