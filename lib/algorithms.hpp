#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "cell.hpp"
#include <vector>
#include <atomic>

void removeWalls(Cell& a, Cell& b);
void dfs_maze(std::vector<std::vector<Cell>>& cells, Cell* start_cell, std::atomic<bool>& is_running);
void bfs_maze(std::vector<std::vector<Cell>>& cells, Cell* start_cell, std::atomic<bool>& is_running);

#endif // ALGORITHMS_HPP
