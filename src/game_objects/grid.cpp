#include "grid.h"
#include "events.h"
#include "game/phase.h"
#include "util/color.h"

#include <fstream>

#define FLOOR_TILE 3
#define WALL_TILE 5

// Default color is white, meaning that it only uses texture
Color default_color = Color::WHITE;
// When selected, will tint the tile green, but keeps texture
Color select_color = Color::GREEN;

Grid::Grid(const char *map_loc) :
        hover(nullptr), hoverX(0), hoverZ(0) {
    load_map(map_loc);

    hover = grid[0][0];

    setup_listeners();
}

Grid::~Grid() {}

void Grid::setup_listeners() {
    events::build::build_grid_move_event.connect([&](Direction dir) {
        move_selection(dir);
    });

    events::build::build_grid_place_event.connect([&]() {
        proto::Construct c;
        c.set_type(selected);
        c.set_x(hover->getX());
        c.set_z(hover->getZ());
        events::build::request_build_event(c);
    });

    events::build::construct_changed_event.connect([&](ConstructType type) {
        selected = type;
    });

    events::build::try_build_event.connect([&](proto::Construct& c) {
        bool permitted = verify_build(static_cast<ConstructType>(c.type()), c.x(), c.z());
        c.set_status(permitted);
        events::build::respond_build_event(c);
    });

    events::build::update_build_event.connect([&](proto::Construct& c) {
        build(static_cast<ConstructType>(c.type()), c.x(), c.z());
    });
}

void Grid::update() {
    if (grid[hoverX][hoverZ] != hover) {
        hover->set_color(default_color);
        hover = grid[hoverX][hoverZ];
        hover->set_color(select_color);
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

    switch (type) {
    case CHEST: {
        Construct* to_add = new Crate(x, z);
        candidate->set_construct(to_add);
        candidate->buildable = false;
        break;
    }
    case REMOVE: {
        // TODO: Move destructor to construct's destructor.
        if (candidate->get_construct() != nullptr) {
            events::remove_rigidbody_event(dynamic_cast<GameObject*>(candidate->get_construct()));
            candidate->set_construct(nullptr);
            candidate->buildable = true;
        }
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

void Grid::load_map(const char *map_loc) {
    // Loads map from file
    std::ifstream fin;
    fin.open(map_loc);

    // File does not exist, exit
    if (!fin.good())
        return;

    std::string str_buf;
    int int_buf;

    // Parse File
    while (!fin.eof()) {
        fin >> str_buf;
        if (str_buf == "height") {
            fin >> int_buf;
            height = int_buf;
        }
        else if (str_buf == "width") {
            fin >> int_buf;
            width = int_buf;
        }
        else if (str_buf == "tag") {
            // To implement later, if we want some automated way
            // in the file to check what number corresponds to what type of tile
            // Currently, tile_ids are hardcoded.
        }
        else if (str_buf == "data") {
            // Parses data and creates grid
            for (int r = 0; r < height; r++) {
                std::vector<Tile *> new_row;

                for (int c = 0; c < width; c++) {
                    fin >> int_buf;
                    new_row.push_back(make_tile(int_buf, c, r));
                }

                grid.push_back(new_row);
            }
        }
    }

    fin.close();
}

Tile *Grid::make_tile(int tile_id, int x, int z) {

    Tile *new_tile = nullptr;

    switch (tile_id) {
    case FLOOR_TILE:
        new_tile = new FloorTile(x, z);
        break;
    case WALL_TILE:
        new_tile = new WallTile(x, z);
        break;
    default:
        break;
    }

    return new_tile;
}
