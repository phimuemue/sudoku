#include "input.h"

sudoku* readSudokuFromFile(const char* path){
  FILE *f = fopen(path, "r");
  sudoku* res = (sudoku*)malloc(sizeof(sudoku));
  int i;
  for (i=0; i<81; ++i){
    res->fields[i]=fgetc(f)-'0';
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
  fclose(f);
  return res;
}
