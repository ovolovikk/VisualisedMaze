# SFML Maze Visualizer

This is a C++ application that visualizes maze generation and pathfinding algorithms using the SFML library. It provides an interactive way to see how algorithms like DFS, BFS, and Dijkstra's work on a grid.

## Features

*   **Maze Generation**:
    *   **Depth-First Search (DFS)**: Generates a maze with long corridors and few dead ends.
    *   **Randomized Prim's Algorithm (BFS-like)**: Generates a maze with more branching and shorter corridors.
*   **Pathfinding**:
    *   **Dijkstra's Algorithm**: Finds the shortest path between a start and end point in a generated maze.
*   **Interactive Controls**:
    *   Set custom start and end points for pathfinding.
    *   Reset the grid to its initial state.
    *   Automatically generates a maze if pathfinding is requested on an empty grid.

## Dependencies

To build and run this project, you will need:
*   A C++17 compatible compiler (like `g++` or `clang++`)
*   `make`
*   **SFML 2.5+**: A multimedia library. On macOS, it can be easily installed via Homebrew:
    ```bash
    brew install sfml
    ```

## Building the Project

1.  Clone the repository.
2.  Navigate to the project directory.
3.  Run `make` to compile the project.

    ```bash
    make
    ```
4.  An executable named `VisualisedMaze` will be created.

## How to Use

Run the executable from your terminal:
```bash
./VisualisedMaze
```

### Controls

| Key | Action                                                              |
|-----|---------------------------------------------------------------------|
| `D` | Generate a maze using **Depth-First Search (DFS)**.                 |
| `B` | Generate a maze using **Randomized Prim's Algorithm (BFS-like)**.   |
| `K` | Find the shortest path using **Dijkstra's Algorithm**.              |
| `R` | **Reset** the grid to a blank state with all walls.                 |
| `S` | Hover over a cell and press `S` to set the **Start** point.         |
| `E` | Hover over a cell and press `E` to set the **End** point.           |