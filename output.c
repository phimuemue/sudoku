#include "output.h"

void printSudoku(sudoku *s){
  int i=0; 
  for (i=0; i<81; ++i){
    if (s->fields[i]>0){
      printf("%d ", s->fields[i]);
    }
    else {
      printf(". ");
    }
    ((i+1)%9)||printf("\n"); // newline after each 9 digits
  }
  printf("\n");
}

void printCandidatesInRowInCol(sudoku *s, int row, int col){
  int i, j;
  printf("|");
  for (i=9*row; i<9*row+9; ++i){
    if (s->fields[i]){
      if (col==4){
	printf(" %d ", s->fields[i]);
      }
      else {
	printf("   ");
      }
    }
    else {
      for (j=col; j<=col+2; ++j){
	if (isCandidate(j, s->candidates[i])){
	  //printf("%d ", j);
	  printf("*");
	}
	else {
	  printf(" ");
	}
      }
    }
    // TODO: Maybe there's an asci character for "||"
    if (i%3==2){
      printf("|");
    }
    else {
      printf("|");
    }
  }
  printf("\n");
}

void printCandidatesInRow(sudoku *s, int row){
  if (row%3!=0){
    printf("+-----------------------------------+\n");
  }
  else {
    printf("+===================================+\n");
  }
  printCandidatesInRowInCol(s, row, 1);
  printCandidatesInRowInCol(s, row, 4);
  printCandidatesInRowInCol(s, row, 7);
}

void printCandidates(sudoku *s){
  int i;
  for (i=0; i<9; ++i){
    printCandidatesInRow(s, i);
  }
  printf("+===================================+\n");
}

