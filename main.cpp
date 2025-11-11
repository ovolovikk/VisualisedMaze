#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960
#define GRID_WIDTH 25
#define GRID_HEIGHT 25

enum class CellType
{
    Empty, Wall, Start, End, Visited, Path
};

struct Cell
{
    CellType type = CellType::Empty;
    int x, y;

    bool visited = false;
    float global_goal;
    float local_goal;
    float heuristic;
    Cell* parent = nullptr;
    std::vector<Cell*> neighbours;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML MazeVisualizer");
    std::vector<std::vector<Cell>> cells;
    
    float cell_width = static_cast<float>(WINDOW_WIDTH) / GRID_WIDTH;
    float cell_height = static_cast<float>(WINDOW_HEIGHT) / GRID_HEIGHT;
    float cell_size = std::min(cell_width, cell_height);

    const float grid_pixel_width = GRID_WIDTH * cell_size;
    const float grid_pixel_height = GRID_HEIGHT * cell_size;
    
    cells.resize(GRID_WIDTH, std::vector<Cell>(GRID_HEIGHT));
    sf::RectangleShape cell_shape(sf::Vector2f(cell_size - 1, cell_size - 1));
    cell_shape.setFillColor(sf::Color::Black);

    float offsetX = (window.getSize().x - grid_pixel_width) / 2.0f;
    float offsetY = (window.getSize().y - grid_pixel_height) / 2.0f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                    
                    int gridX = static_cast<int>((mouse_pos.x - offsetX) / cell_size);
                    int gridY = static_cast<int>((mouse_pos.y - offsetY) / cell_size);

                    if (gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT)
                    {
                        Cell& cell = cells.at(gridX).at(gridY);
                        if (cell.type == CellType::Empty)
                        {
                            cell.type = CellType::Wall;
                        }
                        else if (cell.type == CellType::Wall)
                        {
                            cell.type = CellType::Empty;
                        }
                    }
                }
            }
                
        }

        window.clear(sf::Color::Cyan);
        for(int i = 0;i < GRID_WIDTH;++i) {
            for(int j = 0; j < GRID_HEIGHT;++j) {
                Cell& current_cell = cells.at(i).at(j);
                current_cell.x = i;
                current_cell.y = j;
                cell_shape.setPosition(i * cell_size + offsetX, j * cell_size + offsetY);
                switch (current_cell.type)
                {
                    case CellType::Empty:
                        cell_shape.setFillColor(sf::Color::Black);
                        break;
                    case CellType::Wall:
                        cell_shape.setFillColor(sf::Color::White);
                        break;
                    case CellType::Start:
                        cell_shape.setFillColor(sf::Color::Red);
                        break;
                    case CellType::End:
                        cell_shape.setFillColor(sf::Color::Green);
                        break;
                    case CellType::Visited:
                        cell_shape.setFillColor(sf::Color::Yellow);
                        break;
                    case CellType::Path:
                        cell_shape.setFillColor(sf::Color::Magenta);
                        break;
                    default:
                        cell_shape.setFillColor(sf::Color::Black);
                }
                window.draw(cell_shape);
            }
        }
        window.display();
        sf::sleep(sf::milliseconds(17));
    }
}
