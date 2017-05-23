#include <game/game_state.h>
#include "dungeon.h"
#include "game_objects/player.h"
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

    flag_conn = events::dungeon::player_finished_event.connect([&](int player_id) {
        Player *player = dynamic_cast<Player*>(GameObject::game_objects[player_id]);
        assert(player);
        player->disable();
        goal_reached_flags[player_id] = true;

        proto::ServerMessage msg;
        msg.set_player_finished(player_id);
        events::server::announce(msg);
    });

    for (int id : context.player_ids) {
        goal_reached_flags[id] = false;
        Player* player = dynamic_cast<Player*>(GameObject::game_objects[id]);
        assert(player);
        player->reset_position();
        player->enable();
    }
}

void DungeonPhase::c_setup() {
    context.player_finished = false;
    joystick_conn = events::stick_event.connect([&](events::StickData d) {
        // Left stick
        if (d.input == events::STICK_LEFT) {
            events::dungeon::network_player_move_event(d);
        }
    });

    button_conn = events::button_event.connect([&](events::ButtonData d) {
        // A button pressed.
        if (d.input == events::BTN_A && d.state == 1) {
            events::dungeon::player_interact_event();
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

    // Play music
    events::AudioData d = { AudioManager::DUNGEON_MUSIC };
    events::music_event(d);
}

proto::Phase DungeonPhase::s_update() {
    GameState::physics.update();

    // Send the position and orientation of the specified game objects.
    // Currently sending all Player objects and Constructs.
    for (auto const & o : GameObject::game_objects) {
        if (dynamic_cast<Player*>(o.second) || dynamic_cast<Construct*>(o.second))
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
}

void DungeonPhase::c_teardown() {
    joystick_conn.disconnect();
    button_conn.disconnect();
}