/*
* Grid Class:
*   This class holds a 2D array of Tile pointers that make up the map. Each Tile pointer
*   represents a coordinate on the map (row, col). The pointer points to a Tile object
*   which occupies that space (note that tiles can be more than a single coordinate big).
*
*   Also currently handles building constructs and adding them to the map.
*/

#pragma once

#include <vector>
#include <map>
#include <mutex>
#include "tile.h"
#include "game/construct_types.h"
#include "game/direction.h"
#include "construct_preview.h"

class Grid : public GameObject{
private:
    std::vector<std::vector<Tile *>> grid; // Uses grid[row][column], or row[z][x]
    std::map<int, std::vector<Tile *>> tile_types; // Mapping from tile type ID to tile pointers
    int width, height;
    int hover_row, hover_col;
    Tile *hover; // Currently selected tile
    ConstructType selected = ConstructType::REMOVE;
    Goal *goal;
    std::mutex goal_mutex; // In case a new goal is created before old one is removed.
    int goal_z, goal_x;

    ConstructPreview preview; // the currently selected construct as a transparent preview on the grid

    void setup_listeners();

    // For loading and creating map
    void load_map(const char *map_loc);
    Tile *make_tile(int tile_id, int col, int row);

public:
    Grid(const char *map_loc);

    void update_this() override {};
    void setup_model() override; // Loads tile models, only call this on client

    std::vector<std::vector<Tile *>> getGrid() { return grid; };

    // Returns true if this construct is allowed to be built.
    bool verify_build(ConstructType type, int col, int row);

    // Builds a construct at a location.
    Construct* build(ConstructType type, int col, int row, bool graphical, int id = -1);

    // For moving cursor on tile during build phase
    void move_selection(Direction d);

    // Loads tile models, only call this on client
    void graphical_setup();

    // Places goal with minimum distance from the start.
    // Distance calculated using manhattan distance(x diff + z diff)
    // Note: try not to use a high min dist
    void place_goal(glm::vec3 start, int min_dist);

    void place_existing_goal(int x, int z, int id);

    void remove_goal();

    void update_selection();

    // override draw for instanced rendering
    void draw(Shader *shader, SceneInfo &scene_info) override;
};
