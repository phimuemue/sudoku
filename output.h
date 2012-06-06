#include "types.h"
#include <stdio.h>

#if DEBUG==1
#define debugprintf(...) printf (__VA_ARGS__)
#else
#define debugprintf(...)
#endif

void printSudoku(sudoku *s);

void printCandidatesInRowInCol(sudoku *s, int row, int col);
void printCandidatesInRow(sudoku *s, int row);
void printCandidates(sudoku *s);
