#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960
#define GRID_WIDTH 25
#define GRID_HEIGHT 25
#define CELL_RADIUS 10.f
#define CELL_POINT_COUNT 100
#define GRID_PIXEL_WIDTH GRID_WIDTH * (CELL_RADIUS * 3)
#define GRID_PIXEL_HEIGHT GRID_HEIGHT * (CELL_RADIUS * 3)

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
    cells.resize(GRID_WIDTH, std::vector<Cell>(GRID_HEIGHT));
    sf::CircleShape cellShape(CELL_RADIUS, CELL_POINT_COUNT);
    cellShape.setFillColor(sf::Color::Black);

    float offsetX = (window.getSize().x - GRID_PIXEL_WIDTH) / 2.0f;
    float offsetY = (window.getSize().y - GRID_PIXEL_HEIGHT) / 2.0f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Cyan);
        for(int i = 0;i < GRID_WIDTH;++i)
        {
            for(int j = 0; j < GRID_HEIGHT;++j)
            {
                CellType curr_cell_type = cells.at(i).at(j).type;
                cells.at(i).at(j).x = i;
                cells.at(i).at(j).y = j;
                cellShape.setPosition(i * (CELL_RADIUS * 3) + offsetX, j * (CELL_RADIUS * 3) + offsetY);
                switch (curr_cell_type)
                {
                    case CellType::Empty:
                        cellShape.setFillColor(sf::Color::Black);
                    case CellType::Wall:
                        cellShape.setFillColor(sf::Color::White);
                    case CellType::Start:
                        cellShape.setFillColor(sf::Color::Red);
                    case CellType::End:
                        cellShape.setFillColor(sf::Color::Green);
                    case CellType::Visited:
                        cellShape.setFillColor(sf::Color::Yellow);
                    case CellType::Path:
                        cellShape.setFillColor(sf::Color::Magenta);
                    default:
                        cellShape.setFillColor(sf::Color::Black);
                }
                window.draw(cellShape);
            }
        }
        window.display();
        sf::sleep(sf::milliseconds(17));
    }
}
