# Sudoku-Solver
This project uses C/C++ to create a program that can create Sudoku puzzles and 
solve Sudoku puzzles using recursive algorithms. The program can also create custom 
user-inputted Sudoku puzzles through .txt files, which can then be reinputted into 
the program to solve.

The way the program work is the console input must have the following format:
    ./Sudoku -s [game .txt file]

The .txt file must be in the correct format in order to work. The program will detect 
and notify of any formatting or value errors of the inputted file. If unaware
of the correct format the .txt file must be in, the user can create one using the
program by simply not including any file in the console.
