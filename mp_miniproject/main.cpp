#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include <cstdlib>
#include <iostream>

#include "expr.h"

using namespace std;

class FuncTable
{
 public:
  map<string, Expression *> funcTableMap;

 public:
  void addFunc(string funcName, Expression * funcExpr) {
    funcTableMap.insert(pair<string, Expression *>(funcName, funcExpr));
  }
  Expression * operator[](string funcName) { return funcTableMap[funcName]; }
  ~FuncTable() {
    for (map<string, Expression *>::iterator it = funcTableMap.begin(); it != funcTableMap.end();
         ++it) {
      delete it->second;
    }
  }
};

Expression * parse(FuncTable & functable, const char ** strp);

void skipSpace(const char ** strp) {
  while (isspace(**strp)) {
    *strp = *strp + 1;
  }
}

Expression * makeExpr(FuncTable & funcTable, string op, map<string, Expression *> & varMap) {
  if (op == "+") {
    return new PlusExpression(varMap["x"]->clone(), varMap["y"]->clone());
  }
  if (funcTable.funcTableMap.find(op) != funcTable.funcTableMap.end()) {
    Expression * temp = funcTable[op]->clone();
    temp->assign(varMap);
    return temp;
  }
  return NULL;
}

Expression * parseOp(FuncTable & functable, const char ** strp) {
  skipSpace(strp);
  const char * endp = *strp;
  while (*endp != '\0') {
    ++endp;
    if (*endp == ' ') {
      break;
    }
  }
  std::string op(*strp, endp - *strp);
  //	int opNum = 2;
  *strp = endp + 1;
  map<string, Expression *> opExprMap;
  Expression * temp = parse(functable, strp);
  opExprMap.insert(pair<string, Expression *>("x", temp));
  temp = parse(functable, strp);
  opExprMap.insert(pair<string, Expression *>("y", temp));
  skipSpace(strp);
  Expression * temp2 = NULL;
  if (**strp == ')') {
    *strp = *strp + 1;
    temp2 = makeExpr(functable, op, opExprMap);
  }
  else {
    std::cerr << "Expected ) but found " << *strp << "\n";
  }
  for (map<string, Expression *>::iterator it = opExprMap.begin(); it != opExprMap.end(); ++it) {
    delete it->second;
  }

  return temp2;
}

Expression * parse(FuncTable & functable, const char ** strp) {
  skipSpace(strp);
  if (**strp == '\0') {
    std::cerr << "End of line found mid expression!\n";
    return NULL;
  }
  else if (**strp == '(') {
    // (op id ...)
    *strp = *strp + 1;
    return parseOp(functable, strp);
  }
  else if (isalpha(**strp)) {
    //variable
    const char * endp = *strp;
    while (isalpha(*endp)) {
      ++endp;
    }
    std::string tempStr(*strp, endp - *strp);
    *strp = endp;
    return new VarExpression(tempStr);
  }
  else {
    //number
    char * endp;
    double num = strtod(*strp, &endp);
    if (endp == *strp) {
      std::cerr << "Expected a number, but found " << *strp << "\n";
      return NULL;
    }
    *strp = endp;
    return new NumExpression(num);
  }
}

int main(void) {
  const char * temp = "(+ x y)";
  FuncTable funcTable;
  vector<string> paraVec;
  paraVec.push_back("x");
  paraVec.push_back("y");
  Expression * expr = parse(funcTable, &temp);
  //  delete temp;
  Expression * func = new FuncExpression(expr, paraVec);
  funcTable.addFunc("f", func);

  const char * temp2 = "(+ (f 1 x) x)";
  Expression * expr2 = parse(funcTable, &temp2);
  Expression * func2 = new FuncExpression(expr2, paraVec);
  funcTable.addFunc("g", func2);

  map<string, Expression *> tempMap;
  tempMap.insert(pair<string, Expression *>("x", new NumExpression(5.0)));
  tempMap.insert(pair<string, Expression *>("y", new NumExpression(3.0)));
  Expression * Temp = funcTable["g"]->clone();
  Temp->assign(tempMap);
  std::cout << Temp->evaluate() << std::endl;
  delete Temp;
  map<string, Expression *> tempMap2;
  tempMap2.insert(pair<string, Expression *>("x", new NumExpression(12.33)));
  tempMap2.insert(pair<string, Expression *>("y", new NumExpression(3.3)));
  Expression * Temp2 = funcTable["f"]->clone();
  Temp2->assign(tempMap2);
  std::cout << Temp2->evaluate() << std::endl;
  delete Temp2;

  for (map<string, Expression *>::iterator it = tempMap.begin(); it != tempMap.end(); ++it) {
    delete it->second;
  }
  for (map<string, Expression *>::iterator it = tempMap2.begin(); it != tempMap2.end(); ++it) {
    delete it->second;
  }

  return EXIT_SUCCESS;
}
