# Sudoku_Solver

Code by Simon Martineau.
simon.martineau@ensta-bretagne.org

## Overview
This repository contains a C++ program capable of solving any Sudoku puzzle from Easy to Master difficulty. 


## Table of Contents
- [Installation](#installation)
- [Usage](#usage)


## Installation
To install, follow these steps:

1. Clone the repository into your workspace:
   ```bash
   git clone https://github.com/SimonMartineau/Sudoku_Solver.git
   ```

2. Enter the **Sudoku_Solver** repository
   ```bash
    cd Sudoku_Solver
    ```

3. Create a **build** repository
    ```bash
    mkdir build
    cd build
    ```

4. Use cmake to compile the program
    ```bash
    cmake .. -G Ninja
    ```

5. Compile the program by using the command
   ```bash
   ninja
   ```

6. Run the executable with the command
    ```bash
    ./SudokuSolver
    ```

If there are any issues using this method to compile the program, you can use another C++ compiler like g++

    g++ main.cpp Sudoku.cpp -o SudokuSolver  && ./SudokuSolver 


## Usage

1. **To input your sudoku puzzle to solve**
    - Open the list_of_sudoku.txt file.
    - Copy the empty puzzle filled with 0s at the top.
    - Replace the existing puzzle in the main.cpp file and enter your puzzle.
    - Compile the code by entering this command in the **build** repository.
      
    ```bash
    ninja
    ```
    - Launch the executable.

2. **Techniques**

    This program uses 4 different techniques to solve sudoku puzzles.
    - Obvious Singles
    - Obvious Pairs
    - Hidden Singles
    - Hidden Pairs
    You get an analysis at the execution of the code of which techniques were used and how many times.

    You can modify which techniques are used in the Sudoku.cpp file in the solve method.

