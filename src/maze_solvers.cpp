#include "maze_solvers.hpp"
#include "maze_utils.hpp"

#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>
#include <climits>

void dijkstra_solve(std::vector<std::vector<Cell>>& cells, Cell* start_cell, Cell* end_cell, std::atomic<bool>* is_running)
{
    if (!start_cell || !end_cell) {
        if (is_running) *is_running = false;
        return;
    }

    for(auto& row : cells) for(auto& cell : row) {
        cell.visited = false;
        cell.parent = nullptr;
        if (cell.type == CellType::Path || cell.type == CellType::Visited || cell.type == CellType::Frontier) {
            if (cell.type != CellType::Start && cell.type != CellType::End)
                cell.type = CellType::Empty;
        }
    }

    std::vector<Cell*> unvisited;
    std::vector<std::vector<int>> distance(GRID_WIDTH, std::vector<int>(GRID_HEIGHT, INT_MAX));

    distance[start_cell->x][start_cell->y] = 0;
    unvisited.push_back(start_cell);

    Cell* current = nullptr;
    while(!unvisited.empty()) {
        if (is_running && !is_running->load()) break;

        std::sort(unvisited.begin(), unvisited.end(), [&](Cell* a, Cell* b) {
            return distance[a->x][a->y] > distance[b->x][b->y];
        });
        current = unvisited.back();
        unvisited.pop_back();

        if (current->type != CellType::Start && current->type != CellType::End) {
            current->type = CellType::Path;
        }
        if (is_running) std::this_thread::sleep_for(std::chrono::milliseconds(5));

        if (current == end_cell) break;

        if (current->type != CellType::Start && current->type != CellType::End) {
            current->type = CellType::Visited;
        }
        current->visited = true;

        std::vector<Cell*> traversable_neighbours = get_neighbours(current, cells);

        for (Cell* neighbour : traversable_neighbours) {
            int new_dist = distance[current->x][current->y] + 1;
            if (new_dist < distance[neighbour->x][neighbour->y]) {
                distance[neighbour->x][neighbour->y] = new_dist;
                neighbour->parent = current;
                
                bool in_unvisited = false;
                for(Cell* u_cell : unvisited) if(u_cell == neighbour) in_unvisited = true;
                if(!in_unvisited) {
                    unvisited.push_back(neighbour);
                    if(neighbour->type != CellType::End) neighbour->type = CellType::Frontier;
                }
            }
        }
    }

    for(auto& row : cells) for(auto& cell : row) {
        if (cell.type == CellType::Visited || cell.type == CellType::Frontier) {
            if (cell.type != CellType::Start && cell.type != CellType::End) {
                cell.type = CellType::Empty;
            }
        }
    }

    if (current == end_cell) {
        while(current->parent != nullptr) {
            current = current->parent;
            if (current->type != CellType::Start) {
                current->type = CellType::Path;
                if (is_running) std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }

    if (is_running) *is_running = false;
}
