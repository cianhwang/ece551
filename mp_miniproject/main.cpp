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
    defineFunc(funcTable, defStr.c_str(), *strp);
  }
  else if (command == "test"){
    string ltestStr(*strp, endp-*strp);
    *strp = endp + 1;
    testFunc(funcTable, ltestStr.c_str(), *strp);
  }
}

int main(void) {
  const char * temp = "(* (/ x -12.33) (pow zt y))";
  const char * deftemp = "( f x y zt )";
  FuncTable funcTable;
  defineFunc(funcTable, deftemp, temp);
  /*
  vector<string> paraVec;
  paraVec.push_back("x");
  paraVec.push_back("y");
  paraVec.push_back("zt");

  defineFunc(funcTable, "f", paraVec, temp);*/


  const char * temp2 = "(f (f 5.3 2.5 y) (/ 4.4 zt) 2.1)";
  const char * deftemp2 = "(g x y zt)";
  defineFunc(funcTable, deftemp2, temp2);

  /*  map<string, Expression *> tempMap;
  tempMap.insert(pair<string, Expression *>("1", new NumExpression(5.0)));
  tempMap.insert(pair<string, Expression *>("2", new NumExpression(3.0)));
  tempMap.insert(pair<string, Expression *>("3", new NumExpression(23.12)));*/
  //  Expression * Temp = funcTable["g"]->clone;
  const char * temp3 = "(g 5.0 3.0 23.12)";
  Expression *Temp = parse(funcTable, &temp3);
  //  Temp->assign(tempMap);
  std::cout << Temp->evaluate() << std::endl;

  /*  map<string, Expression *> tempMap2;
  tempMap2.insert(pair<string, Expression *>("1", new NumExpression(1.5)));
  tempMap2.insert(pair<string, Expression *>("2", new NumExpression(3.0)));
  tempMap2.insert(pair<string, Expression *>("3", new NumExpression(5.30)));
  */
  //Expression * Temp2 = funcTable["f"]->clone();
  const char * temp4 = "(f 1.5 3.0 5.3)";
  Expression *Temp2 = parse(funcTable, &temp4);
  //Temp2->assign(tempMap2);
  std::cout << Temp2->evaluate() << std::endl;
  if (test(Temp, Temp2)){
    std::cout << "Success!\n";
  }
  delete Temp;
  delete Temp2;
  //  deleteMap(tempMap);
  //  deleteMap(tempMap2);
  /*
  for (map<string, Expression *>::iterator it = tempMap.begin(); it != tempMap.end(); ++it) {
    delete it->second;
  }
  for (map<string, Expression *>::iterator it = tempMap2.begin(); it != tempMap2.end(); ++it) {
    delete it->second;
    }*/

  return EXIT_SUCCESS;
}
