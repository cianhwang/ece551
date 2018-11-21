#ifndef EXPR_H
#define EXPR_H

#include <string>
#include <cmath>
#include <map>
#include <vector>
#include <iostream>
#include <assert.h>

using std::map;
using std::string;
using std::vector;

class Expression{
 public:
  virtual double eval() const = 0;
  virtual ~Expression(){}
  virtual void setData(map<string, double> &varMap) = 0;
};

class VarExpression: public Expression{
 private:
  std::string varName;
  std::map<std::string, double> varMap;
 public:
 VarExpression(std::string _varName):varName(_varName){ 
    //    std::cout << "VarExpression::varName: " << varName << "\n";
 }
  virtual double eval() const {
    std::map<std::string, double>::const_iterator it;
    it = varMap.find(varName);
    assert(it != varMap.end());
    return it->second;
  }
  virtual void setData(map<string, double> &_varMap){
    varMap = _varMap;
  }
  virtual ~VarExpression(){}
};

class NumExpression: public Expression{
 private:
  double num;
 public:
 NumExpression(double _num):num(_num){}
  virtual double eval() const {return num;}
  virtual ~NumExpression(){}
  virtual void setData(map<string, double> &_varMap){
    return;
  }
};

class UnaryExpression : public Expression{
 protected:
  Expression *exp;
  std::string built_in;
 public:
 UnaryExpression():exp(NULL){}
 UnaryExpression(Expression *_exp, std::string _built_in):exp(_exp), built_in(_built_in){}
  virtual ~UnaryExpression(){
    delete exp;
  }
 virtual void setData(map<string, double> &_varMap){
   exp->setData(_varMap);
  }
};

class DoubleExpression : public Expression{
 protected:
  Expression *lhs;
  Expression *rhs;
  std::string sign;
 public:
 DoubleExpression():lhs(NULL), rhs(NULL){}
 DoubleExpression(Expression *_lhs, Expression *_rhs, std::string _sign):lhs(_lhs), rhs(_rhs), sign(_sign){}
  virtual ~DoubleExpression(){
    delete lhs;
    delete rhs;
  }
 virtual void setData(map<string, double> &_varMap){
   lhs->setData(_varMap);
   rhs->setData(_varMap);
  }
};

class PlusExpression : public DoubleExpression{
 public:
 PlusExpression(Expression *_lhs, Expression *_rhs):DoubleExpression(_lhs, _rhs, "+"){}
  virtual double eval() const {return lhs->eval() + rhs->eval();}
  virtual ~PlusExpression(){}
};

class MinusExpression : public DoubleExpression{
 public:
 MinusExpression(Expression *_lhs, Expression *_rhs):DoubleExpression(_lhs, _rhs, "-"){}
  virtual double eval() const {return lhs->eval() - rhs->eval();}
  virtual ~MinusExpression(){}
};

class TimesExpression : public DoubleExpression{
 public:
 TimesExpression(Expression *_lhs, Expression *_rhs):DoubleExpression(_lhs, _rhs, "*"){}
  virtual double eval() const {return lhs->eval() * rhs->eval();}
  virtual ~TimesExpression(){}
};

class DivExpression : public DoubleExpression{
 public:
 DivExpression(Expression *_lhs, Expression *_rhs):DoubleExpression(_lhs, _rhs, "/"){}
  virtual double eval() const {return lhs->eval() / rhs->eval();}
  virtual ~DivExpression(){}
};

class ModExpression : public DoubleExpression{
 public:
 ModExpression(Expression *_lhs, Expression *_rhs):DoubleExpression(_lhs, _rhs, "%"){}
  virtual double eval() const {
    //    if (std::fmod())
    return std::fmod(lhs->eval(),rhs->eval());
  }
  virtual ~ModExpression(){}
};

class SinExpression : public UnaryExpression{
 public:
 SinExpression(Expression *_exp):UnaryExpression(_exp, "sin"){}
  virtual double eval() const {return sin(exp->eval());}
  virtual ~SinExpression(){}
};

class CosExpression : public UnaryExpression{
 public:
 CosExpression(Expression *_exp):UnaryExpression(_exp, "cos"){}
  virtual double eval() const {return cos(exp->eval());}
  virtual ~CosExpression(){}
};

class SqrtExpression : public UnaryExpression{
 public:
 SqrtExpression(Expression *_exp):UnaryExpression(_exp, "sqrt"){}
  virtual double eval() const {return sqrt(exp->eval());}
  virtual ~SqrtExpression(){}
};

class LnExpression : public UnaryExpression{
 public:
 LnExpression(Expression *_exp):UnaryExpression(_exp, "ln"){}
  virtual double eval() const {return log(exp->eval());}
  virtual ~LnExpression(){}
};

class PowExpression : public DoubleExpression{
 public:
 PowExpression(Expression *_lhs, Expression *_rhs):DoubleExpression(_lhs, _rhs, "pow"){}
  virtual double eval() const {return pow(lhs->eval(), rhs->eval());}
  virtual ~PowExpression(){}
};

class FuncExpression: public Expression{
 private:
  std::string funcName;
  Expression *calcLaw;
  std::map<std::string, Expression *> exprVec;
 public:
 FuncExpression(std::vector<Expression *> _exprVec):exprVec(_exprVec){}
  virtual double eval() const {
    return calcLaw->eval();
  }
  virtual void setData(){
    for (int i = 0; i < exprVec.size(); ++i){
      exprVec[i]->setData();
    }
  }
  virtual ~FuncExpression(){
    for (int i = 0; i < exprVec.size(); ++i){
      delete exprVec[i];
    }
  }
}

#endif
