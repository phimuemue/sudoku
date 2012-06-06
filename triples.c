#include "triples.h"

int tripleIndices[84][3] = {{0, 1, 2}, {0, 1, 3}, {0, 1, 4}, {0, 1, 5},
			    {0, 1, 6}, {0, 1, 7}, {0, 1, 8}, {0, 2, 3},
			    {0, 2, 4}, {0, 2, 5}, {0, 2, 6}, {0, 2, 7},
			    {0, 2, 8}, {0, 3, 4}, {0, 3, 5}, {0, 3, 6},
			    {0, 3, 7}, {0, 3, 8}, {0, 4, 5}, {0, 4, 6},
			    {0, 4, 7}, {0, 4, 8}, {0, 5, 6}, {0, 5, 7},
			    {0, 5, 8}, {0, 6, 7}, {0, 6, 8}, {0, 7, 8},
			    {1, 2, 3}, {1, 2, 4}, {1, 2, 5}, {1, 2, 6},
			    {1, 2, 7}, {1, 2, 8}, {1, 3, 4}, {1, 3, 5},
			    {1, 3, 6}, {1, 3, 7}, {1, 3, 8}, {1, 4, 5},
			    {1, 4, 6}, {1, 4, 7}, {1, 4, 8}, {1, 5, 6},
			    {1, 5, 7}, {1, 5, 8}, {1, 6, 7}, {1, 6, 8},
			    {1, 7, 8}, {2, 3, 4}, {2, 3, 5}, {2, 3, 6},
			    {2, 3, 7}, {2, 3, 8}, {2, 4, 5}, {2, 4, 6},
			    {2, 4, 7}, {2, 4, 8}, {2, 5, 6}, {2, 5, 7},
			    {2, 5, 8}, {2, 6, 7}, {2, 6, 8}, {2, 7, 8},
			    {3, 4, 5}, {3, 4, 6}, {3, 4, 7}, {3, 4, 8},
			    {3, 5, 6}, {3, 5, 7}, {3, 5, 8}, {3, 6, 7},
			    {3, 6, 8}, {3, 7, 8}, {4, 5, 6}, {4, 5, 7},
			    {4, 5, 8}, {4, 6, 7}, {4, 6, 8}, {4, 7, 8},
			    {5, 6, 7}, {5, 6, 8}, {5, 7, 8}, {6, 7, 8}};

int fixTripleInRow(sudoku* s, int row, int col1, int col2, int col3){
  int i;
  int fixer = (s->candidates2d[row][col1] |
	       s->candidates2d[row][col2] |
	       s->candidates2d[row][col3]);
  int findings = 0;
  assert(countBits(fixer)==3);
  fixer = ~fixer;
  for (i=0; i<9; ++i){
    if (i!=col1 && i!=col2 && i!=col3){
      possible old = s->candidates2d[row][i];
      s->candidates2d[row][i] &= fixer;
      if (old!=s->candidates2d[row][i]){
	findings = 1;
      }
    }
  }
  return findings;
}

int fixTripleInCol(sudoku* s, int col, int row1, int row2, int row3){
  int i;
  int fixer = (s->candidates2d[row1][col] |
	       s->candidates2d[row2][col] |
	       s->candidates2d[row3][col]);
  int findings = 0;
  assert(countBits(fixer)==3);
  fixer = ~fixer;
  for (i=0; i<9; ++i){
    if (i!=row1 && i!=row2 && i!=row3){
      possible old = s->candidates2d[i][col];
      s->candidates2d[i][col] &= fixer;
      if (old!=s->candidates2d[i][col]){
	findings = 1;
      }
    }
  }
  return findings;
}

// TODO: rename all "pair"-stuff to "triple"-stuff
int fixTriples(sudoku* s){
  int* indices;
  int pair;
  int i;
  int row, col;
  int findings=0;
  // pairs in rows?
  for(row=0; row<9; ++row){
    for (i=0; i<84; ++i){
      indices=tripleIndices[i];
      if ((s->fields2d[row][indices[0]]==0) &&
	  (s->fields2d[row][indices[1]]==0) &&
	  (s->fields2d[row][indices[2]]==0)){ 
	pair=(s->candidates2d[row][indices[0]] |
	      s->candidates2d[row][indices[1]] |
	      s->candidates2d[row][indices[2]]);
	debugprintf("Row %d, cols %d, %d and %d (%u, %u, %u) have %d candidates in sum:\n", 
	       row, indices[0], indices[1], indices[2],
	       s->candidates2d[row][indices[0]],
	       s->candidates2d[row][indices[1]],
	       s->candidates2d[row][indices[2]], countBits(pair));
	int j;
	for (j=1; j<10; ++j){
	  if ((1u<<j)&pair){
	    debugprintf("%d, ", j);
	  }
	}
	debugprintf("\n");
	if (countBits(pair)==3){
	  debugprintf("Fixing triple in row %d (cols %d, %d, %d)\n",
		 row, indices[0], indices[1], indices[2]);
	  findings += fixTripleInRow(s, row, indices[0], indices[1], indices[2]);
#if DEBUG==1
	  printCandidates(s);
#endif
	}
      }
    }
  }
  // pairs in columns?
  for(col=0; col<9; ++col){
    for (i=0; i<84; ++i){
      indices=tripleIndices[i];
      if ((s->fields2d[indices[0]][col]==0) &&
	  (s->fields2d[indices[1]][col]==0) &&
	  (s->fields2d[indices[2]][col]==0)){
	pair=(s->candidates2d[indices[0]][col] |
	      s->candidates2d[indices[1]][col] |
	      s->candidates2d[indices[2]][col]);
	debugprintf("Col %d, rows %d, %d and %d (%u, %u, %u) have %d candidates in sum:\n", 
	       col, indices[0], indices[1], indices[2],
	       s->candidates2d[indices[0]][col],
	       s->candidates2d[indices[1]][col],
	       s->candidates2d[indices[2]][col], countBits(pair));
	int j;
	for (j=1; j<10; ++j){
	  if ((1u<<j)&pair){
	    debugprintf("%d, ", j);
	  }
	}
	debugprintf("\n");
	if (countBits(pair)==3){
	  debugprintf("Fixing triple in col %d (rows %d, %d, %d)\n",
		 col, indices[0], indices[1], indices[2]);
	  findings += fixTripleInCol(s, col, indices[0], indices[1], indices[2]);
#if DEBUG==1
	  printCandidates(s);
#endif
	}
      }
    }
  }
  if (findings) {
    fixTriples(s);
    return 1;
  }
  return 0;
}
