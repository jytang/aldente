#include "grid.h"
#include "events.h"
#include "game/phase/phase.h"
#include "util/color.h"

#include <fstream>

#define FLOOR_TILE 3
#define WALL_TILE 5

Grid::Grid(const char *map_loc) :
        hover(nullptr), hover_col(0), hover_row(0),
        width(0), height(0), goal(nullptr) {

    tag = "GRID";
    model = nullptr;
    rigidbody = nullptr;

    load_map(map_loc);

    // Default starting location
    hover = grid[0][0];

    setup_listeners();
}

// Override draw - using instanced rendering for performance purposes.
void Grid::c_draw(Shader *shader, SceneInfo &scene_info) {
    // Draw all types of tiles using instanced rendering.
    for (auto it = tile_types.begin(); it != tile_types.end(); ++it) {
        std::vector<Tile *> & vec = it->second;
        // Do instanced draw call on first tile
        vec[0]->c_draw_instanced(shader, scene_info); // do not pass in instance matrix because it doesn't change
    }

    // Draw all constructs by calling base class function.
    GameObject::c_draw(shader, scene_info);
}

void Grid::setup_listeners() {

    // When moving selection during build phase
    events::build::build_grid_move_event.connect([&](Direction dir) {
        move_selection(dir);
        update_selection();
        events::build::hover_position_updated_event(hover_col, hover_row);
    });

    events::build::build_grid_place_event.connect([&]() {
        proto::Construct c;
        c.set_type(selected);
        c.set_x(hover->getX());
        c.set_z(hover->getZ());
        events::build::request_build_event(c);
    });

    events::build::construct_selected_event.connect([&](ConstructType type) {
        selected = type;

        // Change preview to this construct type.
        preview.set_construct_type(type);
        children.push_back(&preview);
        // Update preview position based on hover position
        update_selection();
    });

    events::build::select_grid_return_event.connect([&]() {
        // Remove preview from children
        remove_child(&preview);
    });

    events::build::try_build_event.connect([&](proto::Construct& c) {
        bool permitted = verify_build(static_cast<ConstructType>(c.type()), c.x(), c.z());
        c.set_status(permitted);
        // Build the construct locally on the server, without graphics.
        // A build is permitted if the desired tile is buildable, e.g. not a wall and has no existing construct.
        if (permitted) {
            Construct* built = build(static_cast<ConstructType>(c.type()), c.x(), c.z(), false);
            if (built) {
                c.set_id(built->get_id());
            }
        }

        events::build::respond_build_event(c);
    });

    events::build::update_build_event.connect([&](proto::Construct& c) {
        // Build on the client, with graphics.
        build(static_cast<ConstructType>(c.type()), c.x(), c.z(), true, c.id());
    });

    events::dungeon::place_goal_event.connect([&]() {
        remove_goal();
        place_goal(glm::vec3(0.0f),20);
    });

    events::dungeon::spawn_existing_goal_event.connect([&](int x, int z, int id) {
        std::unique_lock<std::mutex> lock(goal_mutex);
        if (goal) {
            auto position = std::find(children.begin(), children.end(), goal);
            if (position != children.end())
                children.erase(position);
            goal = nullptr;
        }
        place_existing_goal(x, z, id);
    });

    events::build::end_build_event.connect([&]() {
        // Removes any existing preview when leaving build phase.
        remove_child(&preview);
    });
}

bool Grid::verify_build(ConstructType type, int col, int row) {
    Tile* candidate = grid[row][col];
    if (type == REMOVE) {
        return candidate->get_construct() != nullptr;
    }

    return candidate->buildable;
}

Construct* Grid::build(ConstructType type, int col, int row, bool graphical, int id) {
    Tile* candidate = grid[row][col];
    Construct* to_add = nullptr;

    switch (type) {
    case CHEST: {
        to_add = graphical ? new Chest(col, row, id) : new Chest(col, row);
        if (graphical) {
            to_add->setup_model();
            children.push_back(to_add);
            candidate->set_construct(to_add);
        } else {
            candidate->buildable = false;
            candidate->set_construct(to_add);
        }
        break;
    }
    case REMOVE: {
        // TODO: Move destructor to construct's destructor.
        if (candidate->get_construct() != nullptr) {
            if (!graphical) {
                events::remove_rigidbody_event(dynamic_cast<GameObject*>(candidate->get_construct()));
                candidate->buildable = true;
                candidate->set_construct(nullptr);
            }
            else {
                remove_child(candidate->get_construct());
                candidate->set_construct(nullptr);
            }
        }
        break;
    }
    default:
        break;
    }

    return to_add;
}

void Grid::move_selection(Direction d) {
    switch (d) {
    case UP:
        hover_row = glm::max(0, hover_row - 1);
        break;
    case RIGHT:
        hover_col = glm::min(width - 1, hover_col + 1);
        break;
    case DOWN:
        hover_row = glm::min(height - 1, hover_row + 1);
        break;
    case LEFT:
        hover_col = glm::max(0, hover_col - 1);
        break;
    default:
        break;
    }
}

// Updates Selected Tile
void Grid::update_selection() {
    hover = grid[hover_row][hover_col];

    // Move preview to selected tile
    preview.curr_preview->transform.set_position(hover_col, 0.0f, hover_row);
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
                    Tile *new_tile = make_tile(int_buf, c, r);
                    new_row.push_back(new_tile);
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

    // Push pointer back to tile_types map.
    tile_types[tile_id].push_back(new_tile);

    return new_tile;
}

void Grid::setup_model() {
    // Only setup model once for each tile type.
    // Use first element of each tile type vector to set up model.
    for (auto it = tile_types.begin(); it != tile_types.end(); ++it) {
        std::vector<Tile *> & vec = it->second;

        std::vector<glm::mat4> instance_matrix; // vector of all tile transforms
        for (Tile * t : vec) {
            // Insert this tile's transform world matrix into the vector
            instance_matrix.push_back(t->transform.get_world_mat());
        }

        vec[0]->setup_instanced_model(vec.size(), instance_matrix);
    }
}

void Grid::place_goal(glm::vec3 start, int min_dist) {
    // Goal will be in range of (min_dist, edge of map)
    int new_goal_x = rand() % width;
    int new_goal_z = rand() % height;

    // If not buildable or too close, find another
    while (!grid[new_goal_z][new_goal_x]->isBuildable() ||
        (abs(new_goal_x-start.x) + abs(new_goal_z-start.z) < min_dist)) {
        new_goal_x = rand() % width;
        new_goal_z = rand() % height;
    }

    Goal *new_goal = new Goal(new_goal_x, new_goal_z);

    grid[new_goal_z][new_goal_x]->set_construct(new_goal);
    goal = new_goal;
    goal_x = new_goal_x;
    goal_z = new_goal_z;
}

void Grid::place_existing_goal(int x, int z, int id) {
    goal = new Goal(x, z, id);

    goal->setup_model();
    children.push_back(goal);
}

void Grid::remove_goal() {
    //TODO destructor for goal
    if (goal) {
        GameObject::game_objects.erase(goal->get_id());
        grid[goal_z][goal_x]->set_construct(nullptr);
        events::remove_rigidbody_event(goal);
    }
}
