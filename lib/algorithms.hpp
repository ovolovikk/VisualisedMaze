#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "cell.hpp"
#include <vector>

void removeWalls(Cell& a, Cell& b);
void dfs(std::vector<std::vector<Cell>>& cells, Cell* start_cell = nullptr);

#endif // ALGORITHMS_HPP
