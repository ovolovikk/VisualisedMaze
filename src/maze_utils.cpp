#include "maze_utils.hpp"
#include <algorithm>

static std::random_device rd;
std::mt19937 g(rd());

bool is_maze_generated(const std::vector<std::vector<Cell>>& cells)
{
    for(const auto& row : cells) {
        for(const auto& cell : row) {
            if (!cell.walls[0] || !cell.walls[1] || !cell.walls[2] || !cell.walls[3]) {
                return true;
            }
        }
    }
    return false;
}

void cells_reset(std::vector<std::vector<Cell>>& cells)
{
    for(auto& row : cells) for(auto& cell : row)
    {
        cell.visited = false;
        for(int i=0; i<4; ++i) cell.walls[i] = true;
        cell.type = CellType::Empty;
        cell.parent = nullptr;
    }
}

void removeWalls(Cell& a, Cell& b)
{
    int x = a.x - b.x;
    if (x == -1) {
        a.walls[1] = false;
        b.walls[3] = false;
    } 
    else if (x == 1) {
        a.walls[3] = false;
        b.walls[1] = false;
    }

    int y = a.y - b.y;
    if (y == -1) {
        a.walls[2] = false;
        b.walls[0] = false;
    }
    else if (y == 1) {
        a.walls[0] = false;
        b.walls[2] = false;
    }
}

std::vector<Cell*> get_neighbours(Cell* current, std::vector<std::vector<Cell>>& cells, bool get_visited)
{
    std::vector<Cell*> neighbours;
    int cx = current->x;
    int cy = current->y;

    if (cy > 0 && cells[cx][cy-1].visited == get_visited) {
        neighbours.push_back(&cells[cx][cy-1]);
    }
    if (cx < GRID_WIDTH - 1 && cells[cx+1][cy].visited == get_visited) {
        neighbours.push_back(&cells[cx+1][cy]);
    }
    if (cy < GRID_HEIGHT - 1 && cells[cx][cy+1].visited == get_visited) {
        neighbours.push_back(&cells[cx][cy+1]);
    }
    if (cx > 0 && cells[cx-1][cy].visited == get_visited) {
        neighbours.push_back(&cells[cx-1][cy]);
    }
    return neighbours;
}

std::vector<Cell*> get_neighbours(Cell* current, std::vector<std::vector<Cell>>& cells)
{
    std::vector<Cell*> neighbours;
    int cx = current->x;
    int cy = current->y;

    if (!current->walls[0] && !cells[cx][cy-1].visited) {
        neighbours.push_back(&cells[cx][cy-1]);
    }
    if (!current->walls[1] && !cells[cx+1][cy].visited) {
        neighbours.push_back(&cells[cx+1][cy]);
    }
    if (!current->walls[2] && !cells[cx][cy+1].visited) {
        neighbours.push_back(&cells[cx][cy+1]);
    }
    if (!current->walls[3] && !cells[cx-1][cy].visited) {
        neighbours.push_back(&cells[cx-1][cy]);
    }
    return neighbours;
}

void add_neighbours_to_frontier(Cell* cell, std::vector<std::vector<Cell>>& cells, std::vector<Cell*>& frontier)
{
    int cx = cell->x;
    int cy = cell->y;

    auto add_if_valid = [&](int x, int y) {
        if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT && !cells[x][y].visited) {
            bool in_frontier = false;
            for(Cell* f_cell : frontier) if(f_cell == &cells[x][y]) in_frontier = true;
            if(!in_frontier) {
                cells[x][y].type = CellType::Frontier;
                frontier.push_back(&cells[x][y]);
            }
        }
    };

    add_if_valid(cx, cy - 1);
    add_if_valid(cx + 1, cy);
    add_if_valid(cx, cy + 1);
    add_if_valid(cx - 1, cy);
}
