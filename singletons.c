#include "singletons.h"

int fixSingletonsRow(sudoku* s, int row){
  int i; int c;
  int candidatecount;
  int lastindex=-1;
  int fixings=0;
  for(c=1; c<10; ++c){
    candidatecount=0;
    for(i=0; i<9; ++i){
      if(isCandidate(c,s->candidates2d[row][i])){
	candidatecount++;
	lastindex=i;
      }
    }
    if (candidatecount==1){
      if (s->fields2d[row][lastindex]!=c){
	printf("%d is a singleton in row %d.\n", c, row);
	s->fields2d[row][lastindex]=c;
	fixNumber(s, row, lastindex, c);
	fixings++;
      }
    }
  }
  return fixings;
}

int fixSingletonsCol(sudoku* s, int col){
  int i; int c;
  int candidatecount;
  int lastindex=-1;
  int fixings=0;
  for(c=1; c<10; ++c){
    candidatecount=0;
    for(i=0; i<9; ++i){
      if(isCandidate(c,s->candidates2d[i][col])){
	candidatecount++;
	lastindex=i;
      }
    }
    if (candidatecount==1){
      if (s->fields2d[lastindex][col]!=c){
	printf("%d is a singleton in col %d.\n", c, col);
	s->fields2d[lastindex][col]=c;
	fixNumber(s, lastindex, col, c);
	fixings++;
      }
    }
  }
  return fixings;
}

int fixSingletons(sudoku* s){
  int r,c,b;
  int i;
  for (i=0; i<9; ++i){
    r=fixSingletonsRow(s, i);
    printf("Fixed %d singletons in row %d.\n", r, i);
  }
  for (i=0; i<9; ++i){
    c=fixSingletonsCol(s, i);
    printf("Fixed %d singletons in col %d.\n", c, i);
  }
}