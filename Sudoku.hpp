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

    // Class variable checks if any change happened between cycles
    bool change_happened = 1;

    // Class variable vector for possible values of any cell
    std::vector<int> possibilities; 

    // Constructor
    Sudoku(int (&inputGrid)[9][9]);

    // Method to print the sudoku
    void show();

    // Method to solve the sudoku
    void solve();

    // Method to see if the sudoku is finished(1), in progress(0) or stuck(2)
    int is_finished();

    // Method to test by elimination
    void eliminiation();

    // Method to eliminate possibilites by looking at the row
    void test_row(int i);

    // Method to eliminate possibilites by looking at the column
    void test_column(int j);

    // Method to eliminate possibilites by looking at the box
    void test_box(int i, int j);


};

#endif