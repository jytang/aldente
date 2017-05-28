#include <game/game_state.h>
#include <input/modal_input.h>
#include "dungeon.h"
#include "game_objects/player.h"
#include "game_objects/essence.h"
#include "game_objects/traps/mobile_trap.h"
#include "audio/audio_manager.h"

void DungeonPhase::s_setup() {
    transition_after(60, proto::Phase::BUILD);

    collision_conn = events::dungeon::network_collision_event.connect([&](int dispatcher, int other) {
        context.collisions.emplace(dispatcher, other);
    });

    interact_conn = events::dungeon::network_interact_event.connect([&](int dispatcher, int other) {
        context.interacts.emplace(dispatcher, other);
    });

    events::dungeon::s_prepare_dungeon_event();

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
    context.player_finished = false;
    joystick_conn = input::ModalInput::get()->with_mode(input::ModalInput::NORMAL).sticks.connect([&](events::StickData d) {
        // Left stick
        if (d.input == events::STICK_LEFT) {
            events::dungeon::network_player_move_event(d);
        }
    });

    button_conn = input::ModalInput::get()->with_mode(input::ModalInput::NORMAL).buttons.connect([&](events::ButtonData d) {
        if (d.state == 0) return;
        switch (d.input) {
            case events::BTN_A:
                // A button pressed.
                events::dungeon::player_interact_event();
                break;
            case events::BTN_BACK:
                // Toggle leaderboard
                events::ui::toggle_leaderboard();
            break;
            default:
                break;
        }
    });

    player_finish_conn = events::player_finished_event.connect([&](int player_id) {
        if (player_id == context.player_id) {
            context.player_finished = true;
            events::dungeon::post_dungeon_camera_event();
        } else {
            // TODO: can do client-side notification here, e.g. "Player X has reached the goal!"
        }
    });

    essence_conn = events::dungeon::c_spawn_essence_event.connect([&](float x, float z, int id) {
        Essence *ess = new Essence(id);
        ess->set_position({ x, 0, z });
        ess->setup_model();
        GameState::scene_manager.get_current_scene()->objs.push_back(ess);
    });

    // Play music
    events::music_event(events::AudioData{ AudioManager::DUNGEON_MUSIC, 30, true });
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
        return proto::Phase::BUILD;
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
    joystick_conn.disconnect();
    button_conn.disconnect();
    essence_conn.disconnect();
}
