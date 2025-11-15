#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "cell.hpp"
#include <vector>
#include <atomic>

bool is_maze_generated(const std::vector<std::vector<Cell>>& cells);
void cells_reset(std::vector<std::vector<Cell>>& cells);
void dfs_maze(std::vector<std::vector<Cell>>& cells, Cell* start_cell, Cell* end_cell, std::atomic<bool>* is_running);
void bfs_maze(std::vector<std::vector<Cell>>& cells, Cell* start_cell, Cell* end_cell, std::atomic<bool>* is_running);
void dijkstra_solve(std::vector<std::vector<Cell>>& cells, Cell* start_cell, Cell* end_cell, std::atomic<bool>* is_running);

#endif // ALGORITHMS_HPP
