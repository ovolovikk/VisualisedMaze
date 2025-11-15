#ifndef MAZE_CREATORS_HPP
#define MAZE_CREATORS_HPP

#include "cell.hpp"
#include <vector>
#include <atomic>

void create_dfs_maze(std::vector<std::vector<Cell>>& cells, Cell* start_cell, Cell* end_cell, std::atomic<bool>* is_running);
void create_bfs_maze(std::vector<std::vector<Cell>>& cells, Cell* start_cell, Cell* end_cell, std::atomic<bool>* is_running);

#endif // MAZE_CREATORS_HPP
