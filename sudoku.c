#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "types.h"
#include "singletons.h"
#include "backtrack.h"

inline int getX(int index){return index/9;}
inline int getY(int index){return index%9;}
inline int getIndex(int x, int y){return 9*y+x;}

inline int countBits(int n){
  int count = 0;
  /* while (n) { */
  /*   count += n & 0x1u; */
  /*   n >>= 1; */
  /* } */
  int i;
  for (i=1; i<10; ++i){
    if((1<<i)&n) count++;
  }
  return count;
}

// checks whether number is a possible solution for candidate
int isCandidate(int number, int candidate){
  return (1<<number)&candidate;
}

// gives the next possible candidate, given the currently assigned value
int nextCandidate(int number, int candidate){
  int i;
  for (i=number+1; i<=9; ++i){
    if (isCandidate(i, candidate)){
      return i;
    }
  }
  return -1;
}

void removeCandidatesInRow(sudoku* s, int row, int col, int number){
  //printf("Removing candidates in row (%d, %d) => %d ...\n", row, col, getIndex(col, row));
  int x, y=row;
  for (x=0; x<9; ++x){
    if (x!=col){
      int index = getIndex(x,y);
      //printf("%d, ", index);
      s->candidates[index] = s->candidates[index] & ~(1<<number);      
    }
  }
  //printf("\n");
}

void removeCandidatesInCol(sudoku* s, int row, int col, int number){
  //printf("Removing candidates in col (%d, %d) => %d ...\n", row, col, getIndex(col, row));
  int x=col, y;
  for (y=0; y<9; ++y){
    if (y!=row){
      int index = getIndex(x,y);
      //printf("%d, ", index);
      s->candidates[index] = s->candidates[index] & ~(1<<number);      
    }
  }
  //printf("\n");
}

void removeCandidatesInBox(sudoku* s, int row, int col, int number){
  //printf("Removing candidates in box (%d, %d) => %d ...\n", row, col, getIndex(col, row));
  int boxX = 3 * (row / 3);
  int boxY = 3 * (col / 3);
  //printf("Removing candidates in box (%d, %d) => %d ...\n", boxX, boxY, getIndex(col, row));
  int x, y;
  for (x=boxX; x<boxX+3; ++x){
    for (y=boxY; y<boxY+3; ++y){
      if (x != row || y != col){
	//printf ("%d, ", getIndex(y,x));
	int index = getIndex(y,x);
	s->candidates[index] = s->candidates[index] & ~(1<<number);      	
      }
    }
  }
  //printf("\n");
}

void fixNumber(sudoku* s, int row, int col, int number){
  removeCandidatesInRow(s, row, col, number);
  removeCandidatesInCol(s, row, col, number);
  removeCandidatesInBox(s, row, col, number);
}

void fixGivens(sudoku* s){
  int i;
  for (i=0; i<81; ++i){
    if (s->fields[i]){
      fixNumber(s, getX(i), getY(i), s->fields[i]);
    }
  }
}

int getCandidateSingleton(int c){
  int i;
  for(i=1; i<=9; ++i){
    if ((1<<i)&c){
      return i;
    }
  }
  return 0;
}

int countSingletons(sudoku* s){
  int count = 0;
  int i;
  for (i=0; i<81; ++i){
    if (countBits(s->candidates[i])==1){
      s->fields[i] = getCandidateSingleton(s->candidates[i]);
      count++;
    }
  }
  return count;
}

void checkPairsInRow(sudoku* s, int row){
  int x1, x2;
  for (x1=0; x1<9; ++x1){
    for (x2=x1+1; x2<9; ++x2){
      // combine two cells
      int i1 = getIndex(x1, row);
      int i2 = getIndex(x2, row);
      int combi = (s->candidates[i1] | s->candidates[i2]) & ~1;
      printf ("Testing row %d: %d, %d, =>  %d, %d => %d\n", row, x1, x2, s->candidates[i1], s->candidates[i2], combi);
      if (countBits(combi)==2 && s->fields[i1]==0 && s->fields[i2]==0){
	// "backup fields"
	int b1, b2;
	b1 = s->candidates[i1];
	b2 = s->candidates[i2];
	int i;
	for (i=1; i<=9; ++i){
	  if (isCandidate(i, combi) && s->fields[i1]!=0 && s->fields[i2]!=0){
	    printf ("Found a pair in row %d: %d, %d, => %d\n", row, x1, x2, combi);
	    removeCandidatesInRow(s, row, x1, i);
	  }
	}
	s->candidates[i1] = b1;
	s->candidates[i2] = b2;
	printCandidates(s);
      }
    } 
  }
}

