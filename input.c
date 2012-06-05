#include "input.h"

static FILE *f = 0;

sudoku* getNextSudoku(FILE* f){
  sudoku* res = (sudoku*)malloc(sizeof(sudoku));
  int i;
  for (i=0; i<81; ++i){
    res->fields[i]=fgetc(f)-'0';
    if (res->fields[i]+'0'==EOF){
      return 0;
    }
    printf("%d ", res->fields[i]);
    if (res->fields[i]<=0 || res->fields[i]>9){
      res->fields[i]=0;
    }
    if (res->fields[i]>0){
      res->candidates[i]=(1<<res->fields[i]);
    }
    else{
      res->candidates[i]=(1<<10)-1; // all candidates possible (1 to 9, not 0)
    }
  }
  if (fgetc(f)==EOF){
    printf("Closing\n");
    fclose(f);
    return 0;
  }
  printf("\n");
  return res;
}

sudoku* readSudokuFromFile(const char* path){
  if (!f){
    f = fopen(path, "r");
  }
  sudoku* res = getNextSudoku(f);
  return res;
}
