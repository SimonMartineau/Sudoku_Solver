// Code by Simon Martineau

#include <iostream>
#include "Sudoku.hpp"

int main(){
    int grid[9][9] = {
        {4, 3, 0, 6, 8, 0, 7, 0, 0},
        {0, 0, 6, 0, 0, 9, 0, 0, 0},
        {0, 0, 0, 0, 0, 5, 0, 0, 8},
        {0, 0, 0, 0, 0, 6, 0, 0, 0},
        {7, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 4, 3, 2, 0, 0, 0, 9},
        {0, 0, 3, 8, 4, 0, 0, 0, 2},
        {0, 0, 0, 0, 5, 0, 0, 0, 0},
        {0, 2, 0, 0, 0, 0, 9, 0, 0}
    };


    Sudoku sudoku(grid);
    sudoku.solve();
    sudoku.show();
    sudoku.verify();

    return 0;
}