#ifndef SUDOKU_TYPES
#define SUDOKU_TYPES

typedef unsigned int possible;  // possibilities, essentially a bit vector

typedef struct _sudoku{
  union {
    int fields[81]; // sudoku fields to fill
    int fields2d[9][9];
  };
  union {
    possible candidates[81]; // candidates for each field
    possible candidates2d[9][9]; // candidates for each field
  };
} sudoku;

#endif
