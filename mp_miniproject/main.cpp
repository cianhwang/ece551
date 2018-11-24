#include <ctype.h>
#include <cstdio>
#include <string.h>
#include <sstream>
#include <cstdlib>
#include <iostream>

#include "expr_utils.h"

using std::string;
using std::vector;
using std::map;
using std::pair;

void parseDef(const char **strp, string &name, vector<string> &vec){
  skipSpace(strp);
  if (**strp == '\0') {
    std::cerr << "End of line found mid expression!\n";
  }
  if (**strp == '(') {
    // (id id ...)
    *strp = *strp + 1;
    skipSpace(strp);
  }
  string str(*strp);
  std::stringstream ss(str);
  ss >> str;
  name = str;
  while(ss >> str){
    if (str != ")"){
      if (str[str.size()-1] != ')'){
	vec.push_back(str);
      }
      else{
	string tempStr(str, 0, str.size()-1);
	vec.push_back(tempStr);
      }
    }
  }
}

void defineFunc(FuncTable &funcTable, const char *deftemp, const char *temp){
  Expression *expr = parse(funcTable, &temp);
  vector<string> paraVec;
  string funcName;
  parseDef(&deftemp, funcName, paraVec);
  Expression *func = new FuncExpression(expr, paraVec);
  funcTable.addFunc(funcName, func);
}

bool test(Expression * lexpr, Expression * rexpr){
  return abs(lexpr->evaluate()-rexpr->evaluate()) < 0.0000000000001;
}

void testFunc(FuncTable &funcTable, const char *ltemp, const char *rtemp){
  Expression *lExpr = parse(funcTable, &ltemp);
  Expression *rExpr = parse(funcTable, &rtemp);
  std::cout << lExpr->evaluate() << std::endl;
  std::cout << rExpr->evaluate() << std::endl;
  
  if (test(lExpr, rExpr)){
    std::cout << "CORRECT.\n";
  }
  else{
    std::cout << "INCORRECT.\n";
  }
  delete lExpr;
  delete rExpr;
}

void readInput(FuncTable &funcTable, const char **strp){
  skipSpace(strp);
  const char *endp = (*strp + 1);
  while(*endp != ' '){
    ++endp;
  }
  string command(*strp, endp-*strp);
  *strp = endp + 1;
  while(*endp != '='){
    ++endp;
  }
  if (command == "define"){
    string defStr(*strp, endp-*strp);
    *strp = endp + 1;
    std::cout << "left is: " << defStr << std::endl;
    std::cout << "right is: " << *strp << std::endl;
    defineFunc(funcTable, defStr.c_str(), *strp);
  }
  else if (command == "test"){
    string ltestStr(*strp, endp-*strp);
    *strp = endp + 1;
    std::cout << "left is: " << ltestStr << std::endl;
    std::cout << "right is: " << *strp << std::endl;
    testFunc(funcTable, ltestStr.c_str(), *strp);
  }
  return;
}

int main(void) {

  FuncTable funcTable;
  char * line = NULL;
  size_t sz;
  while (getline(&line, &sz, stdin) != -1) {
    const char * temp = line;
    std::cout << "Read expression: " << line;
    readInput(funcTable, &temp);
  }
  free(line);

  /*  
  const char * temp = "(+ x y)";
  const char * deftemp = "(f x y)";
  FuncTable funcTable;
  defineFunc(funcTable, deftemp, temp);

  const char * temp2 = "(f (f a 1) (f 1 b))";
  const char * deftemp2 = "(g a b)";
  defineFunc(funcTable, deftemp2, temp2);

  const char * temp3 = "(g 5.0 3.0)";
  Expression *Temp = parse(funcTable, &temp3);

  std::cout << Temp->evaluate() << std::endl;


  const char * temp4 = "(f 1.5 5.3)";
  Expression *Temp2 = parse(funcTable, &temp4);

  std::cout << Temp2->evaluate() << std::endl;

  delete Temp;
  delete Temp2;
  */
  return EXIT_SUCCESS;
}
