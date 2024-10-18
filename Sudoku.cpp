// Code by Simon Martineau
// simon.martineau@ensta-bretagne.org

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
        loop_counter++; // Increment the loop counter
        change_happened = 0; // Reset change verifier

        obvious_singles(); // Technique 1: Obvious singles

        obvious_pairs(); // Technique 2: Obvious pairs

        hidden_singles(); // Technique 3: Hidden singles

        hidden_pairs(); // Technique 4: Hidden pairs

        update_grid(); // Use possibilities_tensor on grid
        }
    
    if (is_finished() == 1){
        std::cout << "The sudoku is completed!" << std::endl; // Sudoki is completed
        verify();
        std::cout << "Obvious singles technique: " << obvious_singles_counter << "x" << "\n";
        std::cout << "Obvious pairs technique: " << obvious_pairs_counter << "x" << "\n";
        std::cout << "Hidden singles technique: " << hidden_singles_counter << "x" << "\n";
        std::cout << "Hidden pairs technique: " << hidden_pairs_counter << "x" << "\n";
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
                grid[i][j] = possibilities_tensor[i][j][0]; // The grid was updated
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
                    change_happened = 1;
                    obvious_singles_counter++;
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
                    change_happened = 1;
                    obvious_singles_counter++;
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
                        change_happened = 1;
                        obvious_singles_counter++;
            }
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
                                        obvious_pairs_counter++;
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
                                        obvious_pairs_counter++;
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
                                                obvious_pairs_counter++;
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
                        hidden_singles_counter++;
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
                        hidden_singles_counter++;
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
                        hidden_singles_counter++;
                }
            }
        }
    } 
}



// Method that uses the hidden pairs technique
void Sudoku::hidden_pairs(){
    hidden_row_pairs();
    hidden_column_pairs();
    hidden_box_pairs();
}



// Method that uses the hidden pairs technique on rows
void Sudoku::hidden_row_pairs(){
    for (int i=0; i<9; i++){
        // We navigate through every row
        int appearance_array[10] = {0}; // Array that stores the number of times each number appears in the possibilities_tensor in that row
        for (int n=1; n<=9; n++){
            for (int j=0; j<9; j++){
                if (std::find(possibilities_tensor[i][j].begin(), possibilities_tensor[i][j].end(), n) != possibilities_tensor[i][j].end()) {
                    // The value n is found in the vector possibilities_tensor[i][j]
                    appearance_array[n]++; // Increment the number of appearances of number n
                }
            }
        }

        
        std::vector<int> dual_possibilities_numbers; // Store the numbers that appear twice
        int number_of_doubles = 0;
        for (int n=1; n<=9; n++){
            if (appearance_array[n] == 2){
                // Check to see if any numbers appear twice
                dual_possibilities_numbers.push_back(n);
                number_of_doubles++;

            }
        }

        if (number_of_doubles >=2){
            for (int j1=0; j1<8; j1++){
                for (int j2=j1+1; j2<9; j2++){
                    for (int elem1=0; elem1<dual_possibilities_numbers.size()-1; elem1++){
                        for (int elem2=elem1+1; elem2<dual_possibilities_numbers.size(); elem2++){
                            // We increment through 2 numbers in the list of numbers that appear twice in the possibilities_tensor in these cells
                            if (std::find(possibilities_tensor[i][j1].begin(), possibilities_tensor[i][j1].end(), dual_possibilities_numbers[elem1]) != possibilities_tensor[i][j1].end()
                            && std::find(possibilities_tensor[i][j1].begin(), possibilities_tensor[i][j1].end(), dual_possibilities_numbers[elem2]) != possibilities_tensor[i][j1].end()
                            && std::find(possibilities_tensor[i][j2].begin(), possibilities_tensor[i][j2].end(), dual_possibilities_numbers[elem1]) != possibilities_tensor[i][j2].end()
                            && std::find(possibilities_tensor[i][j2].begin(), possibilities_tensor[i][j2].end(), dual_possibilities_numbers[elem2]) != possibilities_tensor[i][j2].end()){
                                
                                if (possibilities_tensor[i][j1].size() > 2 || possibilities_tensor[i][j2].size() > 2){
                                    // A change only happened if the possibilities_tensor reduces in size
                                    hidden_pairs_counter++;
                                    change_happened = 1;
                                }
                                possibilities_tensor[i][j1] = {dual_possibilities_numbers[elem1], dual_possibilities_numbers[elem2]};
                                possibilities_tensor[i][j2] = {dual_possibilities_numbers[elem1], dual_possibilities_numbers[elem2]};
                                return;                      
                            }
                        }
                    }
                }
            }
        }
    }
}



