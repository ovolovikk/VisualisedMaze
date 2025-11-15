#include "maze_creators.hpp"
#include "maze_utils.hpp"

#include <stack>
#include <algorithm>
#include <thread>
#include <chrono>

void create_dfs_maze(std::vector<std::vector<Cell>>& cells, Cell* start_cell, Cell* end_cell, std::atomic<bool>* is_running)
{
    cells_reset(cells);
    std::stack<Cell*> stack;

    if(start_cell == nullptr)
    {
        start_cell = &cells[0][0];
    }
    else
    {
        start_cell->type = CellType::Start;
    }
    if (end_cell)
    {
        end_cell->type = CellType::End;
    }

    start_cell->visited = true;
    if(is_running) start_cell->type = CellType::Path;
    stack.push(start_cell);

    while(!stack.empty())
    {
        if (is_running && !is_running->load()) break;

        Cell* current = stack.top();
        std::vector<Cell*> unvisited_neighbours = get_neighbours(current, cells, false);
        
        if(!unvisited_neighbours.empty())
        {
            std::shuffle(unvisited_neighbours.begin(), unvisited_neighbours.end(), g);
            Cell* chosen = unvisited_neighbours.front();
            
            removeWalls(*current, *chosen);

            chosen->visited = true;
            if(is_running) {
                chosen->type = CellType::Path;
                std::this_thread::sleep_for(std::chrono::milliseconds(15));
            }
            stack.push(chosen);
        }
        else
        {
            if(is_running) current->type = CellType::Visited;
            stack.pop();
        }
    }

    if(is_running) *is_running = false;
}

void create_bfs_maze(std::vector<std::vector<Cell>>& cells, Cell* start_cell, Cell* end_cell, std::atomic<bool>* is_running)
{
    cells_reset(cells);
    std::vector<Cell*> frontier;

    if(start_cell == nullptr)
    {
        start_cell = &cells[0][0];
    }
    else
    {
        start_cell->type = CellType::Start;
    }
    if (end_cell)
    {
        end_cell->type = CellType::End;
    }
    
    start_cell->visited = true;
    if(is_running) start_cell->type = CellType::Visited;
    
    add_neighbours_to_frontier(start_cell, cells, frontier);

    while(!frontier.empty())
    {
        if (is_running && !is_running->load()) break;

        int rand_index = g() % frontier.size();
        Cell* current = frontier[rand_index];
        frontier.erase(frontier.begin() + rand_index);

        if(is_running) std::this_thread::sleep_for(std::chrono::milliseconds(5));

        std::vector<Cell*> visited_neighbours = get_neighbours(current, cells, true);

        if (!visited_neighbours.empty()) {
            Cell* chosen_neighbour = visited_neighbours[g() % visited_neighbours.size()];
            removeWalls(*current, *chosen_neighbour);
        }

        current->visited = true;
        if(is_running) current->type = CellType::Visited;

        add_neighbours_to_frontier(current, cells, frontier);
    }

    if(is_running) {
        for(auto& row : cells) for(auto& cell : row) {
            if (cell.type == CellType::Path || cell.type == CellType::Frontier) {
                cell.type = CellType::Empty;
            }
        }
    }

    if(is_running) *is_running = false;
}
