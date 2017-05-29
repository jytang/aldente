#include "penguin_mg.h"

PenguinMG::PenguinMG(Context& to_set) {
    time = PENGUINMG_TIME;
    scene = nullptr;
    info = MinigameInfo{
        {
            { "xboxControllerLeftThumbStick.png", "Move" },
        }, // input legend
        std::string("Survive until time runs out"), // Objective
    };
    context = to_set;
}

PenguinMG::connect_listeners() {
    collision_conn = events::dungeon::network_collision_event.connect([&](int dispatcher, int other) {
        context.collisions.emplace(dispatcher, other);
    });

    interact_conn = events::dungeon::network_interact_event.connect([&](int dispatcher, int other) {
        context.interacts.emplace(dispatcher, other);
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
}