#include "backtrack.h"

// we allow ourselfes some globals
int firstUnsolved;
sudoku* current;

// checks whether a number can be set in a certain field
int checkNumber(sudoku* s, int row, int col, int number){
  int i=0;
  assert(number<=9 && number>=1);
  for (i=0; i<9; ++i){
    if((s->fields2d[row][i]==number && i!=col) || (s->fields2d[i][col]==number && i!=row)){
      return 0;
    }
  }
  int boxX = 3 * (row / 3);
  int boxY = 3 * (col / 3);
  int x, y;
  for (x=boxX; x<boxX+3; ++x){
    for (y=boxY; y<boxY+3; ++y){
      if (s->fields2d[x][y]==number && (x != row || y != col)){
	return 0;
      }
    }
  }
  return 1;
}

// sets a certain cell to the next possible candidate.
int setNextCandidate(sudoku* s, int index){
  int i=0;
  for (i=s->fields[index]+1; i<10; ++i){
    if ((1u<<i) & s->candidates[index]){
      if (checkNumber(s, getX(index), getY(index), i)){
	s->fields[index]=i;
	return i;
      }
    }
  }
  s->fields[index]=0;
  return 0;
}

// returns the index of the next unsolved (i.e. field==0) cell
int getNextUnsolved(sudoku* s, int index){
  int i=index+1;
  for (i=index+1; i<81; ++i){
    if (s->fields[i]==0){
      return i;
    }
  }
  return -1;
}

int internal_backtrack(sudoku* s, int index){
  printf("Backtracking %d...\n", index);
  // get next legal value for cell
  while (setNextCandidate(s, index)){
    printf("Set field %d to value %d.\n", index, s->fields[index]);
    // if current candidate has become illegal in the meantime, try the next one
    if (checkNumber(s, getY(index), getX(index), s->fields[index])){
      continue;
    }
    // ok, so current setting seems legal, so continue with next one
    int next = getNextUnsolved(s, index);
    if (next==-1){
      return 1;
    }
    int deeper = internal_backtrack(s, next);
    if (deeper){
      return deeper;
    }
  }
  return 0;
}

int compareTwoCells (const void* a, const void* b){
  return (countBits(current->candidates[*(int*)a]) - countBits(current->candidates[*(int*)b]));
}

int backtrack(sudoku* s){
  current = s;
  int unsolvedIndices[81];
  printf("Backtracking...\n");
  int nextUnsolved=getNextUnsolved(s, -1);
  int countUnsolved;
  int i=0;
  while(nextUnsolved!=-1){
    unsolvedIndices[i++] = nextUnsolved;
    nextUnsolved = getNextUnsolved(s, nextUnsolved);
  }
  countUnsolved=i;

#if BACKTRACK_SORT_HEURISTIC==1
  printf("Qsorting");
  qsort(unsolvedIndices, countUnsolved, sizeof(int), compareTwoCells);
#endif

  // this is debug output
  /* for (i=0; i<countUnsolved; ++i){ */
  /*   int ci=unsolvedIndices[i]; */
  /*   printf("Unsolved cell at %d => (%d, %d) | %d.\n", ci, getX(ci), getY(ci), countBits(s->candidates[ci])); */
  /* } */

  // now solving
  i=0;
  while (i>-1 && i<countUnsolved){
    //printf("Now tackling cell %d... ", unsolvedIndices[i]);
    if (setNextCandidate(s, unsolvedIndices[i])){
      i++;
    }
    else {
      i--;
    }
    //printf("set to %d\n", s->fields[unsolvedIndices[i]]);
  }
  if (i==-1) printf("Unsolved\n");
  if (i==countUnsolved) printf("Solved\n");
}
