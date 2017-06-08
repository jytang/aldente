#include <game/game_state.h>
#include <input/modal_input.h>
#include "dungeon.h"
#include "game_objects/player.h"
#include "game_objects/essence.h"
#include "game_objects/traps/mobile_trap.h"
#include "audio/audio_manager.h"
#include "timer.h"

std::string DungeonPhase::to_string() {
    return "DUNGEON PHASE";
}

void DungeonPhase::s_setup() {
    transition_after(6, 60, proto::Phase::MINIGAME); // Add for countdown

    collision_conn = events::dungeon::network_collision_event.connect([&](int dispatcher, int other) {
        context.collisions.emplace(dispatcher, other);
    });

    interact_conn = events::dungeon::network_interact_event.connect([&](int dispatcher, int other) {
        context.interacts.emplace(dispatcher, other);
    });

    events::dungeon::s_prepare_dungeon_event();

    // Start timer to spawn delayed goal.
    Timer::get()->do_after(std::chrono::seconds(30), []() {
        events::dungeon::s_create_goal();
    });

    essence_conn = events::dungeon::s_spawn_essence_event.connect([&](float x, float z) {
        Essence *ess = new Essence();
        ess->set_position({ x, 0, z });
        GameState::scene_manager.get_current_scene()->objs.push_back(ess);
    });

    flag_conn = events::dungeon::player_finished_event.connect([&](int player_id) {
        Player *player = dynamic_cast<Player*>(GameObject::game_objects[player_id]);
        assert(player);
        player->disable();
        goal_reached_flags[player_id] = next_place++;

        proto::ServerMessage msg;
        msg.set_player_finished(player_id);
        events::server::announce(msg);
    });

    for (int id : context.player_ids) {
        goal_reached_flags[id] = 0;
        Player* player = dynamic_cast<Player*>(GameObject::game_objects[id]);
        assert(player);
        player->reset_position();
        player->enable();
    }

    next_place = 1;
}

void DungeonPhase::c_setup() {
    events::ui::show_countdown({"3", "2", "1", "GO"}, [this]() {
        events::dungeon::c_start();

        context.player_finished = false;
        joystick_conn = input::ModalInput::get()->with_mode(input::ModalInput::NORMAL).sticks.connect([&](const events::StickData &d) {
            // Left stick
            if (d.input == events::STICK_LEFT) {
                events::dungeon::network_player_move_event(d);
            }
        });

        button_conn = input::ModalInput::get()->with_mode(input::ModalInput::NORMAL).buttons.connect([&](const events::ButtonData &d) {
            // On release
            if (d.state == 0) {
                switch (d.input) {
                    case events::BTN_LB:
                        events::ui::disable_leaderboard();
                        break;
                    default:
                        break;
                }
            } else {
                switch (d.input) {
                    case events::BTN_A:
                        // A button pressed.
                        events::dungeon::player_interact_event();
                        break;
                    case events::BTN_LB:
                        // Enable leaderboard
                        events::ui::enable_leaderboard();
                        break;
                    default:
                        break;
                }
            }
        });

        player_finish_conn = events::player_finished_event.connect([&](int player_id) {
            if (player_id == context.player_id) {
                context.player_finished = true;
                events::dungeon::post_dungeon_camera_event();
            } else {
                events::ui::show_notification("Someone reached the exit!", 1);
            }
        });

        essence_conn = events::dungeon::c_spawn_essence_event.connect([&](float x, float z, int id) {
            Essence *ess = new Essence(id);
            ess->set_position({ x, 0, z });
            ess->setup_model();
            GameState::scene_manager.get_current_scene()->objs.push_back(ess);
        });

        // Play music
        events::music_event(events::AudioData(AudioManager::DUNGEON_MUSIC, true));
    });
}

proto::Phase DungeonPhase::s_update() {
    GameState::physics.update();

    // Send the position and orientation of the specified game objects.
    // Currently sending all Player objects and Constructs.
    for (auto const & o : GameObject::game_objects) {
        if (!dynamic_cast<Tile*>(o.second) && !dynamic_cast<Grid*>(o.second))
            context.updated_objects.insert(o.second);
    }
    events::dungeon::update_state_event(&context);

    bool all_players_done = true;

    for (auto const &kv : goal_reached_flags) {
        if (!kv.second) {
            all_players_done = false;
            break;
        }
    }

    if (all_players_done)
        return proto::Phase::MINIGAME;
    else
        return next;
}

void DungeonPhase::c_update() {
    GameObject* player_obj = GameObject::game_objects[context.player_id];
    
    // Only apply camera update if player is still exploring
    if (!context.player_finished)
        events::dungeon::player_position_updated_event(player_obj->transform.get_position());
}

void DungeonPhase::s_teardown() {
    cancel_clock_every();
    collision_conn.disconnect();
    interact_conn.disconnect();
    flag_conn.disconnect();
    essence_conn.disconnect();

    // Assigns rewards depending on player's ranking
    Player *curr_player;
    for (auto const &kv : goal_reached_flags) {
        curr_player = dynamic_cast<Player*>(GameObject::game_objects[kv.first]);
        assert(curr_player);
        curr_player->s_modify_stats([&, kv](PlayerStats &stats) {
            stats.add_coins(rewards[kv.second]);
        });
    }
}

void DungeonPhase::c_teardown() {
    events::ui::disable_leaderboard(); // disable if it's being shown currently.
    joystick_conn.disconnect();
    button_conn.disconnect();
    essence_conn.disconnect();
    player_finish_conn.disconnect();

    events::stop_all_sounds();
}
