// Code by Simon Martineau

#include <iostream>
#include "Sudoku.hpp"

int main(){
    int grid[9][9] = {
        {0, 1, 0, 0, 0, 0, 0, 3, 0},
        {7, 3, 0, 9, 0, 8, 2, 0, 0},
        {0, 0, 5, 3, 1, 0, 0, 7, 0},
        {1, 0, 0, 8, 6, 0, 4, 0, 2},
        {0, 0, 0, 0, 0, 5, 0, 0, 6},
        {0, 0, 0, 2, 4, 0, 0, 0, 0},
        {0, 5, 0, 4, 0, 9, 0, 0, 0},
        {0, 0, 6, 0, 8, 0, 0, 4, 0},
        {0, 7, 0, 0, 0, 0, 0, 9, 0}
    };


    Sudoku sudoku(grid);
    sudoku.solve();
    //sudoku.verify();
    sudoku.show();

    return 0;
}