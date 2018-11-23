#ifndef EXPR_H
#define EXPR_H

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
 public:
  virtual ~Expression() {}
  virtual double evaluate() const = 0;
  virtual Expression * clone() const = 0;
  virtual void assign(map<string, Expression *> & mapping) = 0;
  virtual bool isNumExpr() const = 0;
  virtual int CountParaNum() const = 0;
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
  virtual bool isNumExpr() const { return true; }
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
  /*  VarExpression & operator=(const VarExpression & rhs) {
    if (this != &rhs) {
      // delete
      varName = rhs.varName;
      varExpr = rhs.varExpr->clone();
    }
    return *this;
    }*/
  virtual ~VarExpression() {
    delete varExpr;
    //std::cout << "~Varexpression.\n";
  }
  virtual double evaluate() const { return varExpr->evaluate(); }
  virtual Expression * clone() const { return new VarExpression(*this); }
  virtual void assign(map<string, Expression *> & mapping) {
    if (varExpr == NULL) {
      varExpr = mapping.find(varName)->second->clone();
    }
    else if (!(varExpr->isNumExpr())) {
      varExpr->assign(mapping);
    }
    return;
  }
  virtual bool isNumExpr() const { return false; }
  virtual int CountParaNum() const { return 0; }
};

class PlusExpression : public Expression
{
 private:
  Expression * lhs;
  Expression * rhs;

 public:
  PlusExpression(Expression * _lhs, Expression * _rhs) : lhs(_lhs), rhs(_rhs) {}
  PlusExpression(const PlusExpression & pExpr) : lhs(pExpr.lhs->clone()), rhs(pExpr.rhs->clone()) {}
  virtual ~PlusExpression() {
    delete lhs;
    delete rhs;
  }
  virtual double evaluate() const { return lhs->evaluate() + rhs->evaluate(); }
  virtual Expression * clone() const { return new PlusExpression(*this); }
  virtual void assign(map<string, Expression *> & mapping) {
    lhs->assign(mapping);
    rhs->assign(mapping);
    return;
  }
  virtual bool isNumExpr() const { return false; }
  virtual int CountParaNum() const { return 2; }
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
    }
  }
  FuncExpression(const FuncExpression & rhs) : funcExpr(rhs.funcExpr->clone()) {
    for (map<string, Expression *>::const_iterator it = rhs.funcMap.begin();
         it != rhs.funcMap.end();
         ++it) {
      funcMap.insert(pair<string, Expression *>(it->first, it->second->clone()));
    }
  }
  /*  FuncExpression & operator=(const FuncExpression & rhs) {
    if (this != &rhs) {
      //delete!
      funcExpr = rhs.funcExpr->clone();
      for (map<string, Expression *>::const_iterator it = rhs.funcMap.begin();
           it != rhs.funcMap.end();
           ++it) {
        funcMap.insert(pair<string, Expression *>(it->first, it->second->clone()));
      }
    }
    return *this;
    }*/
  virtual ~FuncExpression() {
    delete funcExpr;
    for (map<string, Expression *>::iterator it = funcMap.begin(); it != funcMap.end(); ++it) {
      delete it->second;
    }
  }
  virtual double evaluate() const { return funcExpr->evaluate(); }
  virtual Expression * clone() const { return new FuncExpression(*this); }
  virtual void assign(map<string, Expression *> & mapping) {
    for (map<string, Expression *>::iterator it = funcMap.begin(); it != funcMap.end(); ++it) {
      it->second->assign(mapping);
    }
    funcExpr->assign(mapping);
    /*    for (map<string, Expression *>::iterator it = mapping.begin(); it != mapping.end(); ++it) {
      delete it->second;
      }*/

    return;
  }
  virtual bool isNumExpr() const { return false; }
  virtual int CountParaNum() const { return funcMap.size(); }
};

#endif
