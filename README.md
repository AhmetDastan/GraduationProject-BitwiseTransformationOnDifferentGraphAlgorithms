# Graph Algorithms with Bitwise Transformation

This project consists of two main components:
1. **Bitwise transformation of BFS-DFS algorithms**
2. **Analysis and conversion of equitable graph coloring using bitwise transformation**

Both components have separate makefiles and should be run independently. Detailed usage instructions are provided below.

## Table of Contents
- [Equitable Graph Coloring](#equitable-graph-coloring)
- [BFS-DFS Transformation](#bfs-dfs-transformation)
- [Installation and Usage](#installation-and-usage)
- [File Structure](#file-structure)

## Equitable Graph Coloring

### Overview
This component implements and transforms equitable graph coloring algorithms using bitwise operations.

### Reference
Based on the research paper: "Backtracking based iterated tabu search for equitable coloring" by W J-K & Xiangjing Lai.

### Files Description

- **`BITS.cpp`**: Original source code from the referenced article
- **`bitwise.cpp`**: Our bitwise transformation implementation
- **`bitwise.hpp`**: Bit Vertex transformation logic derived from "BitEA_BitVertex_Evolutionary_Algorithm_to_Enhance_Performance_for_Register_Allocation" (GTU). Contains transformation logic diagrams
- **`bitwiseCustom.cpp`**: Custom implementation with different list compression techniques, developed when GTU transformation logic couldn't be fully adapted. Includes compressed logic diagrams
- **`DSJC125.9.col`**: Test graph dataset for project evaluation

### Running Equitable Graph Coloring

1. Navigate to the equitable graph coloring directory
2. Run the following command:
   ```bash
   make
   ```
3. After execution, check `resultFile.txt` for algorithm comparison results (average time over 10 repetitions)

### Validation

To validate the results:

1. Run the validation script:
   ```bash
   python checkValidation.py
   ```
2. Before running, modify the `COLORING_STRING` variable in the script with the control block output

#### Expected Terminal Output Format:
```
13 43 2 22 26 39 33 28 9 25 23 34 16 19 22 5 25 32 12 10 40 35 4 40 9 10 13 42 38 19 15 7 7 20 4 21 8 34 22 32 35 0 3 15 37 29 33 14 6 33 43 6 23 7 9 29 18 28 18 16 39 21 0 8 30 1 23 13 42 24 8 41 5 36 29 4 41 31 10 11 14 20 32 12 27 37 17 3 14 3 17 24 43 35 27 40 17 24 28 1 36 38 26 11 6 5 39 26 0 31 19 42 30 30 12 16 27 11 41 38 18 2 25 37 34 
```

**Note**: Use the third line (vertex color numbers) for validation input.

## BFS-DFS Transformation

### Overview
This component implements bitwise transformations of Breadth-First Search (BFS) and Depth-First Search (DFS) algorithms.

### Directory Structure

- **`BFS/`**: Contains 3 different BFS implementations
- **`DFS/`**: Contains 3 different DFS implementations  
- **`Graphs/`**: Test graphs with varying vertex counts and 20% density

### Files Description

- **`main.py`**: Visualization tool for analyzing `result.txt` output

### Running BFS-DFS Transformation

1. Navigate to the `Bfs-DfsTransformation` directory
2. Run the following command:
   ```bash
   make
   ```

### Visualization

To visualize the results:
```bash
python main.py
```

## Installation and Usage

### Prerequisites
- C++ compiler (g++ recommended)
- Python 3.x
- Make utility

### Quick Start

1. Clone the repository
2. Choose the component you want to run:
   - For Equitable Graph Coloring: Navigate to the respective directory and run `make`
   - For BFS-DFS Transformation: Navigate to `Bfs-DfsTransformation` and run `make`

## File Structure

```
project-root/
├── README.md
├── Equitable-Graph-Coloring/
│   ├── BITS.cpp
│   ├── bitwise.cpp
│   ├── bitwise.hpp
│   ├── bitwiseCustom.cpp
│   ├── DSJC125.9.col
│   ├── checkValidation.py
│   └── Makefile
└── Bfs-DfsTransformation/
    ├── BFS/
    ├── DFS/
    ├── Graphs/
    ├── main.py
    └── Makefile
```
 
