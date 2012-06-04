#include "types.h"
#include <stdio.h>

void printSudoku(sudoku *s);

void printCandidatesInRowInCol(sudoku *s, int row, int col);
void printCandidatesInRow(sudoku *s, int row);
void printCandidates(sudoku *s);
