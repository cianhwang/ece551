#include <ctype.h>
#include <cstdio>
#include <string.h>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <stack>
#include "expr_utils.h"

using std::string;
using std::vector;
using std::map;
using std::pair;
using std::stack;

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
  const char *endp = *strp;
  while(*endp != ')'){
    ++endp;
  }
  string str(*strp, endp-*strp);
  std::stringstream ss(str);
  ss >> str;

  name = str;
  while(ss >> str){
    vec.push_back(str);
  }
}

bool checkID(const string &id){
  for (size_t i = 0; i < id.size(); ++i){
    if (!isalpha(id[i])){
      return false;
    }
  } 
  return true;
}

void defineFunc(FuncTable &funcTable, const char *deftemp, const char *temp){
  vector<string> paraVec;
  string funcName;
  parseDef(&deftemp, funcName, paraVec);
  if (!checkID(funcName)){
    std::cerr << "Id should be all letters.\n";
    return;
  }
  if (paraVec.empty()){
    std::cerr << "At least one parameter.\n";
    return;
  }
  else{
    for (size_t i = 0; i < paraVec.size(); ++i){
      if (!checkID(paraVec[i])){
	std::cerr << "Id should be all letters.\n";
	return;
      }
    }
    // error handling: each para are unique.
  }
  Expression *expr = parse(funcTable, &temp);
  if (expr == NULL){
    std::cout << "Could not parse expression, please try again.\n";
    return;
  }
  // error handing: expression contains parameters that not belong to function.
  Expression *func = new FuncExpression(expr, paraVec);
  if (funcTable.addFunc(funcName, func)){
    std::cout << "defined " << funcName << "(";
    for (size_t i = 0; i < paraVec.size()-1; ++i){
      std::cout << paraVec[i] << " ";
    }
    std::cout << paraVec[paraVec.size()-1] << ")\n";
  }
}

bool test(Expression * lexpr, Expression * rexpr){
  return abs(lexpr->evaluate()-rexpr->evaluate()) < 0.0000000000001;
}

void printTest(const string &lstr, const string &rstr){
  string temp(lstr);
  string rtemp(rstr);
  temp.erase(temp.begin());
  std::stringstream ss(temp);
  ss >> temp;
  std::cout << temp << "(";
  while (ss >> temp){
    std::cout << temp << " ";
  }
  std::cout << "= ";
  rtemp.erase(rtemp.end()-1);
  std::cout << rtemp;
}

void testFunc(FuncTable &funcTable, const char *ltemp, const char *rtemp){
  string lstr(ltemp);
  string rstr(rtemp);
  Expression *lExpr = parse(funcTable, &ltemp);
  Expression *rExpr = parse(funcTable, &rtemp);
  // error handling: check no variables 
  //                 and all ids refer to function.
  printTest(lstr, rstr);
  if (test(lExpr, rExpr)){
    std::cout << " [correct]\n";
  }
  else{
    std::cout << " [INCORRECT: expected "<< lExpr->evaluate() <<"]\n";
  }
  delete lExpr;
  delete rExpr;
}

void readInput(FuncTable &funcTable, const char **strp){
  skipSpace(strp);
  if (**strp == '\0') {
    std::cerr << "End of line found mid expression!\n";
    return;
  }
  if (**strp == '#'){
    return;
  }
  const char *endp = (*strp + 1);
  while(*endp != ' '){
    if (*endp == '\0'){
      std::cerr << "Invalid expression.\n";
      return;
    }
    ++endp;
  }
  string command(*strp, endp-*strp);
  *strp = endp + 1;
  skipSpace(strp);
  endp = *strp;
  if (command == "define"){
    while(*endp != '='){
      if (*endp == '\0'){
	std::cerr << "Expect = but failed to find.\n";
	return;
      }
      ++endp;
    }
    string ldefStr(*strp, endp-*strp);
    *strp = endp + 1;
    while(*endp != '#' && *endp != '\0'){
      ++endp;
    }
    string rdefStr(*strp, endp-*strp); 
    defineFunc(funcTable, ldefStr.c_str(), rdefStr.c_str());
  }
  else if (command == "test"){
    stack<char> bracketStack;
    do{
      if (*endp == '('){
	bracketStack.push(*endp);
      }
      if (*endp == ')'){
	bracketStack.pop();
      }
      ++endp;
    }while(!bracketStack.empty());
    string ltestStr(*strp, endp-*strp);
    *strp = endp + 1;
    skipSpace(strp);
    while( *endp != '\0' && *endp != '#'){
      ++endp;
    }
    string rtestStr(*strp, endp-*strp);
    //    std::cout << "test " << ltestStr << " = " << rtestStr;
    testFunc(funcTable, ltestStr.c_str(), rtestStr.c_str());
  }
  else{
    std::cerr << "Cannot identify 'define' or 'test'.\n";
  }
  return;
}

int main(void) {

  FuncTable funcTable;
  char * line = NULL;
  size_t sz;
  while (getline(&line, &sz, stdin) != -1) {
    const char * temp = line;
    //    std::cout << "Read expression: " << line;
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
