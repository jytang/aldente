#include "grid.h"
#include "events.h"
#include "game/game_state.h"
#include "game/phase.h"

Grid::Grid(int w, int h) :
		hover(nullptr) {
    width = w;
    height = h;
    hoverX = 0;
    hoverZ = 0;

    for (int i = 0; i < width; i++) {
        std::vector<Tile *> newRow;
        for (int j = 0; j < height; j++) {
            if (i == 0 || i == width - 1 || j == 0 || j == height - 1) {
                WallTile* toAdd = new WallTile(i, j);
				newRow.push_back(toAdd);
            } else {
				FloorTile* toAdd = new FloorTile(i, j);
				newRow.push_back(toAdd);
            }
        }
        grid.push_back(newRow);
    }

    hover = grid[hoverX][hoverZ];

    events::joystick_event.connect([&](events::JoystickData d) {
        if (GameState::get_phase_type() != PhaseType::BUILD ||
            dynamic_cast<BuildPhase*>(GameState::get_phase())->is_menu) return;
        if (d.is_button == 0 && d.input == 0) {
            if (d.state == 5)
                move_selection(GridDirection::RIGHT);
            if (d.state == -5)
                move_selection(GridDirection::LEFT);
        }
        else if (d.is_button == 0 && d.input == 1) {
            if (d.state == 5)
                move_selection(GridDirection::DOWN);
            if (d.state == -5)
                move_selection(GridDirection::UP);
        }
    });

    events::build::construct_changed_event.connect([&](ConstructType c) {
        selected = c;
        fprintf(stderr, "CHANGE: %d\n", c);
    });

    events::joystick_event.connect([&](events::JoystickData d) {
        if (GameState::get_phase_type() == PhaseType::BUILD && 
            !dynamic_cast<BuildPhase*>(GameState::get_phase())->is_menu) {
            if (d.is_button == true && d.input == 0 && d.state == 0) {
                build();
            }
        }
    });
}

Grid::~Grid() {}

void Grid::update() {
    if (grid[hoverX][hoverZ] != hover) {
        hover->set_color(color::indian_red);
        hover = grid[hoverX][hoverZ];
        hover->set_color(color::windwaker_green);
    }
}

void Grid::build() {
    if (selected == REMOVE) {
        if (hover->get_construct()) {
            //TODO Make destructor for construct
            hover->set_construct(nullptr);
            hover->buildable = true;
        }
        return;
    }

    if (hover->buildable) {
        fprintf(stderr, "BUILD: %d\n", selected);
        switch (selected) {
        case CHEST: {
            Construct* to_add = new Crate(hover->getX(), hover->getZ());
            hover->set_construct(to_add);
            hover->buildable = false;
            break;
        }
        default:
            break;
        }
    }
}

void Grid::move_selection(GridDirection d) {
    switch (d) {
    case GridDirection::UP:
        hoverZ = glm::max(0, hoverZ - 1);
        break;
    case GridDirection::RIGHT:
        hoverX = glm::min(width - 1, hoverX + 1);
        break;
    case GridDirection::DOWN:
        hoverZ = glm::min(height - 1, hoverZ + 1);
        break;
    case GridDirection::LEFT:
        hoverX = glm::max(0, hoverX - 1);
        break;
    default:
        break;
    }
}
