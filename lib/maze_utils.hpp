#ifndef MAZE_UTILS_HPP
#define MAZE_UTILS_HPP

#include "cell.hpp"
#include <vector>
#include <random>

extern std::mt19937 g;

bool is_maze_generated(const std::vector<std::vector<Cell>>& cells);
void cells_reset(std::vector<std::vector<Cell>>& cells);
void removeWalls(Cell& a, Cell& b);
std::vector<Cell*> get_neighbours(Cell* current, std::vector<std::vector<Cell>>& cells, bool get_visited);
std::vector<Cell*> get_neighbours(Cell* current, std::vector<std::vector<Cell>>& cells);
void add_neighbours_to_frontier(Cell* cell, std::vector<std::vector<Cell>>& cells, std::vector<Cell*>& frontier);

#endif // MAZE_UTILS_HPP
