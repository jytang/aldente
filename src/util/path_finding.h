#pragma once

#include "glm/glm.hpp"
#include <vector>
#include "game_objects/tile.h"

class PathFinding {
public:
    static void set_grid(std::vector<std::vector<Tile *>> new_grid) { grid = new_grid; };

    static glm::vec3 find_path(glm::vec3 start_pos, glm::vec3 end_pos);
private:
    static std::vector<std::vector<Tile *>> grid; // Uses grid[row][column], copy of grid
};