# Course Prerequisite and Building Route Planner Using Graphs in C

## Project Overview
This project implements a **course prerequisite planner** alongside a **building route planner** using **Dijkstra's algorithm** to find the shortest paths between buildings. The system helps determine the most efficient route between campus buildings while managing course prerequisites in an optimized sequence. Courses are represented as vertices in a graph, with prerequisites as directed edges, and building routes are calculated to optimize traversal between them.

### Features
- **Graph Representation**: 
  - Courses are represented as vertices, and prerequisites are directed edges.
  - Campus buildings are also represented as nodes, with paths between them as weighted edges.
- **Topological Sorting for Course Prerequisites**:
  - Ensures courses are taken in the correct order based on dependencies.
- **Dijkstra's Algorithm for Shortest Building Routes**:
  - Finds the shortest path between buildings on campus, helping users navigate efficiently.
- **Min-Heap for Route Optimization**:
  - Utilized in Dijkstra's algorithm to quickly determine the next building or course to process.
- **Queue Structure**:
  - Manages the order of courses and building traversal using an efficient queue structure.

### Data Structures Used
- **Graph (Vertices and Edges)**: Represents both course prerequisites and campus buildings/routes.
- **Min-Heap**: Used to efficiently compute shortest paths in Dijkstra's algorithm.
- **Queue**: Used for processing course sequences and managing traversal between buildings.

### Technologies
- **Language**: C
- **Data Structures**:
  - Graph (with vertices and edges)
  - Min-Heap (used in Dijkstra's algorithm)
  - Queue

### How to Use
1. **Compile the Program**: Ensure you have a C compiler installed. Use the following command to compile the program:
    ```bash
    gcc -o course_building_planner main.c
    ```
2. **Run the Program**: After compilation, run the program:
    ```bash
    ./course_building_planner
    ```
3. **Input Data**: The program will prompt you to input:
   - Number of courses and their prerequisites
   - Campus buildings and the paths between them (with distances)
   - Use Dijkstra's algorithm to find the shortest route between two buildings.
