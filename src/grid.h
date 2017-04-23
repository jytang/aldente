/*
* Grid Class:
*   This class holds a 2D array of Tile pointers that make up the map. Each Tile pointer
*   represents a coordinate on the map (x,z). We use z because we want the map to be parallel to 
*   the "ground". The pointer points to a Tile object which occupies that space (note that tiles
*   can be more than a single coordinate big). 
*/

#pragma once

#include <vector>
#include "tile.h"

class Grid {
private:
    std::vector<std::vector<Tile *>> grid;
    int width, height;

    Tile *hover;

public:
    Grid(int w, int h);

    ~Grid();

    std::vector<std::vector<Tile *>> getGrid() { return grid; };
};