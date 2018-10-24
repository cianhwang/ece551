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
};

class NumExpression : public Expression
{
 private:
  long n;

 public:
  NumExpression(long num) : n(num) { stm << n; }
  virtual ~NumExpression() {}
  virtual std::string toString() const { return stm.str(); }
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
};

#endif
