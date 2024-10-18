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
            if (grid[i][j] == 0){
                zeros_counter++;
                }
            }
        std::cout << std::endl;
        }

    if (zeros_counter > 0){
        std::cout << zeros_counter << " empty cells left" << "\n";
        }
    }


// Method to solve the sudoku
void Sudoku::solve() {
    initalize_possibilities_tensor();
    while (is_finished() == 0){ // Sudoku is in progress
        loop_counter++;
        change_happened = 0; // Reset change verifier

        obvious_singles(); // Technique 1: Obvious singles

        obvious_pairs(); // Technique 2: Obvious pairs

        hidden_singles(); // Technique 3: Hidden singles

        //hidden_row_pairs(); // Technique 4: Hidden pairs on the rows

        update_grid(); // Use possibilities_tensor on grid
        }
    

    if (is_finished() == 1){
        std::cout << "The sudoku is completed!" << std::endl; // Sudoki is completed
        }

    else if (is_finished() == 2){
        std::cout << "The solver is stuck." << std::endl; // Sudoku is stuck
        }
    std::cout << "loop_counter = " << loop_counter << "\n";
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
void Sudoku::update_grid(){
    for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
            if(possibilities_tensor[i][j].size() == 1 && grid[i][j] == 0){
                grid[i][j] = possibilities_tensor[i][j][0];
                change_happened = 1; // The grid was updated, the process is still going
                }
            }
        }
    }


// Method that uses the obvious singles technique
void Sudoku::obvious_singles(){
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
                        // If the examined cell isn't our current cell
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
                        change_happened = 1; // The grid was updated, the process is still going
                    }
                

                // Use hidden singles technique on the columns
                temp_possibilities = possibilities_tensor[cell_i][cell_j];
                for (int i=0; i<9; i++){
                    if (i != cell_i){
                        // If the examined cell isn't our current cell
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
                        change_happened = 1; // The grid was updated, the process is still going
                    }
                
                // Use hidden singles technique on the boxes
                temp_possibilities = possibilities_tensor[cell_i][cell_j];
                int I = cell_i/3; // row index of the box the cell is in.
                int J = cell_j/3; // column index of the box the cell is in.
                for (int offset_i=0; offset_i<3; offset_i++){
                    for (int offset_j=0; offset_j<3; offset_j++){
                        if (3*I + offset_i == cell_i && 3*J + offset_j == cell_j){}
                        else{
                            // If the examined cell isn't our current cell
                            for (const int& value : possibilities_tensor[3*I + offset_i][3*J + offset_j]){
                                auto it = std::find(temp_possibilities.begin(), temp_possibilities.end(), value);
                                if (it != temp_possibilities.end()) {
                                    // If it exists, remove it from possibilities
                                    temp_possibilities.erase(it);
                                    }
                                }   
                            }
                        }
                    }
                if (temp_possibilities.size() == 1){
                        possibilities_tensor[cell_i][cell_j] = {temp_possibilities[0]};
                        change_happened = 1; // The grid was updated, the process is still going
                    }

                }
            }
        } 
    }


// Method that uses the obvious pairs technique
void Sudoku::obvious_pairs(){
    for (int cell_i=0; cell_i<9; cell_i++){
        for (int cell_j=0; cell_j<9; cell_j++){
            if (grid[cell_i][cell_j] == 0 && possibilities_tensor[cell_i][cell_j].size() == 2){
                
                // Using hidden pairs technique on the rows
                for (int j=0; j<9; j++){
                    if (j != cell_j && possibilities_tensor[cell_i][cell_j] == possibilities_tensor[cell_i][j]){
                        // We don't want to eliminate the possibilities from the 2 cells that must contain them, and any other cell we can eliminate them from must have 3+ possibilities.
                        for(int elem_j=0; elem_j<9; elem_j++){
                            if (possibilities_tensor[cell_i][elem_j].size() > 2){
                                // We don't want to eliminate the possibilities from the 2 cells that must contain them
                                for (const int& value : possibilities_tensor[cell_i][cell_j]){
                                    auto it = std::find(possibilities_tensor[cell_i][elem_j].begin(), possibilities_tensor[cell_i][elem_j].end(), value);
                                    if (it != possibilities_tensor[cell_i][elem_j].end()) {
                                        // If it exists, remove it from possibilities
                                        possibilities_tensor[cell_i][elem_j].erase(it);
                                        change_happened = 1;
                                        std::cout << "hi" << "\n";
                                        }
                                    }   
                                }
                            }
                        }  
                    }
                
                // Using hidden pairs technique on the columns
                for (int i=0; i<9; i++){
                    if (i != cell_i && possibilities_tensor[cell_i][cell_j] == possibilities_tensor[i][cell_j]){
                        // If another cell has the same 2 possibilities, we can eliminate these 2 possibilities from the other cells in the same column
                        for(int elem_i=0; elem_i<9; elem_i++){
                            if (possibilities_tensor[elem_i][cell_j].size() > 2){
                                // We don't want to eliminate the possibilities from the 2 cells that must contain them, and any other cell we can eliminate them from must have 3+ possibilities.
                                for (const int& value : possibilities_tensor[cell_i][cell_j]){
                                    auto it = std::find(possibilities_tensor[elem_i][cell_j].begin(), possibilities_tensor[elem_i][cell_j].end(), value);
                                    if (it != possibilities_tensor[elem_i][cell_j].end()) {
                                        // If it exists, remove it from possibilities
                                        possibilities_tensor[elem_i][cell_j].erase(it);
                                        change_happened = 1;
                                        std::cout << "hi" << "\n";
                                        }
                                    }   
                                }
                            }
                        }  
                    }

                // Using hidden pairs technique on the boxes
                int I = cell_i/3;
                int J = cell_j/3;
                for (int offset_i=0; offset_i<3; offset_i++){
                    for (int offset_j=0; offset_j<3; offset_j++){
                        if (3*I+offset_i != cell_i && 3*J+offset_j != cell_j && possibilities_tensor[3*I+offset_i][3*J+offset_j] == possibilities_tensor[cell_i][cell_j]){
                            // If another cell has the same 2 possibilities, we can eliminate these 2 possibilities from the other cells in the same box
                            for(int offset_elem_i=0; offset_elem_i<3; offset_elem_i++){
                                for (int offset_elem_j=0; offset_elem_j<3; offset_elem_j++){
                                    if (possibilities_tensor[3*I+offset_elem_i][3*J+offset_elem_j].size() > 2){
                                        // We don't want to eliminate the possibilities from the 2 cells that must contain them, and any other cell we can eliminate them from must have 3+ possibilities.
                                        for (const int& value : possibilities_tensor[cell_i][cell_j]){
                                            auto it = std::find(possibilities_tensor[3*I+offset_elem_i][3*J+offset_elem_j].begin(), possibilities_tensor[3*I+offset_elem_i][3*J+offset_elem_j].end(), value);
                                            if (it != possibilities_tensor[3*I+offset_elem_i][3*J+offset_elem_j].end()) {
                                                // If it exists, remove it from possibilities
                                                possibilities_tensor[3*I+offset_elem_i][3*J+offset_elem_j].erase(it);
                                                change_happened = 1;
                                                std::cout << "hi" << "\n";
                                                }
                                            }
                                        }
                                    }
                                }
                            }  
                        }
                    }
                }
            }
        }
    }


// Method that uses the hidden pairs technique on rows
void Sudoku::hidden_row_pairs(){
    for (int j=0; j<9; j++){
        // Navigating every row of the sudoku
        

        }    
    }