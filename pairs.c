#include "pairs.h"

// pair indices

int pairIndices[36][2] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6},
			  {0, 7}, {0, 8}, {1, 2}, {1, 3}, {1, 4}, {1, 5},
			  {1, 6}, {1, 7}, {1, 8}, {2, 3}, {2, 4}, {2, 5},
			  {2, 6}, {2, 7}, {2, 8}, {3, 4}, {3, 5}, {3, 6},
			  {3, 7}, {3, 8}, {4, 5}, {4, 6}, {4, 7}, {4, 8},
			  {5, 6}, {5, 7}, {5, 8}, {6, 7}, {6, 8}, {7, 8}};

int fixPairInRow(sudoku* s, int row, int col1, int col2){
  int i;
  int fixer = (s->candidates2d[row][col1] | s->candidates2d[row][col2]);
  int findings = 0;
  assert(countBits(fixer)==2);
  fixer = ~fixer;
  for (i=0; i<9; ++i){
    if (i!=col1 && i!=col2){
      possible old = s->candidates2d[row][i];
      s->candidates2d[row][i] &= fixer;
      if (old != s->candidates2d[row][i]){
	findings++;
      }
    }
  }
  return findings;
}

int fixPairInCol(sudoku* s, int col, int row1, int row2){
  int i;
  int fixer = (s->candidates2d[row1][col] | s->candidates2d[row2][col]);
  int findings = 0;
  assert(countBits(fixer)==2);
  fixer = ~fixer;
  for (i=0; i<9; ++i){
    if (i!=row1 && i!=row2){
      possible old = s->candidates2d[i][col];
      s->candidates2d[i][col] &= fixer;
      if (old != s->candidates2d[i][col]){
	findings++;
      }
    }
  }
  return findings;
}

int fixPairs(sudoku* s){
  int* indices;
  int pair;
  int i;
  int row, col;
  int findings=0;
  // pairs in rows?
  for(row=0; row<9; ++row){
    for (i=0; i<36; ++i){
      indices=pairIndices[i];
      if ((s->fields2d[row][indices[0]]==0) &&
	  (s->fields2d[row][indices[1]]==0)){ // TODO: why does this segfault?
	pair=(s->candidates2d[row][indices[0]] | s->candidates2d[row][indices[1]]);
	debugprintf("Row %d, cols %d and %d (%u, %u) have %d candidates in sum:\n", 
	       row, indices[0], indices[1],
	       s->candidates2d[row][indices[0]],
	       s->candidates2d[row][indices[1]], countBits(pair));
	int j;
	for (j=1; j<10; ++j){
	  if ((1u<<j)&pair){
	    debugprintf("%d, ", j);
	  }
	}
	debugprintf("\n");
	if (countBits(pair)==2){
	  debugprintf("Fixing pair in row %d (cols %d, %d)\n", row, indices[0], indices[1]);
	  findings += fixPairInRow(s, row, indices[0], indices[1]);
#if DEBUG==1
	  printCandidates(s);
#endif
	}
      }
    }
  }
  // pairs in columns?
  for(col=0; col<9; ++col){
    for (i=0; i<36; ++i){
      indices=pairIndices[i];
      if ((s->fields2d[indices[0]][col]==0) &&
	  (s->fields2d[indices[1]][col]==0)){ // TODO: why does this segfault?
	pair=(s->candidates2d[indices[0]][col] | s->candidates2d[indices[1]][col]);
	debugprintf("Col %d, rows %d and %d (%u, %u) have %d candidates in sum:\n", 
	       col, indices[0], indices[1],
	       s->candidates2d[indices[0]][col],
	       s->candidates2d[indices[1]][col], countBits(pair));
	int j;
	for (j=1; j<10; ++j){
	  if ((1u<<j)&pair){
	    debugprintf("%d, ", j);
	  }
	}
	debugprintf("\n");
	if (countBits(pair)==2){
	  debugprintf("Fixing pair in col %d (rows %d, %d)\n", col, indices[0], indices[1]);
	  findings += fixPairInCol(s, col, indices[0], indices[1]);
#if DEBUG==1
	  printCandidates(s);
#endif
	}
      }
    }
  }
  // do we need additional rounds?
  if (findings){
    fixPairs(s);
    return 1;
  }
  return 0;
}
