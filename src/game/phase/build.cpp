#include <game/game_state.h>
#include <input/modal_input.h>
#include <util/config.h>
#include "build.h"
#include "audio/audio_manager.h"

#define STARTING_GOLD 100

bool BuildPhase::is_menu = true;

std::string BuildPhase::to_string() {
    return "BUILD PHASE";
}

void BuildPhase::s_setup() {
    Config::config->get_value(Config::str_show_dialogues, show_dialogues);

    // Update the round counter. Starts at 0, so pre-increment.
    ++context.current_round;

    GameState::set_scene(&GameState::main_scene);

    transition_after(0, 60, s_phase_when_done());
    ready_conn = events::player_ready_event.connect([&](int player_id) {
        // Ready up if not previously ready. Otherwise un-ready up.
        context.ready_flags[player_id] = !context.ready_flags[player_id];
    });

    s_verify_and_build_conn = events::build::s_verify_and_build.connect([&](proto::Construct& c) {
        Player* player = GameState::players[c.player_id()];
        assert(player);
        auto type = static_cast<ConstructType>(c.type());
        auto& construct = Constructs::CONSTRUCTS.at(type);

        // If player can afford to purchase, try to build on the tile.
        if (player->can_afford(construct.cost)) {
            events::build::s_try_build_event(c, [&]() {

                // If build successful, deduct the cost from player's funds.
                player->s_modify_stats([construct](PlayerStats& stats) {
                    stats.add_coins(-construct.cost);
                });
            });
        }
    });

    for (int id : context.player_ids) {
        context.ready_flags[id] = false;
    }

    for (auto & p : GameState::players) {
        Player* player = p.second;
        player->set_start_position({ 2.f, 0, 1.f + p.first });
        player->reset_position();

        // If it's the first round, grant everyone start gold
        if (context.current_round == 1) {
            player->s_modify_stats([](PlayerStats &stats) {
                stats.set_coins(STARTING_GOLD);
            });
        }
    }

    // Resets all game objects
    for (auto & kv : GameObject::game_objects) {
        kv.second->s_reset();
    }
}

void BuildPhase::c_setup() {
    // Updates the client-side round counter, and update the UI.
    events::ui::round_changed_event(++context.current_round);

    GameState::set_scene(&GameState::main_scene);

    events::build::start_build_event();
    events::build::select_grid_return_event();
    is_menu = true;

    joystick_conn = input::ModalInput::get()->with_mode(input::ModalInput::NORMAL).sticks.connect([&](const events::StickData &d) {

        // Left stick for UI Grid Movement
        if (d.input == events::STICK_LEFT) {
            // Movement axes.
            Direction dir;
            bool moving = true;

            if (d.state.first == events::INPUT_ANALOG_LEVELS && d.state.second == 0)
                dir = Direction::RIGHT;
            else if (d.state.first == -events::INPUT_ANALOG_LEVELS && d.state.second == 0)
                dir = Direction::LEFT;
            else if (d.state.second == events::INPUT_ANALOG_LEVELS && d.state.first == 0)
                dir = Direction::DOWN;
            else if (d.state.second == -events::INPUT_ANALOG_LEVELS && d.state.first == 0)
                dir = Direction::UP;
            else
                moving = false;

            if (moving) {
                if (is_menu)
                    events::build::select_grid_move_event(dir);
                else
                    events::build::build_grid_move_event(dir);
            }
        }
        else if (d.input == events::STICK_RIGHT) {
            events::build::pan_camera_event(d.state);
        }
    });

    button_conn = input::ModalInput::get()->with_mode(input::ModalInput::NORMAL).buttons.connect([&](const events::ButtonData &d) {
        Direction dir;
        bool d_pad = false;

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
            case events::BTN_A: {
                if (is_menu)
                    events::build::select_grid_confirm_event();
                else
                    events::build::build_grid_place_event();
                break;
            }
            case events::BTN_B: {
                if (!is_menu) {
                    events::build::select_grid_return_event();
                    is_menu = true;
                }
                break;
            }
            case events::BTN_START: {
                events::build::toggle_ui_ready_event();

                proto::ClientMessage msg;
                msg.set_ready_request(context.player_id);
                events::client::send(msg);
                break;
            }
            case events::BTN_UP: {
                dir = Direction::UP;
                d_pad = true;
                break;
            }
            case events::BTN_RIGHT: {
                dir = Direction::RIGHT;
                d_pad = true;
                break;
            }
            case events::BTN_DOWN: {
                dir = Direction::DOWN;
                d_pad = true;
                break;
            }
            case events::BTN_LEFT: {
                dir = Direction::LEFT;
                d_pad = true;
                break;
            }
            case events::BTN_LB: {
                if (!is_menu)
                    events::build::c_rotate_preview_event(false);
                else
                    events::ui::enable_leaderboard();
                break;
            }
            case events::BTN_RB: {
                if (!is_menu)
                    events::build::c_rotate_preview_event(true);
                break;
            }
            default:
                break;
            }
        }

        if (d_pad) {
            if (is_menu)
                events::build::select_grid_move_event(dir);
            else
                events::build::build_grid_move_event(dir);
        }
    });

    c_preview_conn = events::build::construct_selected_event.connect([&](ConstructType type) {
        Player* player = dynamic_cast<Player*>(GameObject::game_objects[context.player_id]);
        assert(player);
        int cost = Constructs::CONSTRUCTS.at(type).cost;
        bool afford = player->can_afford(cost);

        // Create a construct preview, tinted green/red based on whether or not player can afford.
        events::build::c_construct_preview_event(type, afford);
    });

    c_check_funds_conn = events::build::c_check_funds_event.connect([&](ConstructType type) {
        Player* player = dynamic_cast<Player*>(GameObject::game_objects[context.player_id]);
        assert(player);
        int cost = Constructs::CONSTRUCTS.at(type).cost;
        bool afford = player->can_afford(cost);

        // Switch to building on the 3D grid, since player can afford to build this construct.
        if (player->can_afford(cost)) {
            is_menu = false;
            events::build::construct_selected_event(type);
        }
    });

    // Play music
    events::music_event(events::AudioData(AudioManager::BUILD_MUSIC, true));

    // Resets game objects on client side
    for (auto & kv : GameObject::game_objects) {
        if (!kv.second) continue;
        if (dynamic_cast<Player*>(kv.second))
            kv.second->disable();
        kv.second->c_reset();
    }
}

proto::Phase BuildPhase::s_update() {
    bool all_ready = true;
    for (auto const &kv : context.ready_flags) {
        if (!kv.second) {
            all_ready = false;
            break;
        }
    }

    if (all_ready)
        return s_phase_when_done();
    else
        return next;
}

void BuildPhase::s_teardown() {
    cancel_clock_every();
    ready_conn.disconnect();
    s_verify_and_build_conn.disconnect();

    events::build::end_build_event();
}

void BuildPhase::c_teardown() {
    joystick_conn.disconnect();
    button_conn.disconnect();
    c_preview_conn.disconnect();
    c_check_funds_conn.disconnect();

    events::build::end_build_event();

    // Stop all music/sounds
    events::stop_music_event();
    events::stop_all_sounds_event();
}

proto::Phase BuildPhase::s_phase_when_done() {
    return (context.current_round == 1 && show_dialogues) ? proto::Phase::DUNGEON_TUTORIAL : proto::Phase::DUNGEON;
}
