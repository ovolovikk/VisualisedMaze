#ifndef CELL_HPP
#define CELL_HPP

#include <vector>

#define GRID_WIDTH 50
#define GRID_HEIGHT 50

enum class CellType
{
    Empty, Start, End, Visited, Path, Frontier
};

struct Cell
{
    CellType type = CellType::Empty;
    int x, y;
    // top, right, bottom, left
    bool walls[4] = {true, true, true, true};

    bool visited = false;
    Cell* parent = nullptr;
    std::vector<Cell*> neighbours;
};

#endif // CELL_HPP
