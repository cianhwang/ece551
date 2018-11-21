#ifndef FUNC_H
#define FUNC_H
#include "expr.h"
#include "expr_utils.h"
#include <map>
#include <vector>
#include <cmath>
#include <sstream>

class Function{
 private:
  std::map<std::string, double> varMap;
  Expression *exp;
 public:
 Function():exp(NULL){}
 Function(std::vector<std::string> &varVec, Expression *_exp):exp(_exp){
    for (int i = 0; i < varVec.size(); ++i){
      varMap.insert(std::pair<std::string, double>(varVec[i], 0.0));
    }
  }
  void assign(std::vector<double> &paraVec){
    std::map<std::string, double>::iterator it;
    int i = 0;
    for (it = varMap.begin(); it != varMap.end(); ++it){
      it->second = paraVec[i++];
    }
    exp->setData(varMap);
  }
  double calc(){return exp->eval();}
  bool test(Expression *expRhs){
    return (abs(exp->eval()-expRhs->eval()) < 0.0000000000001);
  }
  ~Function(){ delete exp; }
};

class GenFunction{
 private:  
  std::map<std::string, Function *> funcMap;
  void parseFuncDef(const char **strp, int n, std::string &name, std::vector<std::string> &varVec){
    std::string defStr(*strp, n);
    std::stringstream ss(defStr);
    std::string temp;
    ss >> temp;
    if (temp == "("){
      ss >> temp;
    }
    if (temp[0] == '('){
      std::string tempName(temp, 1, temp.size()-1);
      name = tempName;
      std::cout << "function Name: " << name << "\n";
    }
    else{
      name = temp;
      std::cout << "function Name: " << name << "\n";
    }
    while(ss >> temp){
      if (temp == ")"){
	break;
      }
      if (temp.size() > 1 && temp[temp.size()-1] == ')'){
	std::string tempVar(temp, 0, temp.size()-1);
	varVec.push_back(tempVar);
	std::cout << "Add variable: " << tempVar << "\n";
      }
      else{
	varVec.push_back(temp);
	std::cout << "Add variable: " << temp << "\n";
      }
    }
  }
 public:

  void addFunc(std::string funcName, std::vector<std::string> &vec, Expression *exp){
    Function *newFunc = new Function(vec, exp);
    funcMap.insert(std::pair<std::string, Function *>(funcName, newFunc));
  }
  void parseFunc(const char ** strp);
  Function * operator[](std::string funcName){
    //if can find
    return funcMap[funcName];
  }
  ~GenFunction(){
    std::map<std::string, Function *>::iterator it;
    for (it = funcMap.begin(); it != funcMap.end(); ++it){
      delete it->second;
    }// delete Func * in map
  }
};

void GenFunction::parseFunc(const char ** strp){
  skipSpace(strp);
  const char *endp = *strp;
  while( *endp != '\0'){
    ++endp;
    if (*endp== ' '){
      break;
    }
  }
  std::string operation(*strp, endp-*strp);
  *strp = endp+1;
  if (operation == "define"){
    while(*endp != '='){
      ++endp;
    }
    std::string funcName;
    std::vector<std::string> vec;
    parseFuncDef(strp, endp-*strp, funcName, vec);
    const char *temp = endp+1;
    Expression *exp = parse(&temp);
    addFunc(funcName, vec, exp);
  }
  else if (operation == "test"){
    ;// need implementation
  }
  else{ // wrong input
    std::cerr << "plz check you input\n";
    return;
  }
}
#endif
