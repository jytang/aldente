#include <game/game_state.h>
#include "menu.h"

void MenuPhase::s_setup() {
    ready_conn = events::player_ready_event.connect([&](int player_id) {
        context.ready_flags[player_id] = true;
    });
}

void MenuPhase::c_setup() {
    button_conn = events::button_event.connect([&](events::ButtonData d) {
        if (d.state == 0) return;
        switch (d.input) {
            case events::BTN_START: {
                proto::ClientMessage msg;
                msg.set_ready_request(context.player_id);
                events::client::send(msg);
                break;
            }
            case events::BTN_LB: {
                events::menu::c_cycle_player_model_event(false);
                break;
            }
            case events::BTN_RB: {
                events::menu::c_cycle_player_model_event(true);
                break;
            }
            default:
                break;
        }
    });
}

proto::Phase MenuPhase::s_update() {
    GameState::physics.update();

    // Send the position and orientation of the specified game objects.
    // Currently sending all Player objects and Constructs.
    for (auto const & o : GameObject::game_objects) {
        if (!dynamic_cast<Tile*>(o.second) && !dynamic_cast<Grid*>(o.second))
            context.updated_objects.insert(o.second);
    }
    events::dungeon::update_state_event(&context);

    bool all_ready = true;
    for (auto const &kv : context.ready_flags) {
        if (!kv.second) {
            all_ready = false;
            break;
        }
    }

    // Move on to build phase if all players ready, and there's at least one player.
    if (context.player_ids.size() > 0 && all_ready)
        return proto::Phase::BUILD;
    else
        return next;
}

void MenuPhase::c_update() {
}

void MenuPhase::s_teardown() {
    ready_conn.disconnect();

    events::menu::end_menu_event();
}

void MenuPhase::c_teardown() {
    button_conn.disconnect();

    events::menu::end_menu_event();
}
