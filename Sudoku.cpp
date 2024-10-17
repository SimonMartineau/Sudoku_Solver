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
    initalize_possibilities_tensor();
    while (is_finished() == 0){ // Sudoku is in progress
        change_happened = 0; // Reset change verifier

        update_possibilities_tensor(); // Check and update the possibilities_tensor

        hidden_singles(); // Technique 1; Hidden singles

        use_possibilities_tensor(); // Use possibilities_tensor on grid
    }
    

    if (is_finished() == 1){
        std::cout << "The sudoku is completed!" << std::endl; // Sudoki is completed
    }

    else if (is_finished() == 2){
        std::cout << "The solver is stuck." << std::endl; // Sudoku is stuck
    }
}


// Method to see if the sudoku is finished(1), in progress(0) or stuck(2)
int Sudoku::is_finished() {
    for (int i=0; i<9; i++) {
        for (int j=0; j<9; j++) {
            if (grid[i][j] == 0){
                if (change_happened == 0){
                    // The grid isn't complete and there was not change in the previous cycle so we are stuck.
                    return 2;
                    }
                return 0; // Sudoku is still in progress
            }
        }
    }

    return 1; // No empty cell is left, the sudoku is finished
}


// Set the 3D possibilities tensor 
void Sudoku::initalize_possibilities_tensor(){
    for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
            if (grid[i][j] == 0){
                // If the cell is empty, every value is possible in the beginning.
                possibilities_tensor[i][j].assign({1, 2, 3, 4, 5, 6, 7, 8, 9});
            }
            else{
                // If the cell is filled, the possible value is already fixed.
                possibilities_tensor[i][j].assign({grid[i][j]});
            }
        }
    }
}


// Method to use the possibilities tensor
void Sudoku::use_possibilities_tensor(){
    for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
            if(possibilities_tensor[i][j].size() == 1 && grid[i][j] == 0){
                grid[i][j] = possibilities_tensor[i][j][0];
                change_happened = 1; // The grid was updated, the process is still going
            }
        }
    }
}


// Method to test rows, columns and boxes to eliminate impossibilities from the possibilities_tensor
void Sudoku::update_possibilities_tensor(){
    for (int cell_i=0; cell_i<9; cell_i++){
        for (int cell_j=0; cell_j<9; cell_j++){
            // Eliminate impossibilities according to the row
            for (int j=0; j<9; j++){
                int value = grid[cell_i][j]; // Value of the cell being scanned

                // Check if the value exists in possibilities
                auto it = std::find(possibilities_tensor[cell_i][cell_j].begin(), possibilities_tensor[cell_i][cell_j].end(), value);
                if (it != possibilities_tensor[cell_i][cell_j].end()) {
                    // If it exists, remove it from possibilities
                    possibilities_tensor[cell_i][cell_j].erase(it);
                    }
                }


            // Eliminate impossibilities according to the column
            for (int i=0; i<9; i++){
                int value = grid[i][cell_j]; // Value of the cell being scanned

                // Check if the value exists in possibilities
                auto it = std::find(possibilities_tensor[cell_i][cell_j].begin(), possibilities_tensor[cell_i][cell_j].end(), value);
                if (it != possibilities_tensor[cell_i][cell_j].end()) {
                    // If it exists, remove it from possibilities
                    possibilities_tensor[cell_i][cell_j].erase(it);
                    }
                }


            // Eliminate impossibilities according to the box
            int I = cell_i/3; // row index of the box the cell is in.
            int J = cell_j/3; // column index of the box the cell is in.
            
            for (int offset_i=0; offset_i<3; offset_i++){
                for (int offset_j=0; offset_j<3; offset_j++){
                    int value = grid[3*I + offset_i][3*J + offset_j]; // Value of the cell being scanned

                    // Check if the value exists in possibilities
                    auto it = std::find(possibilities_tensor[cell_i][cell_j].begin(), possibilities_tensor[cell_i][cell_j].end(), value);
                    if (it != possibilities_tensor[cell_i][cell_j].end()) {
                        // If it exists, remove it from possibilities
                        possibilities_tensor[cell_i][cell_j].erase(it);
                }
            }
        }


            }
        }
    }


// Method to use the hidden singles technique
void Sudoku::hidden_singles(){
    for (int cell_i=0; cell_i<9; cell_i++){
        for (int cell_j=0; cell_j<9; cell_j++){
            if (grid[cell_i][cell_j] == 0){

                // Use hidden singles technique on the rows
                temp_possibilities = possibilities_tensor[cell_i][cell_j];
                for (int j=0; j<9; j++){
                    if (j != cell_j){
                        for (const int& value : possibilities_tensor[cell_i][j]){
                            auto it = std::find(temp_possibilities.begin(), temp_possibilities.end(), value);
                            if (it != temp_possibilities.end()) {
                                // If it exists, remove it from possibilities
                                temp_possibilities.erase(it);
                                }
                            }   
                        }
                    }
                    if (temp_possibilities.size() == 1){
                            possibilities_tensor[cell_i][cell_j] = {temp_possibilities[0]};
                        }
                

                // Use hidden singles technique on the columns
                temp_possibilities = possibilities_tensor[cell_i][cell_j];
                for (int i=0; i<9; i++){
                    if (i != cell_i){
                        for (const int& value : possibilities_tensor[i][cell_j]){
                            auto it = std::find(temp_possibilities.begin(), temp_possibilities.end(), value);
                                if (it != temp_possibilities.end()) {
                                    // If it exists, remove it from possibilities
                                    temp_possibilities.erase(it);
                                    }
                            }
                        }
                    }
                    if (temp_possibilities.size() == 1){
                            possibilities_tensor[cell_i][cell_j] = {temp_possibilities[0]};
                        }
                }
            }
        } 
    }