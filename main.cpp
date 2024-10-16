// Code by Simon Martineau

#include <iostream>
#include "Sudoku.hpp"

int main(){
    int grid[9][9] = {
        {9, 0, 0, 0, 0, 0, 0, 3, 1},
        {0, 6, 1, 9, 3, 8, 0, 5, 4},
        {3, 5, 2, 4, 0, 6, 0, 8, 7},
        {0, 0, 5, 0, 7, 3, 4, 0, 0},
        {2, 0, 0, 0, 0, 0, 0, 6, 0},
        {0, 0, 0, 6, 0, 1, 5, 9, 0},
        {0, 7, 6, 0, 0, 0, 0, 4, 0},
        {4, 2, 0, 0, 6, 9, 0, 0, 0},
        {0, 0, 0, 7, 0, 0, 3, 0, 0}
    };

    Sudoku sudoku(grid);
    sudoku.solve();
    sudoku.show();

    return 0;
}