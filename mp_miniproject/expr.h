#ifndef EXPR_H
#define EXPR_H

#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using std::map;
using std::pair;
using std::string;
using std::vector;

class Expression
{
  // 5 type of Expression:
  // 1. NumberExpr. Denotes number.
  // 2. VarExpr. Denotes variable in function. have variable name(string) and value(expression).
  // 3. FuncExpression. Denotes function. Have 'calculation law', naming how to deal with the variables. (expression)
  //                                       and mapping from varibles to value.
  // 4. UnaryExpression. built-in operator which takes one parameter.
  // 5. DoubleExpression. built-in operator which takes two parameters.
 public:
  virtual ~Expression() {}
  virtual double evaluate() const = 0;
  virtual Expression * clone() const = 0; // dynamic dispatch copy
  // assign expression to the corresponding variable. string: name fo variable. Expression *: the assigned expression.
  virtual void assign(map<string, Expression *> & mapping) = 0; 
  virtual int checkType() const = 0; // check expression type. NumExpr: 1, VarExpr: 2, FuncExpr: 3, others: 0
  virtual int CountParaNum() const = 0; // count number of parameters. Eg. +: 2  sin: 1
};

class NumExpression : public Expression
{
 private:
  double num;

 public:
  NumExpression(double _num) : num(_num) {}
  NumExpression(const NumExpression & rhs) : num(rhs.num) {}
  NumExpression & operator=(const NumExpression & rhs) {
    if (this != &rhs) {
      num = rhs.num;
    }
    return *this;
  }
  virtual ~NumExpression() {
    //  std::cout << "~Numexpression.\n";
  }
  virtual double evaluate() const { return num; }
  virtual Expression * clone() const { return new NumExpression(*this); }
  virtual void assign(map<string, Expression *> & mapping) { return; }
  virtual int checkType() const { return 1; }
  virtual int CountParaNum() const { return 0; }
};

class VarExpression : public Expression
{
 private:
  string varName;
  Expression * varExpr;

 public:
  VarExpression(const string & _varName) : varName(_varName), varExpr(NULL) {}
  VarExpression(const VarExpression & rhs) : varName(rhs.varName) {
    if (rhs.varExpr != NULL) {
      varExpr = rhs.varExpr->clone();
    }
    else {
      varExpr = NULL;
    }
  }
  VarExpression & operator=(const VarExpression & rhs) {
    if (this != &rhs) {
      delete varExpr;
      varName = rhs.varName;
      varExpr = rhs.varExpr->clone();
    }
    return *this;
  }
  virtual ~VarExpression() {
    delete varExpr;
    //std::cout << "~Varexpression.\n";
  }
  virtual double evaluate() const { return varExpr->evaluate(); }
  virtual Expression * clone() const { return new VarExpression(*this); }
  virtual void assign(map<string, Expression *> & mapping) {
    if (varExpr != NULL){
      delete varExpr;
    }
    // search the map by its own name and find the corresponding value.
    varExpr = mapping.find(varName)->second->clone();
      //}
      //else if (varExpr->checkType() != 1) {
      //varExpr->assign(mapping);
      //}
    return;
  }
  virtual int checkType() const { return 2; }
  virtual int CountParaNum() const { return 0; }
};

class UnaryExpression : public Expression {
 protected:
  Expression * rhs;

 public:
 UnaryExpression(Expression *_rhs):rhs(_rhs){}

  virtual ~UnaryExpression() {
    delete rhs;
  }
  virtual void assign(map<string, Expression *> & mapping) {
    rhs->assign(mapping);
    return;
  }
  virtual int checkType() const { return 0; }
  virtual int CountParaNum() const { return 1; }
};

class SinExpression : public UnaryExpression
{
 public:
 SinExpression(Expression * _rhs) : UnaryExpression(_rhs) {}
 SinExpression(const SinExpression & uExpr) : UnaryExpression(uExpr.rhs->clone()) {}
  virtual ~SinExpression() {  }
  virtual double evaluate() const { return sin(rhs->evaluate()); }
  virtual Expression * clone() const { return new SinExpression(*this); }
};

