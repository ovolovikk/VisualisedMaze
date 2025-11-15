#include "algorithms.hpp"

#include <iostream>
#include <stack>
#include <queue>
#include <random>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>

static std::random_device rd;
static std::mt19937 g(rd());

static std::vector<Cell*> get_neighbours(Cell* current, std::vector<std::vector<Cell>>& cells, bool get_visited)
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

static void add_neighbours_to_frontier(Cell* cell, std::vector<std::vector<Cell>>& cells, std::vector<Cell*>& frontier)
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

void cells_reset(std::vector<std::vector<Cell>>& cells)
{
    for(auto& row : cells) for(auto& cell : row)
    {
        cell.visited = false;
        for(int i=0; i<4; ++i) cell.walls[i] = true;
        if (cell.type == CellType::Path || cell.type == CellType::Visited) {
            cell.type = CellType::Empty;
        }
    }
}

void dfs_maze(std::vector<std::vector<Cell>>& cells, Cell* start_cell, std::atomic<bool>& is_running)
{
    cells_reset(cells);

    std::stack<Cell*> stack;

    if(start_cell == nullptr)
    {
        start_cell = &cells[0][0];
    }

    start_cell->visited = true;
    start_cell->type = CellType::Path;
    stack.push(start_cell);

    while(!stack.empty() && is_running)
    {
        Cell* current = stack.top();

        std::vector<Cell*> unvisited_neighbours = get_neighbours(current, cells, false);
        
        if(!unvisited_neighbours.empty())
        {
            std::shuffle(unvisited_neighbours.begin(), unvisited_neighbours.end(), g);
            Cell* chosen = unvisited_neighbours.front();
            
            removeWalls(*current, *chosen);

            chosen->visited = true;
            chosen->type = CellType::Path;
            stack.push(chosen);
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
        }
        else
        {
            current->type = CellType::Visited;
            stack.pop();
        }
    }

    is_running = false;
}

void bfs_maze(std::vector<std::vector<Cell>>& cells, Cell* start_cell, std::atomic<bool>& is_running)
{
    cells_reset(cells);

    std::vector<Cell*> frontier;

    if(start_cell == nullptr)
    {
        start_cell = &cells[0][0];
    }
    
    start_cell->visited = true;
    start_cell->type = CellType::Visited;
    
    add_neighbours_to_frontier(start_cell, cells, frontier);

    while(!frontier.empty() && is_running)
    {
        int rand_index = g() % frontier.size();
        Cell* current = frontier[rand_index];
        frontier.erase(frontier.begin() + rand_index);

        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        std::vector<Cell*> visited_neighbours = get_neighbours(current, cells, true);

        if (!visited_neighbours.empty()) {
            Cell* chosen_neighbour = visited_neighbours[g() % visited_neighbours.size()];
            removeWalls(*current, *chosen_neighbour);
        }

        current->visited = true;
        current->type = CellType::Visited;

        add_neighbours_to_frontier(current, cells, frontier);
    }

    for(auto& row : cells) for(auto& cell : row) {
        if (cell.type == CellType::Path || cell.type == CellType::Frontier) {
            cell.type = CellType::Empty;
        }
    }

    is_running = false;
}