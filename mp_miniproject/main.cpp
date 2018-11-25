#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "expr_utils.h"



int main(void) {
  FuncTable funcTable;
  char * line = NULL;
  size_t sz;
  while (getline(&line, &sz, stdin) != -1) {
    const char * temp = line;
    //    std::cout << "Read expression: " << line;
    funcTable.readInput( &temp);
  }
  free(line);

  return EXIT_SUCCESS;
}
