#include <game/game_state.h>
#include "dungeon.h"
#include "game_objects/player.h"

void DungeonPhase::setup() {
//    transition_after(10, proto::Phase::BUILD);

    collision_conn = events::dungeon::network_collision_event.connect([&](int obj_id) {
        context.collisions.insert(obj_id);
    });

    interact_conn = events::dungeon::network_interact_event.connect([&](int obj_id) {
        context.interacts.insert(obj_id);
    });

    events::dungeon::place_goal_event();

    flag_conn = events::dungeon::player_finished_event.connect([&](int player_id) {
        goal_reached_flags[player_id] = true;
    });

    for (int id : context.player_ids) {
        goal_reached_flags[id] = false;
    }
}

void DungeonPhase::client_setup() {
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
}

proto::Phase DungeonPhase::update() {
    // Send the position and orientation of the specified game objects.
    // Currently sending all Player objects and Goal.
    for (auto const & o : GameObject::game_objects) {
        if (dynamic_cast<Player*>(o.second) || dynamic_cast<Goal*>(o.second))
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

void DungeonPhase::client_update() {
    GameObject* player_obj = GameObject::game_objects[context.player_id];
    events::dungeon::player_position_updated_event(player_obj->transform.get_position());
}

void DungeonPhase::teardown() {
    cancel_clock_every();
    collision_conn.disconnect();
    interact_conn.disconnect();
    flag_conn.disconnect();
}

void DungeonPhase::client_teardown() {
    joystick_conn.disconnect();
    button_conn.disconnect();
}