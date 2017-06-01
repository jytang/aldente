#include "path_finding.h"

#include <Queue>

// Compare function for priority queue
struct comp {
    bool compare_func(std::pair<std::pair<int, int>, int> n1, std::pair<std::pair<int, int>, int> n2) {
        return n1.second > n2.second;
    }
};

// Given a start and end position, returns the direction for the object to go towards in order
// to find the shortest path. (using A*)
glm::vec3 PathFinding::find_path(glm::vec3 start_pos, glm::vec3 end_pos) {
    // Start and End Position
    int start_x = (int)start_pos.x;
    int start_z = (int)start_pos.z;
    int end_x = (int)end_pos.x;
    int end_z = (int)end_pos.z;

    // Setting up Priority Queue
    std::priority_queue<std::pair<std::pair<int, int>, int>, std::vector<std::pair<std::pair<int, int>, int>>, comp> to_explore;
    std::pair<int, int> start = std::make_pair(start_x, start_z);
    std::pair<int, int> goal = std::make_pair(end_x, end_z);
    to_explore.push(std::make_pair(start, 0));

    // Neighbor Offsets
    int next_x[4] = { -1, 0, 0, 1 };
    int next_z[4] = { 0, 1, -1, 0 };

    std::map<std::pair<int, int>, std::pair<int, int>> came_from;
    std::map<std::pair<int, int>, int> cost_so_far;
    came_from[start] = start;
    cost_so_far[start] = 0;

    while (!to_explore.empty()) {
        std::pair<std::pair<int, int>, int> current = to_explore.top();
        to_explore.pop();

        std::pair<int, int> current_loc = current.first;
        int current_cost = current.second;

        if (current_loc.first == end_x && current_loc.second == end_z) {
            break;
        }

        for (int i = 0; i < 4; i++) {
            std::pair<int, int> next = std::make_pair(current_loc.first + next_x[i], current_loc.second + next_z[i]);
            int new_cost = cost_so_far[current_loc] + 1;

            if (!cost_so_far.count(next) || new_cost < cost_so_far[next]) {
                cost_so_far[next] = new_cost;
                int priority = new_cost + heuristic(next, goal);
                to_explore.push(std::make_pair(next, priority));
                came_from[next] = current;
            }
        }
    }

    if (came_from.count(goal)) {
        std::pair<int, int> current_loc = goal;
        while (came_from[current_loc] != start) {
            current_loc = came_from[current_loc];
        }

        // Find direction to face to go follow the path
        glm::vec3 dir = glm::vec3(current_loc.first - start.first, 0, current_loc.second - start.second);
        return dir;
    }
}

int PathFinding::heuristic(std::pair<int, int> loc_a, std::pair<int, int> loc_b) {
    return abs(loc_a.first - loc_b.first) + abs(loc_a.second - loc_b.second);
}