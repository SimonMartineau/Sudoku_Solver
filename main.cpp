// Code by Simon Martineau
// simon.martineau@ensta-bretagne.org

#include <iostream>
#include "Sudoku.hpp"

int main(){
    int grid[9][9] = {
        {0, 2, 6, 0, 0, 1, 0, 0, 0},
        {3, 0, 0, 0, 0, 9, 2, 5, 7},
        {4, 9, 5, 0, 0, 0, 0, 1, 0},
        {0, 4, 0, 0, 5, 3, 1, 0, 0},
        {0, 6, 3, 2, 1, 0, 7, 4, 0},
        {0, 0, 1, 0, 9, 0, 0, 3, 0},
        {0, 8, 9, 0, 7, 0, 0, 0, 1},
        {6, 0, 0, 0, 8, 0, 4, 2, 9},
        {0, 0, 0, 9, 6, 0, 3, 0, 8}
    };

    Sudoku sudoku(grid);
    sudoku.solve();
    sudoku.show();

    return 0;
}