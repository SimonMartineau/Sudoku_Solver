// Code by Simon Martineau

#ifndef SOLVER_HPP
#define SOLVER_HPP

#include<iostream>
#include<vector>
#include<algorithm>

class Sudoku {

public:
    // Stores grid reference
    int (&grid)[9][9];

    // 3D tensor that stores the possible values of every cell
    std::vector<int> possibilities_tensor[9][9];

    // Class variable checks if any change happened between cycles
    bool change_happened = 1;

    // Class variable to store temportary possibilities vector info
    std::vector<int> temp_possibilities;

    // Class variable to store number of loops
    int loop_counter = 0;

    // Class variable to count the number of empty cells at the end
    int zeros_counter = 0;

    // Constructor
    Sudoku(int (&inputGrid)[9][9]);

    // Method to print the sudoku
    void show();

    // Method to solve the sudoku
    void solve();

    // Method to see if the sudoku is finished(1), in progress(0) or stuck(2)
    int is_finished();

    // Method to set the 3D possibilities tensor 
    void initalize_possibilities_tensor();

    // Method to use the possibilities tensor
    void update_grid();

    // Method that uses the obvious singles technique
    void obvious_singles();

    // Method that uses the obvious pairs technique
    void obvious_pairs();

    // Method that uses the hidden singles technique
    void hidden_singles();

    // Method that uses the hidden pairs technique on rows
    void hidden_row_pairs();

    // Final verification
    void verify();
};

#endif