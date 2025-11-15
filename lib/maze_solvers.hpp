#ifndef MAZE_SOLVERS_HPP
#define MAZE_SOLVERS_HPP

#include "cell.hpp"
#include <vector>
#include <atomic>

void dijkstra_solve(std::vector<std::vector<Cell>>& cells, Cell* start_cell, Cell* end_cell, std::atomic<bool>* is_running);

#endif // MAZE_SOLVERS_HPP
