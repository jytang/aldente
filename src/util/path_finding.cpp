#include "path_finding.h"

#include <Queue>
#include <tuple>

std::vector<std::vector<Tile *>> PathFinding::grid;

// Given a start and end position, returns the direction for the object to go towards in order
// to find the shortest path. (using A*)
glm::vec3 PathFinding::find_path(glm::vec3 start_pos, glm::vec3 end_pos) {
    // Start and End Position
    int start_x = (int)start_pos.x;
    int start_z = (int)start_pos.z;
    int end_x = (int)end_pos.x;
    int end_z = (int)end_pos.z;

    // Setting up Priority Queue
    // First two numbers of tuple represent X and Z coord of tile, and last number is Cost to go there./
    // Lambda compares cost for sorting
    auto cmp = [](std::tuple<int, int, int> n1, std::tuple<int, int, int> n2) { return (std::get<2>(n1) > std::get<2>(n2)); };
    std::priority_queue<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>, decltype(cmp)> to_explore(cmp);

    // Make start and goal pairs
    std::pair<int, int> start = std::pair<int, int>(start_x, start_z);
    std::pair<int, int> goal = std::pair<int, int>(end_x, end_z);
    to_explore.push(std::tuple<int, int, int>(start.first, start.second, 0));

    // Neighbor Offsets
    int next_x[4] = { -1, 0, 0, 1 };
    int next_z[4] = { 0, 1, -1, 0 };

    // For knowing the path
    std::map<std::pair<int, int>, std::pair<int, int>> came_from;
    came_from[start] = start;

    // For knowing the cost of a certain location
    std::map<std::pair<int, int>, int> cost_so_far;
    cost_so_far[start] = 0;

    // Run through A* Algorithm
    while (!to_explore.empty()) {
        std::tuple<int, int, int> current = to_explore.top();
        to_explore.pop();

        std::pair<int, int> current_loc = std::pair<int, int>(std::get<0>(current), std::get<1>(current));
        int current_cost = std::get<2>(current);

        if (current_loc.first == end_x && current_loc.second == end_z) {
            break;
        }

        for (int i = 0; i < 4; i++) {
            std::pair<int, int> next = std::make_pair(current_loc.first + next_x[i], current_loc.second + next_z[i]);
            int new_cost = cost_so_far[current_loc] + 1;

            if (!cost_so_far.count(next) || new_cost < cost_so_far[next]) {
                cost_so_far[next] = new_cost;
                int priority = new_cost + heuristic(next, goal);
                to_explore.push(std::tuple<int, int, int>(next.first, next.second, priority));
                came_from[next] = current_loc;
            }
        }
    }

    // Backtrack to find the next path needed
    if (came_from.count(goal)) {
        std::pair<int, int> current_loc = goal;
        while (came_from[current_loc] != start) {
            current_loc = came_from[current_loc];
        }

        // Find direction to face to go follow the path
        glm::vec3 dir = glm::vec3(current_loc.first - start.first, 0, current_loc.second - start.second);
        return dir;
    }
    else // Path not found
        return glm::vec3(0.0f);
}

// For determining heuristic distance
int PathFinding::heuristic(std::pair<int, int> loc_a, std::pair<int, int> loc_b) {
    return abs(loc_a.first - loc_b.first) + abs(loc_a.second - loc_b.second);
}