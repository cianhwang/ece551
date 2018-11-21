#ifndef EXPR_UTILS_H
#define EXPR_UTILS_H


#include "expr.h"
#include <iostream>
#include <algorithm>

Expression * parse(const char ** strp);

void skipSpace(const char ** strp) {
  while(isspace(**strp)) {
    *strp = *strp + 1;
  }
}
Expression * makeUnaryExpr(char op, Expression * exp) {
  switch(op) {
  case 'i':
    return new SinExpression(exp);
  case 'o':
    return new CosExpression(exp);
  case 'q':
    return new SqrtExpression(exp);
  case 'n':
    return new LnExpression(exp);
  default:
    std::cerr << "Impossible op char: " << op << "\n";
    abort();
  }
}
Expression * makeExpr(char op, Expression * lhs, Expression * rhs) {
  switch(op) {
  case '+':
    return new PlusExpression(lhs,rhs);
  case '-':
    return new MinusExpression(lhs,rhs);
  case '*':
    return new TimesExpression(lhs,rhs);
  case '/':
    return new DivExpression(lhs,rhs);
  case '%':
    return new ModExpression(lhs,rhs);
  case 'p':
    return new PowExpression(lhs, rhs);
  default:
    std::cerr << "Impossible op char: " << op << "\n";
    abort();
  }
}
bool isValidOp(char c) {
  return strchr("+-*/%p", c) != NULL;
}

bool isValidBuiltIn(std::string op){
  std::vector<std::string> bio;
  bio.push_back("sin");
  bio.push_back("cos"); 
  bio.push_back("pow"); 
  bio.push_back("ln");
  bio.push_back("sqrt");
  bio.push_back("+");
  bio.push_back("-");
  bio.push_back("*");
  bio.push_back("/");
  bio.push_back("%");
  return (std::find(bio.begin(), bio.end(), op) != bio.end());
}


Expression * parseOp(const char ** strp) {
  skipSpace(strp);
//built-in function, Unary
  const char *endp = *strp;
  while( *endp != '\0'){
    ++endp;
    if (*endp== ' '){
      break;
    }
  }
  std::string built_in_op(*strp, endp-*strp);
  if (!isValidBuiltIn(built_in_op)){
    std::cerr << "op invalid." << "\n";
    return NULL;
  }
  *strp = endp + 1;
  if (!isValidOp(built_in_op[0])){
    char bio_short = built_in_op[1];

    Expression *exp = parse(strp);
    if (exp == NULL){
      return NULL;
    }
    skipSpace(strp);
    if (**strp == ')') {
      *strp = *strp + 1;
      return makeUnaryExpr(bio_short,exp);
    }
    std::cerr << "expected )" << std::endl;
    delete exp;
    return NULL;
  }
  else{
    char op = built_in_op[0];
    Expression * lhs = parse(strp);
    if (lhs == NULL) {
      return NULL;
    }
    Expression * rhs = parse(strp);
    if (rhs == NULL) {
      delete lhs;
      return NULL;
    }
    skipSpace(strp);
    if (**strp == ')') {
      *strp = *strp + 1;
      return makeExpr(op,lhs,rhs);
    }
    std::cerr <<"Expected ) but found " << *strp << "\n";
    delete lhs;
    delete rhs;
    return NULL;
  }
}

Expression * parse(const char ** strp) {
  skipSpace(strp);
  if (**strp == '\0') {
    std::cerr << "End of line found mid expression!\n";
    return NULL;
  }
  else if (**strp == '(') {
    // (op E E)
    *strp = *strp + 1;
    return parseOp(strp);
    // CAUTION: (op E)
  }
  else if (isalpha(**strp)){
    //variable
    const char * endp = *strp;
    while(isalpha(*endp)){
      ++endp;
    }
    std::string tempStr(*strp, endp-*strp);
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

#endif