class CosExpression : public UnaryExpression
{
 public:
 CosExpression(Expression * _rhs) : UnaryExpression(_rhs) {}
 CosExpression(const CosExpression & uExpr) : UnaryExpression(uExpr.rhs->clone()) {}
  virtual ~CosExpression() {  }
  virtual double evaluate() const { return cos(rhs->evaluate()); }
  virtual Expression * clone() const { return new CosExpression(*this); }
};

class SqrtExpression : public UnaryExpression
{
 public:
 SqrtExpression(Expression * _rhs) : UnaryExpression(_rhs) {}
 SqrtExpression(const SqrtExpression & uExpr) : UnaryExpression(uExpr.rhs->clone()) {}
  virtual ~SqrtExpression() {  }
  virtual double evaluate() const { return sqrt(rhs->evaluate()); }
  virtual Expression * clone() const { return new SqrtExpression(*this); }
};

class LnExpression : public UnaryExpression
{
 public:
 LnExpression(Expression * _rhs) : UnaryExpression(_rhs) {}
 LnExpression(const LnExpression & uExpr) : UnaryExpression(uExpr.rhs->clone()) {}
  virtual ~LnExpression() {  }
  virtual double evaluate() const { return log(rhs->evaluate()); }
  virtual Expression * clone() const { return new LnExpression(*this); }
};

class DoubleExpression : public Expression {
 protected:
  Expression * lhs;
  Expression * rhs;

 public:
 DoubleExpression(Expression *_lhs, Expression *_rhs):lhs(_lhs), rhs(_rhs){}

  virtual ~DoubleExpression() {
    delete lhs;
    delete rhs;
  }

  virtual void assign(map<string, Expression *> & mapping) {
    lhs->assign(mapping);
    rhs->assign(mapping);
    return;
  }
  virtual int checkType() const { return 0; }
  virtual int CountParaNum() const { return 2; }
};

class PlusExpression : public DoubleExpression
{
 public:
 PlusExpression(Expression * _lhs, Expression * _rhs) : DoubleExpression(_lhs, _rhs) {}
 PlusExpression(const PlusExpression & pExpr) : DoubleExpression(pExpr.lhs->clone(),pExpr.rhs->clone()) {}
  virtual ~PlusExpression() {  }
  virtual double evaluate() const { return lhs->evaluate() + rhs->evaluate(); }
  virtual Expression * clone() const { return new PlusExpression(*this); }
};

class MinusExpression : public DoubleExpression
{
 public:
 MinusExpression(Expression * _lhs, Expression * _rhs) : DoubleExpression(_lhs, _rhs) {}
 MinusExpression(const MinusExpression & pExpr) : DoubleExpression(pExpr.lhs->clone(),pExpr.rhs->clone()) {}
  virtual ~ MinusExpression() {  }
  virtual double evaluate() const { return lhs->evaluate() - rhs->evaluate(); }
  virtual Expression * clone() const { return new MinusExpression(*this); }
};

class TimesExpression : public DoubleExpression
{
 public:
 TimesExpression(Expression * _lhs, Expression * _rhs) : DoubleExpression(_lhs, _rhs) {}
 TimesExpression(const TimesExpression & pExpr) : DoubleExpression(pExpr.lhs->clone(),pExpr.rhs->clone()) {}
  virtual ~TimesExpression() {  }
  virtual double evaluate() const { return lhs->evaluate() * rhs->evaluate(); }
  virtual Expression * clone() const { return new TimesExpression(*this); }
};

class DivExpression : public DoubleExpression
{
 public:
 DivExpression(Expression * _lhs, Expression * _rhs) : DoubleExpression(_lhs, _rhs) {}
 DivExpression(const DivExpression & pExpr) : DoubleExpression(pExpr.lhs->clone(),pExpr.rhs->clone()) {}
  virtual ~DivExpression() {  }
  virtual double evaluate() const { return lhs->evaluate() / rhs->evaluate(); }
  virtual Expression * clone() const { return new DivExpression(*this); }
};