// Method that uses the hidden pairs technique on columns
void Sudoku::hidden_column_pairs(){
    for (int j=0; j<9; j++){
        // We navigate through every column
        int appearance_array[10] = {0}; // Array that stores the number of times each number appears in the possibilities_tensor in that column
        for (int n=1; n<=9; n++){
            for (int i=0; i<9; i++){
                if (std::find(possibilities_tensor[i][j].begin(), possibilities_tensor[i][j].end(), n) != possibilities_tensor[i][j].end()) {
                    // The value n is found in the vector possibilities_tensor[i][j]
                    appearance_array[n]++; // Increment the number of appearances of number n
                }
            }
        }

        
        std::vector<int> dual_possibilities_numbers; // Store the numbers that appear twice
        int number_of_doubles = 0;
        for (int n=1; n<=9; n++){
            if (appearance_array[n] == 2){
                // Check to see if any numbers appear twice
                dual_possibilities_numbers.push_back(n);
                number_of_doubles++;

            }
        }

        if (number_of_doubles >=2){
            for (int i1=0; i1<8; i1++){
                for (int i2=i1+1; i2<9; i2++){
                    for (int elem1=0; elem1<dual_possibilities_numbers.size()-1; elem1++){
                        for (int elem2=elem1+1; elem2<dual_possibilities_numbers.size(); elem2++){
                            // We increment through 2 numbers in the list of numbers that appear twice in the possibilities_tensor in these cells
                            if (std::find(possibilities_tensor[i1][j].begin(), possibilities_tensor[i1][j].end(), dual_possibilities_numbers[elem1]) != possibilities_tensor[i1][j].end()
                            && std::find(possibilities_tensor[i1][j].begin(), possibilities_tensor[i1][j].end(), dual_possibilities_numbers[elem2]) != possibilities_tensor[i1][j].end()
                            && std::find(possibilities_tensor[i2][j].begin(), possibilities_tensor[i2][j].end(), dual_possibilities_numbers[elem1]) != possibilities_tensor[i2][j].end()
                            && std::find(possibilities_tensor[i2][j].begin(), possibilities_tensor[i2][j].end(), dual_possibilities_numbers[elem2]) != possibilities_tensor[i2][j].end()){

                                if (possibilities_tensor[i1][j].size() > 2 || possibilities_tensor[i2][j].size() > 2){
                                    // A change only happened if the possibilities_tensor reduces in size
                                    hidden_pairs_counter++;
                                    change_happened = 1;
                                }
                                possibilities_tensor[i1][j] = {dual_possibilities_numbers[elem1], dual_possibilities_numbers[elem2]};
                                possibilities_tensor[i2][j] = {dual_possibilities_numbers[elem1], dual_possibilities_numbers[elem2]};
                                return;                      
                            }
                        }
                    }
                }
            }
        }
    }
}



