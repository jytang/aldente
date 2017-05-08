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
#include "tile.h"
#include "game/construct_types.h"
#include "game/direction.h"

class Grid : public GameObject{
private:
    std::vector<std::vector<Tile *>> grid; // Uses grid[row][column], or row[z][x]
    int width, height;
    int hover_row, hover_col;
    Tile *hover; // Currently selected tile
    ConstructType selected = ConstructType::REMOVE;

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
    void build(ConstructType type, int col, int row, bool graphical);

    // For moving cursor on tile during build phase
    void move_selection(Direction d);
    void update_selection();
};