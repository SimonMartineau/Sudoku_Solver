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
    void use_possibilities_tensor();

    // Method to test rows, columns and boxes to eliminate impossibilities from the possibilities_tensor
    void update_possibilities_tensor();

    // Method to use the hidden singles technique
    void hidden_singles();



};

#endif