// Method that uses the hidden pairs technique on boxes
void Sudoku::hidden_box_pairs(){
    for (int I=0; I<3; I++){
        for (int J=0; J<3; J++){
            // We navigate through every box
            int appearance_array[10] = {0}; // Array that stores the number of times each number appears in the possibilities_tensor in that box
            for (int offset_i=0; offset_i<3; offset_i++){
                for (int offset_j=0; offset_j<3; offset_j++){
                    int cell_i = 3*I + offset_i;
                    int cell_j = 3*J + offset_j;
                    for (int n=1; n<=9; n++){
                        if (std::find(possibilities_tensor[cell_i][cell_j].begin(), possibilities_tensor[cell_i][cell_j].end(), n) != possibilities_tensor[cell_i][cell_j].end()) {
                            // The value n is found in the vector possibilities_tensor[i][j]
                            appearance_array[n]++; // Increment the number of appearances of number n
                        }
                    }
                }
            }

            std::vector<int> dual_possibilities_numbers; // Store the numbers that appear twice
            int number_of_doubles = 0; // To count how many numbers appear twice
            for (int n=1; n<=9; n++){
                if (appearance_array[n] == 2){
                    // Check to see if any numbers appear twice
                    dual_possibilities_numbers.push_back(n);
                    number_of_doubles++;

                }
            }

            if (number_of_doubles >=2){
                // If we have 2 numbers that appear twice at least in the possibilities_tensor
                for (int offset_i1=0; offset_i1<3; offset_i1++){
                    for (int offset_j1=0; offset_j1<3; offset_j1++){
                        for (int offset_i2=0; offset_i2<3; offset_i2++){
                            for (int offset_j2=0; offset_j2<3; offset_j2++){
                                // We define the offsets for the position of the 2 cells in the box
                                if (offset_i1 == offset_j1 && offset_j1 == offset_j2){}
                                else{
                                    // The 2 cells must be different
                                    int cell_i1 = 3*I + offset_i1;
                                    int cell_i2 = 3*I + offset_i2;
                                    int cell_j1 = 3*J + offset_j2;
                                    int cell_j2 = 3*J + offset_j2;                                    

                                    for (int elem1=0; elem1<dual_possibilities_numbers.size()-1; elem1++){
                                        for (int elem2=elem1+1; elem2<dual_possibilities_numbers.size(); elem2++){
                                            // We increment through 2 numbers in the list of numbers that appear twice in the possibilities_tensor in these cells
                                            if (std::find(possibilities_tensor[cell_i1][cell_j1].begin(), possibilities_tensor[cell_i1][cell_j1].end(), dual_possibilities_numbers[elem1]) != possibilities_tensor[cell_i1][cell_j1].end()
                                                && std::find(possibilities_tensor[cell_i1][cell_j1].begin(), possibilities_tensor[cell_i1][cell_j1].end(), dual_possibilities_numbers[elem2]) != possibilities_tensor[cell_i1][cell_j1].end()
                                                && std::find(possibilities_tensor[cell_i2][cell_j2].begin(), possibilities_tensor[cell_i2][cell_j2].end(), dual_possibilities_numbers[elem1]) != possibilities_tensor[cell_i2][cell_j2].end()
                                                && std::find(possibilities_tensor[cell_i2][cell_j2].begin(), possibilities_tensor[cell_i2][cell_j2].end(), dual_possibilities_numbers[elem2]) != possibilities_tensor[cell_i2][cell_j2].end()){

                                                    // We reduce the number of possibilities to just these 2 as the technique shows.
                                                    if (possibilities_tensor[cell_i1][cell_j1].size() > 2 || possibilities_tensor[cell_i2][cell_j2].size() > 2){
                                                        // A change only happened if the possibilities_tensor reduces in size
                                                        hidden_pairs_counter++;
                                                        change_happened = 1;
                                                    }

                                                    possibilities_tensor[cell_i1][cell_j1] = {dual_possibilities_numbers[elem1], dual_possibilities_numbers[elem2]};
                                                    possibilities_tensor[cell_i2][cell_j2] = {dual_possibilities_numbers[elem1], dual_possibilities_numbers[elem2]};
                                                    return;  // We exit the function as soon as we find a hidden pair                     
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



// Final verification
void Sudoku::verify(){
    int problem_flag = 0;
    // Check the rows
    for (int i=0; i<9; i++){
        for (int n=1; n<=9; n++){
            // Testing that every number is in every row
            bool found = false;  // Flag to track if the number is found
            for (int j=0; j<9; j++){
                if (grid[i][j] == n){
                    found = true; // Exit the inner loop as the number is found
                    break;
                }
            }
            if (!found) {
                std::cout << "Sudoku error: " << n << " not found on row " << i << "\n";
                problem_flag = 1;
            }
        }
    }

    // Check the columns
    for (int j=0; j<9; j++){
        for (int n=1; n<=9; n++){
            // Testing that every number is in every column
            bool found = false;  // Flag to track if the number is found
            for (int i=0; i<9; i++){
                if (grid[i][j] == n){
                    found = true; // Exit the inner loop as the number is found
                    break;
                }
            }
            if (!found) {
                std::cout << "Sudoku error: " << n << " not found on column " << j << "\n";
                problem_flag = 1;
            }
        }
    }

    // Check the boxes
    for (int I=0; I<3; I++){
        for (int J=0; J<3; J++){
            // Going through each box
            for (int n=1; n<=9; n++){
                // Testing that every number is in every column
                bool found = false;  // Flag to track if the number is found
                for (int offset_i=0; offset_i<3; offset_i++){
                    for (int offset_j=0; offset_j<3; offset_j++){
                        // Going through each cell in the box
                        if (grid[3*I+offset_i][3*J+offset_j] == n){
                            found = true; // Exit the inner loop as the number is found
                            break;
                        }
                    }
                    if (found) {
                        break; // Break out of the outer loop as well when n is found
                    }
                }
                if (!found) {
                        std::cout << "Sudoku error: " << n << " not found in box " << I << "," << J <<" \n";
                        problem_flag = 1;
                    }
            }
        }
    }
    if (!problem_flag){
        std::cout << "The solution has been verified and has no problems." << "\n";
    }
}