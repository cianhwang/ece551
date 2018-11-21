#include<iostream>
#include "expr_utils.h"
#include "func.h"
#include <ctype.h>
#include <cstdlib>
#include <string.h>
#include <stdio.h>
#include <algorithm>

int main(void) {
  char * line = NULL;
  size_t sz;
  GenFunction funArr;
  while (getline(&line, &sz, stdin) != -1) {
    const char * temp = line;
    std::cout << "Read expression: " << line;
    //Expression * expr = parse(&temp);
    funArr.parseFunc(&temp);
    //    if (expr != NULL) {
    if (true){
      //      std::cout << "Evaluated expression to: " << expr->eval() << "\n";
      //      std::vector<std::string> vec;
      //vec.push_back("x");
      //vec.push_back("y");
      //funArr.addFunc("f", vec, expr);
      for (int i = 0; i < 5; ++i){
	std::vector<double> numVec;
	double a;
	for (int i = 0; i < 3; ++i){ 
	  std::cin >> a;
	  numVec.push_back(a);
	}
	funArr["f"]->assign(numVec);
	std::cout << "func outcome: " << funArr["f"]->calc() << "\n";
      }
      //      delete expr;
    }
    else {
      std::cout << "Could not parse expression, please try again.\n";
    }
  }
  free(line);
  return EXIT_SUCCESS;
}
