#include "algorithms.hpp"

#include <iostream>
#include <stack>
#include <queue>
#include <random>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>

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
    std::random_device rd;
    std::mt19937 g(rd());

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

        std::vector<Cell*> unvisited_neighbours;
        int cx = current->x;
        int cy = current->y;

        if (cy > 0 && !cells[cx][cy-1].visited) unvisited_neighbours.push_back(&cells[cx][cy-1]);
        if (cx < GRID_WIDTH-1 && !cells[cx+1][cy].visited) unvisited_neighbours.push_back(&cells[cx+1][cy]);
        if (cy < GRID_HEIGHT-1 && !cells[cx][cy+1].visited) unvisited_neighbours.push_back(&cells[cx][cy+1]);
        if (cx > 0 && !cells[cx-1][cy].visited) unvisited_neighbours.push_back(&cells[cx-1][cy]);
        
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

    std::random_device rd;
    std::mt19937 g(rd());
    std::vector<Cell*> frontier;

    if(start_cell == nullptr)
    {
        start_cell = &cells[g() % GRID_WIDTH][g() % GRID_HEIGHT];
    }
    
    start_cell->visited = true;
    start_cell->type = CellType::Path;
    
    int sx = start_cell->x;
    int sy = start_cell->y;
    if (sy > 0 && !cells[sx][sy-1].visited) {
        cells[sx][sy-1].visited = true;
        frontier.push_back(&cells[sx][sy-1]);
    }
    if (sx < GRID_WIDTH-1 && !cells[sx+1][sy].visited) {
        cells[sx+1][sy].visited = true;
        frontier.push_back(&cells[sx+1][sy]);
    }
    if (sy < GRID_HEIGHT-1 && !cells[sx][sy+1].visited) {
        cells[sx][sy+1].visited = true;
        frontier.push_back(&cells[sx][sy+1]);
    }
    if (sx > 0 && !cells[sx-1][sy].visited) {
        cells[sx-1][sy].visited = true;
        frontier.push_back(&cells[sx-1][sy]);
    }

    while(!frontier.empty() && is_running)
    {
        int rand_index = g() % frontier.size();
        Cell* current = frontier[rand_index];
        frontier.erase(frontier.begin() + rand_index);

        current->type = CellType::Path;
        std::this_thread::sleep_for(std::chrono::milliseconds(15));

        std::vector<Cell*> visited_neighbours;
        int cx = current->x;
        int cy = current->y;
        if (cy > 0 && cells[cx][cy-1].visited) visited_neighbours.push_back(&cells[cx][cy-1]);
        if (cx < GRID_WIDTH-1 && cells[cx+1][cy].visited) visited_neighbours.push_back(&cells[cx+1][cy]);
        if (cy < GRID_HEIGHT-1 && cells[cx][cy+1].visited) visited_neighbours.push_back(&cells[cx][cy+1]);
        if (cx > 0 && cells[cx-1][cy].visited) visited_neighbours.push_back(&cells[cx-1][cy]);

        if (!visited_neighbours.empty()) {
            Cell* chosen_neighbour = visited_neighbours[g() % visited_neighbours.size()];
            removeWalls(*current, *chosen_neighbour);
        }

        if (cy > 0 && !cells[cx][cy-1].visited) {
            cells[cx][cy-1].visited = true;
            cells[cx][cy-1].type = CellType::Path;
            frontier.push_back(&cells[cx][cy-1]);
        }
        if (cx < GRID_WIDTH-1 && !cells[cx+1][cy].visited) {
            cells[cx+1][cy].visited = true;
            cells[cx+1][cy].type = CellType::Path;
            frontier.push_back(&cells[cx+1][cy]);
        }
        if (cy < GRID_HEIGHT-1 && !cells[cx][cy+1].visited) {
            cells[cx][cy+1].visited = true;
            cells[cx][cy+1].type = CellType::Path;
            frontier.push_back(&cells[cx][cy+1]);
        }
        if (cx > 0 && !cells[cx-1][cy].visited) {
            cells[cx-1][cy].visited = true;
            cells[cx-1][cy].type = CellType::Path;
            frontier.push_back(&cells[cx-1][cy]);
        }
    }

    for(auto& row : cells) for(auto& cell : row) {
        if (cell.type == CellType::Path) {
            cell.type = CellType::Visited;
        }
    }

    is_running = false;
}