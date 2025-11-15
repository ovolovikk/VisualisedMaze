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

bool is_maze_generated(const std::vector<std::vector<Cell>>& cells)
{
    for(const auto& row : cells) {
        for(const auto& cell : row) {
            // If a cell doesn't have all 4 walls, a maze has been started.
            if (!cell.walls[0] || !cell.walls[1] || !cell.walls[2] || !cell.walls[3]) {
                return true;
            }
        }
    }
    return false;
}

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

static std::vector<Cell*> get_neighbours(Cell* current, std::vector<std::vector<Cell>>& cells)
{
    std::vector<Cell*> neighbours;
    int cx = current->x;
    int cy = current->y;

    // top
    if (!current->walls[0] && !cells[cx][cy-1].visited) {
        neighbours.push_back(&cells[cx][cy-1]);
    }
    // right
    if (!current->walls[1] && !cells[cx+1][cy].visited) {
        neighbours.push_back(&cells[cx+1][cy]);
    }
    // bottom
    if (!current->walls[2] && !cells[cx][cy+1].visited) {
        neighbours.push_back(&cells[cx][cy+1]);
    }
    // left
    if (!current->walls[3] && !cells[cx-1][cy].visited) {
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
        cell.type = CellType::Empty;
        cell.parent = nullptr;
    }
}

void dfs_maze(std::vector<std::vector<Cell>>& cells, Cell* start_cell, Cell* end_cell, std::atomic<bool>* is_running)
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

void bfs_maze(std::vector<std::vector<Cell>>& cells, Cell* start_cell, Cell* end_cell, std::atomic<bool>* is_running)
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

void dijkstra_solve(std::vector<std::vector<Cell>>& cells, Cell* start_cell, Cell* end_cell, std::atomic<bool>* is_running)
{
    if (!start_cell || !end_cell) {
        if (is_running) *is_running = false;
        return;
    }

    for(auto& row : cells) for(auto& cell : row) {
        cell.visited = false;
        cell.parent = nullptr;
        if (cell.type == CellType::Path || cell.type == CellType::Visited || cell.type == CellType::Frontier) {
            if (cell.type != CellType::Start && cell.type != CellType::End)
                cell.type = CellType::Empty;
        }
    }

    std::vector<Cell*> unvisited;
    std::vector<std::vector<int>> distance(GRID_WIDTH, std::vector<int>(GRID_HEIGHT, INT_MAX));

    distance[start_cell->x][start_cell->y] = 0;
    unvisited.push_back(start_cell);

    Cell* current = nullptr;
    while(!unvisited.empty()) {
        if (is_running && !is_running->load()) break;

        std::sort(unvisited.begin(), unvisited.end(), [&](Cell* a, Cell* b) {
            return distance[a->x][a->y] > distance[b->x][b->y];
        });
        current = unvisited.back();
        unvisited.pop_back();

        if (current->type != CellType::Start && current->type != CellType::End) {
            current->type = CellType::Path;
        }
        if (is_running) std::this_thread::sleep_for(std::chrono::milliseconds(5));

        if (current == end_cell) break;

        if (current->type != CellType::Start && current->type != CellType::End) {
            current->type = CellType::Visited;
        }
        current->visited = true;

        std::vector<Cell*> traversable_neighbours = get_neighbours(current, cells);

        for (Cell* neighbour : traversable_neighbours) {
            int new_dist = distance[current->x][current->y] + 1;
            if (new_dist < distance[neighbour->x][neighbour->y]) {
                distance[neighbour->x][neighbour->y] = new_dist;
                neighbour->parent = current;
                
                bool in_unvisited = false;
                for(Cell* u_cell : unvisited) if(u_cell == neighbour) in_unvisited = true;
                if(!in_unvisited) {
                    unvisited.push_back(neighbour);
                    if(neighbour->type != CellType::End) neighbour->type = CellType::Frontier;
                }
            }
        }
    }

    for(auto& row : cells) for(auto& cell : row) {
        if (cell.type == CellType::Visited || cell.type == CellType::Frontier) {
            if (cell.type != CellType::Start && cell.type != CellType::End) {
                cell.type = CellType::Empty;
            }
        }
    }

    if (current == end_cell) {
        while(current->parent != nullptr) {
            current = current->parent;
            if (current->type != CellType::Start) {
                current->type = CellType::Path;
                if (is_running) std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }

    if (is_running) *is_running = false;
}