// Code by Simon Martineau

#include "Sudoku.hpp"


// Constructor
Sudoku::Sudoku(int (&inputGrid)[9][9]): grid(inputGrid) {
    // No copying any array necessary, grid is a reference to inputGrid
}


// Method to print the sudoku
void Sudoku::show() {
    for (int i=0; i<9; i++) {
        for (int j=0; j<9; j++) {
            std::cout << grid[i][j] << " ";
        }
        std::cout << std::endl;
    }
}


// Method to solve the sudoku
void Sudoku::solve() {
    while (is_finished() == 0){
        // Sudoku is in progress
        change_happened = 0; // Reset change verifier

        // Technique 1: By process of elimination
        eliminiation();

        // Technique 2:
    }
    

    if (is_finished() == 1){
        // Sudoki is completed
        std::cout << "The sudoku is completed!" << std::endl;
    }

    else if (is_finished() == 2){
        // Sudoku is stuck
        std::cout << "The solver is stuck." << std::endl;
    }
}


// Method to see if the sudoku is finished(1), in progress(0) or stuck(2)
int Sudoku::is_finished() {
    
    for (int i=0; i<9; i++) {
        for (int j=0; j<9; j++) {
            if (grid[i][j] == 0){
                if (change_happened == 0){
                    return 2; // The grid isn't complete and there was not change in the previous cycle so we are stuck.
                    }
                return 0; // Sudoku not finished yet
            }
        }
    }

    return 1; // No 0 in the grid, the sudoku is finished
}


// Method to test by elimination
void Sudoku::eliminiation(){
    for (int i=0; i<9; i++) {
        for (int j=0; j<9; j++) {
            if (grid[i][j] == 0){
                // We only want to test empty cells

                // Using the elimination strategies
                possibilities.assign({1, 2, 3, 4, 5, 6, 7, 8, 9});
                test_row(i);
                test_column(j);
                test_box(i, j);
                
                // If only one possibility remains, it must be it.
                if (possibilities.size() == 1){
                    grid[i][j] = possibilities[0];
                    change_happened = 1;
                    }
                }
            }
        }
    }
    

// Method to eliminate possibilites by looking at the row
void Sudoku::test_row(int i){
    for (int j=0; j<9; j++){
        int value = grid[i][j]; // Value of the cell being scanned

        // Check if the value exists in possibilities
        auto it = std::find(possibilities.begin(), possibilities.end(), value);
        if (it != possibilities.end()) {
            // If it exists, remove it from possibilities
            possibilities.erase(it);
        }
    }
}


// Method to eliminate possibilites by looking at the column
void Sudoku::test_column(int j){
    for (int i=0; i<9; i++){
        int value = grid[i][j]; // Value of the cell being scanned

        // Check if the value exists in possibilities
        auto it = std::find(possibilities.begin(), possibilities.end(), value);
        if (it != possibilities.end()) {
            // If it exists, remove it from possibilities
            possibilities.erase(it);
        }
    }
}


// Method to eliminate possibilites by looking at the box
void Sudoku::test_box(int i, int j){
    int I = i/3; // row index of the box the cell is in.
    int J = j/3; // column index of the box the cell is in.
    
    for (int offset_i=0; offset_i<3; offset_i++){
        for (int offset_j=0; offset_j<3; offset_j++){
            int value = grid[3*I + offset_i][3*J + offset_j]; // Value of the cell being scanned

            // Check if the value exists in possibilities
            auto it = std::find(possibilities.begin(), possibilities.end(), value);
            if (it != possibilities.end()) {
                // If it exists, remove it from possibilities
                possibilities.erase(it);
            }
        }
    }

}