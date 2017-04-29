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
    events::build::grid_move_event.connect([&](Direction dir, bool is_menu) {
        if (is_menu) return;
        move_selection(dir);
    });

    events::build::grid_placement_event.connect([&](bool is_menu) {
        if (is_menu) return;
        proto::Construct c;
        c.set_type(selected);
        c.set_x(hover->getX());
        c.set_z(hover->getZ());
        events::build::request_build_event(c);
    });

    events::build::construct_changed_event.connect([&](ConstructType c) {
        selected = c;
    });

    events::build::try_build_event.connect([&](proto::Construct& c) {
        bool permitted = verify_build(static_cast<ConstructType>(c.type()), c.x(), c.z());
        events::build::respond_build_event(c, permitted);
    });

    events::build::update_build_event.connect([&](proto::Construct& c) {
        build(static_cast<ConstructType>(c.type()), c.x(), c.z());
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
    Tile* candidate = grid[x][z];
    if (type == REMOVE) {
        return candidate->get_construct() != nullptr;
    }

    return candidate->buildable;
}

void Grid::build(ConstructType type, int x, int z) {
    Tile* candidate = grid[x][z];
    if (type == REMOVE) {
        // TODO Make destructor for construct
        candidate->set_construct(nullptr);
        candidate->buildable = true;
        return;
    }

    switch (type) {
    case CHEST: {
        Construct* to_add = new Crate(x, z);
        candidate->set_construct(to_add);
        candidate->buildable = false;
        break;
    }
    default:
        break;
    }
}

void Grid::move_selection(Direction d) {
    switch (d) {
    case UP:
        hoverZ = glm::max(0, hoverZ - 1);
        break;
    case RIGHT:
        hoverX = glm::min(width - 1, hoverX + 1);
        break;
    case DOWN:
        hoverZ = glm::min(height - 1, hoverZ + 1);
        break;
    case LEFT:
        hoverX = glm::max(0, hoverX - 1);
        break;
    default:
        break;
    }
}
