extern int out;

#define INC ++cells[cell_index];
#define DEC --cells[cell_index];
#define LEFT --cell_index;
#define RIGHT ++cell_index;
#define PRINT ??;
#define READ ??;
#define BEGINLOOP for(;cells[cell_index];) { 
#define ENDLOOP }

#include "helper.h"

#define CELL_TYPE char
#define CELL_SIZE 64

#define FUNCTION_NAME test

#define POTTEMPLATE(code) \
int FUNCTION_NAME() { \
  CELL_TYPE _cells[CELL_SIZE] = {0}; \
  CELL_TYPE *const cells = _cells; \
  int cell_index = 0; \
  code\
  out = *(cells + cell_index);\
  return 0;\
}