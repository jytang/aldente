#include "grid.h"
#include "events.h"
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
    setup_listeners();
}

Grid::~Grid() {}

void Grid::setup_listeners() {
    events::joystick_event.connect([&](events::JoystickData d) {
        if (Phase::curr_phase != PhaseType::BUILD || BuildPhase::is_menu) return;

        // Wants to build a construct at this tile.
        if (d.is_button && d.input == 0 && d.state == 0) {
            events::build::ConstructData cd = { selected, hover->getX(), hover->getZ() };
            events::build::request_build_event(cd);
        }

        // Hover tile movement.
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
    });

    events::build::try_build_event.connect([&](events::build::ConstructData& cd) {
        bool permitted = verify_build(cd.type, cd.x, cd.z);
        events::build::respond_build_event(cd, permitted);
    });

    events::build::update_build_event.connect([&](events::build::ConstructData& cd) {
        build(cd.type, cd.x, cd.z);
    });
}

void Grid::update() {
    if (grid[hoverX][hoverZ] != hover) {
        hover->set_color(color::indian_red);
        hover = grid[hoverX][hoverZ];
        hover->set_color(color::windwaker_green);
    }
}

bool Grid::verify_build(ConstructType type, int x, int z) {
    if (type == REMOVE) {
        return hover->get_construct() != nullptr;
    }

    return hover->buildable;
}

void Grid::build(ConstructType type, int x, int z) {
    if (type == REMOVE) {
        //TODO Make destructor for construct
        hover->set_construct(nullptr);
        hover->buildable = true;
        return;
    }

    fprintf(stderr, "BUILD: %d\n", type);
    switch (type) {
    case CHEST: {
        Construct* to_add = new Crate(x, z);
        hover->set_construct(to_add);
        hover->buildable = false;
        break;
    }
    default:
        break;
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
