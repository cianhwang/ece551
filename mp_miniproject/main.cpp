#include <ctype.h>
#include <cstdio>
#include <string.h>
#include <sstream>
#include <cstdlib>
#include <iostream>

#include "expr.h"

using namespace std;

void deleteMap (map<string, Expression *> &exprMap){
   for (map<string, Expression *>::iterator it = exprMap.begin(); it != exprMap.end();
         ++it) {
      delete it->second;
    }
   return;
}

class FuncTable
{
 public:
  map<string, Expression *> funcTableMap;

 public:
  void addFunc(string funcName, Expression * funcExpr) {
    funcTableMap.insert(pair<string, Expression *>(funcName, funcExpr));
  }
  Expression * operator[](string funcName) { return funcTableMap[funcName]; }
  
  int countOpNum(string op) const{
    return funcTableMap.find(op)->second->CountParaNum();
  }
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

Expression * makeExpr(FuncTable & funcTable, string op, vector<Expression *> & varVec) {
  if (op == "+") {
    return new PlusExpression(varVec[0]->clone(), varVec[1]->clone());
  }
  else if (op == "-"){
    return new MinusExpression(varVec[0]->clone(), varVec[1]->clone());
  }
  else if (op == "*"){
    return new TimesExpression(varVec[0]->clone(), varVec[1]->clone());
  }
  else if (op == "/"){
    return new DivExpression(varVec[0]->clone(), varVec[1]->clone());
  }
  else if (op == "%"){
    return new ModExpression(varVec[0]->clone(), varVec[1]->clone());
  }
  else if (op == "pow"){
    return new PowExpression(varVec[0]->clone(), varVec[1]->clone());
  }
  else if (op == "sin"){
    return new SinExpression(varVec[0]->clone());
  }
  else if (op == "cos"){
    return new CosExpression(varVec[0]->clone());
  }
  else if (op == "sqrt"){
    return new SqrtExpression(varVec[0]->clone());
  }
  else if (op == "ln"){
    return new LnExpression(varVec[0]->clone());
  }

  if (funcTable.funcTableMap.find(op) != funcTable.funcTableMap.end()) {
    Expression * temp = funcTable[op]->clone();
    map<string, Expression *> varMap;
    for (size_t i = 0; i < varVec.size();++i){
      std::stringstream out;
      out << i;
      varMap.insert(pair<string, Expression *>(out.str(), varVec[i]->clone()));
    }
    temp->assign(varMap);
    deleteMap(varMap);
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
  map<string, int> opMap;
  opMap.insert(pair<string, int>("+", 2));
  opMap.insert(pair<string, int>("-", 2));
  opMap.insert(pair<string, int>("*", 2));
  opMap.insert(pair<string, int>("/", 2));
  opMap.insert(pair<string, int>("%", 2));
  opMap.insert(pair<string, int>("pow", 2));
  opMap.insert(pair<string, int>("sin", 1));
  opMap.insert(pair<string, int>("cos", 1));
  opMap.insert(pair<string, int>("sqrt", 1));
  opMap.insert(pair<string, int>("ln", 1));
  int opNum;
  if (opMap.find(op) != opMap.end()){
    opNum = opMap.find(op)->second;
  }
  else{
    opNum = functable.countOpNum(op);
  }
  *strp = endp + 1;
  vector <Expression *> varVec;
  for (int i = 0; i < opNum; ++i){
    Expression * temp = parse(functable, strp);
    varVec.push_back(temp);
  }
  
  skipSpace(strp);
  Expression * temp2 = NULL;
  if (**strp == ')') {
    *strp = *strp + 1;
    temp2 = makeExpr(functable, op, varVec);
  }
  else {
    std::cerr << "Expected ) but found " << *strp << "\n";
  }
  for (size_t i = 0; i < varVec.size(); ++i){
    delete varVec[i];
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

bool test(Expression * lexpr, Expression * rexpr){
    return abs(lexpr->evaluate()-rexpr->evaluate()) < 0.0000000000001;
}

int main(void) {
  const char * temp = "(* (/ x -12.33) (pow zt y))";
  FuncTable funcTable;
  vector<string> paraVec;
  paraVec.push_back("x");
  paraVec.push_back("y");
  paraVec.push_back("zt");
  Expression * expr = parse(funcTable, &temp);
  //  delete temp;
  Expression * func = new FuncExpression(expr, paraVec);
  funcTable.addFunc("f", func);

  const char * temp2 = "(f (f 5.3 2.5 y) (/ 4.4 zt) 2.1)";
  Expression * expr2 = parse(funcTable, &temp2);
  Expression * func2 = new FuncExpression(expr2, paraVec);
  funcTable.addFunc("g", func2);

  map<string, Expression *> tempMap;
  tempMap.insert(pair<string, Expression *>("1", new NumExpression(5.0)));
  tempMap.insert(pair<string, Expression *>("2", new NumExpression(3.0)));
  tempMap.insert(pair<string, Expression *>("3", new NumExpression(23.12)));
  Expression * Temp = funcTable["g"]->clone();
  Temp->assign(tempMap);
  std::cout << Temp->evaluate() << std::endl;

  map<string, Expression *> tempMap2;
  tempMap2.insert(pair<string, Expression *>("1", new NumExpression(1.5)));
  tempMap2.insert(pair<string, Expression *>("2", new NumExpression(3.0)));
  tempMap2.insert(pair<string, Expression *>("3", new NumExpression(5.30)));

  Expression * Temp2 = funcTable["f"]->clone();
  Temp2->assign(tempMap2);
  std::cout << Temp2->evaluate() << std::endl;
  if (test(Temp, Temp2)){
    std::cout << "Success!\n";
  }
  delete Temp;
  delete Temp2;
  deleteMap(tempMap);
  deleteMap(tempMap2);
  /*
  for (map<string, Expression *>::iterator it = tempMap.begin(); it != tempMap.end(); ++it) {
    delete it->second;
  }
  for (map<string, Expression *>::iterator it = tempMap2.begin(); it != tempMap2.end(); ++it) {
    delete it->second;
    }*/

  return EXIT_SUCCESS;
}