class ModExpression : public DoubleExpression
{
 public:
 ModExpression(Expression * _lhs, Expression * _rhs) : DoubleExpression(_lhs, _rhs) {}
 ModExpression(const ModExpression & pExpr) : DoubleExpression(pExpr.lhs->clone(),pExpr.rhs->clone()) {}
  virtual ~ModExpression() {  }
  virtual double evaluate() const { return std::fmod(lhs->evaluate(),rhs->evaluate()); }
  virtual Expression * clone() const { return new ModExpression(*this); }
};

class PowExpression : public DoubleExpression
{
 public:
 PowExpression(Expression * _lhs, Expression * _rhs) : DoubleExpression(_lhs, _rhs) {}
 PowExpression(const PowExpression & pExpr) : DoubleExpression(pExpr.lhs->clone(),pExpr.rhs->clone()) {}
  virtual ~PowExpression() {  }
  virtual double evaluate() const { return pow(lhs->evaluate(), rhs->evaluate()); }
  virtual Expression * clone() const { return new PowExpression(*this); }
};

class FuncExpression : public Expression
{
 private:
  Expression * funcExpr;
  map<string, Expression *> funcMap;

 public:
  FuncExpression(Expression * _funcExpr, vector<string> & varNameVec) : funcExpr(_funcExpr) {
    for (size_t i = 0; i < varNameVec.size(); ++i) {
            funcMap.insert(pair<string, Expression *>(varNameVec[i], new VarExpression(varNameVec[i])));
      //      funcMap.insert(pair<string, Expression *>(varNameVec[i], NULL));
    }
  }
  FuncExpression(const FuncExpression & rhs) : funcExpr(rhs.funcExpr->clone()) {
    for (map<string, Expression *>::const_iterator it = rhs.funcMap.begin();
         it != rhs.funcMap.end();
         ++it) {
      funcMap.insert(pair<string, Expression *>(it->first, it->second->clone()));
    }
  }
  FuncExpression & operator=(const FuncExpression & rhs) {
    if (this != &rhs) {
      delete funcExpr;
      for (map<string, Expression *>::const_iterator it = rhs.funcMap.begin();
	   it != rhs.funcMap.end();
	   ++it) {
	delete it->second;
      }

      funcExpr = rhs.funcExpr->clone();
      for (map<string, Expression *>::const_iterator it = rhs.funcMap.begin();
           it != rhs.funcMap.end();
           ++it) {
        funcMap.insert(pair<string, Expression *>(it->first, it->second->clone()));
      }
    }
    return *this;
  }
  virtual ~FuncExpression() {
    delete funcExpr;
    for (map<string, Expression *>::iterator it = funcMap.begin(); it != funcMap.end(); ++it) {
      delete it->second;
    }
  }
  virtual double evaluate() const { return funcExpr->evaluate(); }
  virtual Expression * clone() const { return new FuncExpression(*this); }
  virtual void assign(map<string, Expression *> & mapping) {
    if (mapping.find("0") != mapping.end()){ //initialize, first step, only requires the value(expression)
      for (map<string, Expression *>::iterator it = funcMap.begin(), it2 = mapping.begin(); it != funcMap.end(); ++it, ++it2) {
	  delete it->second;
	it->second = it2->second->clone();
      }
    }
    else{ //non-init, meaning giving the value to the parameters by the name.
      for (map<string, Expression *>::iterator it = funcMap.begin(); it != funcMap.end(); ++it){
	if (it->second != NULL){
	  it->second->assign(mapping);
	}
	else if(mapping.find(it->first) != mapping.end()){
	  it->second = mapping.find(it->first)->second->clone();
	}
      }
    }
    funcExpr->assign(funcMap);
    /*    for (map<string, Expression *>::iterator it = mapping.begin(); it != mapping.end(); ++it) {
      delete it->second;
      }*/
    return;
  }
  virtual int checkType() const { return 3; }
  virtual int CountParaNum() const { return funcMap.size(); }
};

#endif