void checkTriplesInRow(sudoku* s, int row){
  int x1, x2, x3;
  for (x1=0; x1<9; ++x1){
    for (x2=x1+1; x2<9; ++x2){
      for (x3=x2+1; x3<9; ++x3){
	// combine three cells
	int i1 = getIndex(x1, row);
	int i2 = getIndex(x2, row);
	int i3 = getIndex(x3, row);
	int combi = (s->candidates[i1] | s->candidates[i2] | s->candidates[i3]) & ~1;
	printf ("Testing row %d: %d, %d, %d =>  %d, %d, %d => %d\n", row, x1, x2, x3, s->candidates[i1], s->candidates[i2], s->candidates[i3], combi);
	if (countBits(combi)==3){
	  // "backup fields"
	  int b1, b2, b3;
	  b1 = s->candidates[i1];
	  b2 = s->candidates[i2];
	  b3 = s->candidates[i3];
	  int i;
	  for (i=1; i<=9; ++i){
	    if (isCandidate(i, combi) && s->fields[i1]!=0 && s->fields[i2]!=0 && s->fields[i3]!=0){
	      printf ("Found a triple in row %d: %d, %d, %d => %d\n", row, x1, x2, x3, combi);
	      removeCandidatesInRow(s, row, x1, i);
	    }
	  }
	  s->candidates[i1] = b1;
	  s->candidates[i2] = b2;
	  s->candidates[i3] = b3;
	  printCandidates(s);
	}
      }
    } 
  }
}

void checkTriplesInCol(sudoku* s, int col){
  int y1, y2, y3;
  for (y1=0; y1<9; ++y1){
    for (y2=y1+1; y2<9; ++y2){
      for (y3=y2+1; y3<9; ++y3){
	// combine three cells
	int i1 = getIndex(col, y1);
	int i2 = getIndex(col, y2);
	int i3 = getIndex(col, y3);
	int combi = (s->candidates[i1] | s->candidates[i2] | s->candidates[i3]) & ~1;
	printf ("Testing col %d: %d, %d, %d =>  %d, %d, %d => %d\n", col, y1, y2, y3, s->candidates[i1], s->candidates[i2], s->candidates[i3], combi);
	if (countBits(combi)==3){
	  // "backup fields"
	  int b1, b2, b3;
	  b1 = s->candidates[i1];
	  b2 = s->candidates[i2];
	  b3 = s->candidates[i3];
	  int i;
	  for (i=1; i<=9; ++i){
	    if (isCandidate(i, combi) && s->fields[i1]!=0 && s->fields[i2]!=0 && s->fields[i3]!=0){
	      printf ("Found a triple in col %d: %d, %d, %d => %d\n", col, y1, y2, y3, combi);
	      removeCandidatesInCol(s, y1, col, i);
	    }
	  }
	  s->candidates[i1] = b1;
	  s->candidates[i2] = b2;
	  s->candidates[i3] = b3;
	  printCandidates(s);
	}
      }
    } 
  }
}

void checkTriples(sudoku* s){
  int i;
  for (i=0; i<9; ++i){
    checkPairsInRow(s, i);
    /* checkTriplesInRow(s, i); */
    /* checkTriplesInCol(s, i); */
  }
}

// generate list of numbers (resp. their indices) to solve:
void generateIndexList(sudoku *s, int* indices){
  int i;
  int currentIndex = 0;
  for (i=0; i<81; ++i){
    if (s->fields[i]==0){
      indices[currentIndex++] = i;
    }
  }
  for (i=currentIndex; i<81; ++i){
    indices[i] = -1;
  }
}

int solve(sudoku* s){
  fixGivens(s);
  if (fixSingletons(s)) printf("Singletons found.\n");
  if (fixPairs(s)) printf("Pairs found.\n");
  if (fixTriples(s)) printf("Triples found.\n");
  printCandidates(s);
  backtrack(s);  
}

int main(int argc, char** argv){
  sudoku *s; 
  while(s=readSudokuFromFile("input.txt")){
    solve(s);
    printSudoku(s);
    free(s);
  }
  return 0;
}
