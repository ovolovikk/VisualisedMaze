#include "algorithms.hpp"

#include <iostream>
#include <stack>
#include <random>
#include <vector>
#include <algorithm>

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

void dfs(std::vector<std::vector<Cell>>& cells, Cell* start_cell)
{
    for(auto& row : cells) for(auto& cell : row)
    {
        cell.visited = false;
        for(int i=0; i<4; ++i) cell.walls[i] = true;
    }

    std::stack<Cell*> stack;
    std::random_device rd;
    std::mt19937 g(rd());

    if(start_cell == nullptr)
    {
        start_cell = &cells[0][0];
    }

    start_cell->visited = true;
    stack.push(start_cell);

    while(!stack.empty())
    {
        Cell* current = stack.top();
        stack.pop();

        std::vector<Cell*> unvisited_neighbours;
        int cx = current->x;
        int cy = current->y;

        if (cy > 0 && !cells[cx][cy-1].visited) unvisited_neighbours.push_back(&cells[cx][cy-1]);
        if (cx < GRID_WIDTH-1 && !cells[cx+1][cy].visited) unvisited_neighbours.push_back(&cells[cx+1][cy]);
        if (cy < GRID_HEIGHT-1 && !cells[cx][cy+1].visited) unvisited_neighbours.push_back(&cells[cx][cy+1]);
        if (cx > 0 && !cells[cx-1][cy].visited) unvisited_neighbours.push_back(&cells[cx-1][cy]);

        if(!unvisited_neighbours.empty()) {
            stack.push(current);
            std::shuffle(unvisited_neighbours.begin(), unvisited_neighbours.end(), g);
            Cell* chosen = unvisited_neighbours.front();
            
            removeWalls(*current, *chosen);

            chosen->visited = true;
            stack.push(chosen);
        }
    }
}