#ifndef EXPR_H
#define EXPR_H
#include <sstream>
#include <string>

class Expression
{
 protected:
  std::ostringstream stm;

 public:
  virtual ~Expression() {}
  virtual std::string toString() const = 0;
  virtual long evaluate() const = 0;
};

class NumExpression : public Expression
{
 private:
  long n;

 public:
  NumExpression(long num) : n(num) { stm << n; }
  virtual ~NumExpression() {}
  virtual std::string toString() const { return stm.str(); }
  virtual long evaluate() const { return n; }
};

class SignExpression : public Expression
{
 protected:
  Expression * _lhs;
  Expression * _rhs;
  std::string sign;

 public:
  SignExpression(Expression * lhs, Expression * rhs, std::string sgn) :
      _lhs(lhs),
      _rhs(rhs),
      sign(sgn) {
    stm << "(" << _lhs->toString() << sign << _rhs->toString() << ")";
  }
  virtual ~SignExpression() {
    delete _lhs;
    delete _rhs;
  }
  virtual std::string toString() const { return stm.str(); }
};

class PlusExpression : public SignExpression
{
 public:
  PlusExpression(Expression * lhs, Expression * rhs) : SignExpression(lhs, rhs, " + ") {}
  virtual ~PlusExpression() {}
  virtual long evaluate() const { return _lhs->evaluate() + _rhs->evaluate(); }
};

class MinusExpression : public SignExpression
{
 public:
  MinusExpression(Expression * lhs, Expression * rhs) : SignExpression(lhs, rhs, " - ") {}
  virtual ~MinusExpression() {}
  virtual long evaluate() const { return _lhs->evaluate() - _rhs->evaluate(); }
};

class TimesExpression : public SignExpression
{
 public:
  TimesExpression(Expression * lhs, Expression * rhs) : SignExpression(lhs, rhs, " * ") {}
  virtual ~TimesExpression() {}
  virtual long evaluate() const { return _lhs->evaluate() * _rhs->evaluate(); }
};

class DivExpression : public SignExpression
{
 public:
  DivExpression(Expression * lhs, Expression * rhs) : SignExpression(lhs, rhs, " / ") {}
  virtual ~DivExpression() {}
  virtual long evaluate() const { return _lhs->evaluate() / _rhs->evaluate(); }
};

#endif
