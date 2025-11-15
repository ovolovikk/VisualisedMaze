#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <thread>
#include <atomic>

#include "maze_creators.hpp"
#include "maze_solvers.hpp"
#include "maze_utils.hpp"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960

void draw_grid(sf::RenderWindow& window, std::vector<std::vector<Cell>>& cells, float cell_size, float offsetX, float offsetY)
{
    sf::RectangleShape cell_shape(sf::Vector2f(cell_size, cell_size));
    sf::RectangleShape wall_shape;
    wall_shape.setFillColor(sf::Color::White);

    for(int i = 0; i < GRID_WIDTH; ++i) {
        for(int j = 0; j < GRID_HEIGHT; ++j) {
            Cell& current_cell = cells.at(i).at(j);
            
            cell_shape.setPosition(i * cell_size + offsetX, j * cell_size + offsetY);
            cell_shape.setSize({cell_size, cell_size});
            cell_shape.setOutlineThickness(0);

            switch (current_cell.type)
            {
                case CellType::Empty:
                    cell_shape.setFillColor(sf::Color::Black);
                    break;
                case CellType::Start:
                    cell_shape.setFillColor(sf::Color::Green);
                    break;
                case CellType::End:
                    cell_shape.setFillColor(sf::Color::Red);
                    break;
                case CellType::Visited:
                    cell_shape.setFillColor(sf::Color(0, 100, 100));
                    break;
                case CellType::Path:
                    cell_shape.setFillColor(sf::Color(128, 0, 0));
                    break;
                case CellType::Frontier:
                    cell_shape.setFillColor(sf::Color(0, 0, 128));
                    break;
                default:
                    cell_shape.setFillColor(sf::Color::Black);
            }
            window.draw(cell_shape);

            // walls
            float wall_thickness = 1.0f;
            float px = i * cell_size + offsetX;
            float py = j * cell_size + offsetY;

            if (current_cell.walls[0]) { // top
                wall_shape.setSize({cell_size, wall_thickness});
                wall_shape.setPosition(px, py);
                window.draw(wall_shape);
            }
            if (current_cell.walls[1]) { // right
                wall_shape.setSize({wall_thickness, cell_size});
                wall_shape.setPosition(px + cell_size - wall_thickness, py);
                window.draw(wall_shape);
            }
            if (current_cell.walls[2]) { // bottom
                wall_shape.setSize({cell_size, wall_thickness});
                wall_shape.setPosition(px, py + cell_size - wall_thickness);
                window.draw(wall_shape);
            }
            if (current_cell.walls[3]) { // left
                wall_shape.setSize({wall_thickness, cell_size});
                wall_shape.setPosition(px, py);
                window.draw(wall_shape);
            }
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML MazeVisualizer");
    std::vector<std::vector<Cell>> cells;
    Cell* start_cell_ptr = nullptr;
    Cell* end_cell_ptr = nullptr;

    std::thread algorithm_thread;
    std::atomic<bool> is_algorithm_running = false;
    bool needs_cleanup = false;

    float cell_width = static_cast<float>(WINDOW_WIDTH) / GRID_WIDTH;
    float cell_height = static_cast<float>(WINDOW_HEIGHT) / GRID_HEIGHT;
    float cell_size = std::min(cell_width, cell_height);

    const float grid_pixel_width = GRID_WIDTH * cell_size;
    const float grid_pixel_height = GRID_HEIGHT * cell_size;
    
    cells.resize(GRID_WIDTH, std::vector<Cell>(GRID_HEIGHT));
    for(int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            cells[i][j].x = i;
            cells[i][j].y = j;
        }
    }
    
    float offsetX = (window.getSize().x - grid_pixel_width) / 2.0f;
    float offsetY = (window.getSize().y - grid_pixel_height) / 2.0f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                is_algorithm_running = false;
                if (algorithm_thread.joinable()) algorithm_thread.join();
                window.close();
            }
            if (event.type == sf::Event::KeyReleased)
            {
                sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                    
                int gridX = static_cast<int>((mouse_pos.x - offsetX) / cell_size);
                int gridY = static_cast<int>((mouse_pos.y - offsetY) / cell_size);

                if(event.key.code == sf::Keyboard::D)
                {
                    if (!is_algorithm_running) {
                        is_algorithm_running = true;
                        needs_cleanup = true;
                        if (algorithm_thread.joinable()) algorithm_thread.join();
                        algorithm_thread = std::thread(create_dfs_maze, std::ref(cells), start_cell_ptr, end_cell_ptr, &is_algorithm_running);
                    }
                }

                if(event.key.code == sf::Keyboard::B)
                {
                    if (!is_algorithm_running) {
                        is_algorithm_running = true;
                        needs_cleanup = true;
                        if (algorithm_thread.joinable()) algorithm_thread.join();
                        algorithm_thread = std::thread(create_bfs_maze, std::ref(cells), start_cell_ptr, end_cell_ptr, &is_algorithm_running);
                    }
                }

                if(event.key.code == sf::Keyboard::R)
                {
                    if (!is_algorithm_running)
                    {
                        cells_reset(cells);
                        start_cell_ptr = nullptr;
                        end_cell_ptr = nullptr;
                    }
                }

                if(event.key.code == sf::Keyboard::K)
                {
                    if (!is_algorithm_running)
                    {
                        if (!start_cell_ptr) 
                        {
                            start_cell_ptr = &cells[0][0];
                            start_cell_ptr->type = CellType::Start;
                        }
                        if (!end_cell_ptr)
                        {
                            end_cell_ptr = &cells[GRID_WIDTH-1][GRID_HEIGHT-1];
                            end_cell_ptr->type = CellType::End;
                        }

                        is_algorithm_running = true;
                        if (algorithm_thread.joinable()) algorithm_thread.join();

                        auto solve_task = [&]() {
                            if (!is_maze_generated(cells)) {
                                create_bfs_maze(cells, start_cell_ptr, end_cell_ptr, nullptr);
                            }
                            dijkstra_solve(cells, start_cell_ptr, end_cell_ptr, &is_algorithm_running);
                        };
                        
                        algorithm_thread = std::thread(solve_task);
                    }
                }

                if(event.key.code == sf::Keyboard::S)
                {
                    if (gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT)
                    {
                        Cell& cell = cells.at(gridX).at(gridY);
                        if (cell.type != CellType::Start)
                        {
                            if (start_cell_ptr) {
                                start_cell_ptr->type = CellType::Empty;
                            }
                            cell.type = CellType::Start;
                            start_cell_ptr = &cell;
                        }
                        else if (cell.type == CellType::Start)
                        {
                            cell.type = CellType::Empty;
                            start_cell_ptr = nullptr;
                        }
                    }
                }
                if(event.key.code == sf::Keyboard::E)
                {
                    if (gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT)
                    {
                        Cell& cell = cells.at(gridX).at(gridY);
                        if (cell.type != CellType::End)
                        {
                            if (end_cell_ptr) {
                                end_cell_ptr->type = CellType::Empty;
                            }
                            cell.type = CellType::End;
                            end_cell_ptr = &cell;
                        }
                        else if (cell.type == CellType::End)
                        {
                            cell.type = CellType::Empty;
                            end_cell_ptr = nullptr;
                        }
                    }
                }
            }
                
        }

        if (needs_cleanup && !is_algorithm_running) {
            if (algorithm_thread.joinable()) algorithm_thread.join();
            for(auto& row : cells) for(auto& cell : row) {
                if (cell.type == CellType::Path || cell.type == CellType::Frontier) {
                    if (cell.type != CellType::Start && cell.type != CellType::End)
                        cell.type = CellType::Empty;
                }
                if (cell.type == CellType::Visited) {
                     cell.type = CellType::Empty;
                }
            }
            needs_cleanup = false;
        }

        window.clear(sf::Color::Cyan);
        draw_grid(window, cells, cell_size, offsetX, offsetY);
        window.display();
        sf::sleep(sf::milliseconds(17));
    }
}