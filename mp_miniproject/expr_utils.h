#ifndef EXPR_UTILS_H
#define EXPR_UTILS_H
#include <assert.h>
#include <ctype.h>
#include <string.h>

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <sstream>
#include <stack>

#include "expr.h"

using std::map;
using std::pair;
using std::stack;
using std::string;
using std::stringstream;
using std::vector;

void deleteMap(map<string, Expression *> & exprMap) {
  for (map<string, Expression *>::iterator it = exprMap.begin(); it != exprMap.end(); ++it) {
    delete it->second;
  }
  return;
}

class FuncTable
{
 private:
  map<string, Expression *> funcTableMap;
  map<string, int> opMap;

 public:
  FuncTable() {
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
  }
  bool addFunc(string funcName, Expression * funcExpr) {
    if (funcTableMap.find(funcName) != funcTableMap.end()) {
      std::cerr << "Function " << funcName << " already exists.\n";
      //!!
      delete funcExpr;
      //      exit(EXIT_FAILURE);
      return false;
    }
    funcTableMap.insert(pair<string, Expression *>(funcName, funcExpr));
    return true;
  }
  Expression * operator[](string funcName) { return funcTableMap[funcName]; }

  int countOpNum(string op) const { return funcTableMap.find(op)->second->CountParaNum(); }
  ~FuncTable() {
    for (map<string, Expression *>::iterator it = funcTableMap.begin(); it != funcTableMap.end();
         ++it) {
      delete it->second;
    }
  }
  Expression * makeExpr(string op, vector<Expression *> & varVec);
  Expression * parseOp(const char ** strp);
  Expression * parse(const char ** strp);

  void skipSpace(const char ** strp) {
    while (isspace(**strp)) {
      *strp = *strp + 1;
    }
  }

  void parseDef(const char ** strp, string & name, vector<string> & vec);
  bool checkID(const string & id);
  void defineFunc(const char * deftemp, const char * temp);
  bool test(Expression * lexpr, Expression * rexpr);
  void testFunc(const char * ltemp, const char * rtemp);
  double calcVolumn(string funcName, double step, vector<double> & range);
  double mcVolume(string funcName, int times, vector<double> & range);
  void readInput(const char ** strp);
  void printTest(const string & lstr, const string & rstr);
  void generateRandNum(vector<double> & vec, vector<double> & range);
  void gradUpdate(bool type,
                  string funcName,
                  double gamma,
                  vector<double> & currVec,
                  vector<double> & newVec);
  double gradientMethod(bool type,
                        string funcName,
                        double gamma,
                        double convDis,
                        int rounds,
                        vector<double> & startPosVec);
};

Expression * FuncTable::makeExpr(string op, vector<Expression *> & varVec) {
  if (op == "+") {
    return new PlusExpression(varVec[0]->clone(), varVec[1]->clone());
  }
  else if (op == "-") {
    return new MinusExpression(varVec[0]->clone(), varVec[1]->clone());
  }
  else if (op == "*") {
    return new TimesExpression(varVec[0]->clone(), varVec[1]->clone());
  }
  else if (op == "/") {
    return new DivExpression(varVec[0]->clone(), varVec[1]->clone());
  }
  else if (op == "%") {
    return new ModExpression(varVec[0]->clone(), varVec[1]->clone());
  }
  else if (op == "pow") {
    return new PowExpression(varVec[0]->clone(), varVec[1]->clone());
  }
  else if (op == "sin") {
    return new SinExpression(varVec[0]->clone());
  }
  else if (op == "cos") {
    return new CosExpression(varVec[0]->clone());
  }
  else if (op == "sqrt") {
    return new SqrtExpression(varVec[0]->clone());
  }
  else if (op == "ln") {
    return new LnExpression(varVec[0]->clone());
  }

  if (funcTableMap.find(op) != funcTableMap.end()) {
    Expression * temp = funcTableMap[op]->clone();
    map<string, Expression *> varMap;
    for (size_t i = 0; i < varVec.size(); ++i) {
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

Expression * FuncTable::parseOp(const char ** strp) {
  skipSpace(strp);
  const char * endp = *strp;
  while (*endp != '\0') {
    ++endp;
    if (*endp == ' ') {
      break;
    }
  }
  std::string op(*strp, endp - *strp);

  int opNum;
  if (opMap.find(op) != opMap.end()) {
    opNum = opMap.find(op)->second;
  }
  else {
    opNum = countOpNum(op);
  }
  *strp = endp + 1;
  vector<Expression *> varVec;
  for (int i = 0; i < opNum; ++i) {
    Expression * temp = parse(strp);
    varVec.push_back(temp);
  }

  skipSpace(strp);
  Expression * temp2 = NULL;
  if (**strp == ')') {
    *strp = *strp + 1;
    temp2 = makeExpr(op, varVec);
  }
  else {
    std::cerr << "Expected ) but found " << *strp << "\n";
  }
  for (size_t i = 0; i < varVec.size(); ++i) {
    delete varVec[i];
  }

  return temp2;
}

Expression * FuncTable::parse(const char ** strp) {
  skipSpace(strp);
  if (**strp == '\0') {
    std::cerr << "End of line found mid expression!\n";
    return NULL;
  }
  else if (**strp == '(') {
    // (op id ...)
    *strp = *strp + 1;
    return parseOp(strp);
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

void FuncTable::parseDef(const char ** strp, string & name, vector<string> & vec) {
  skipSpace(strp);
  if (**strp == '\0') {
    std::cerr << "End of line found mid expression!\n";
  }
  if (**strp == '(') {
    // (id id ...)
    *strp = *strp + 1;
    skipSpace(strp);
  }
  const char * endp = *strp;
  while (*endp != ')') {
    ++endp;
  }
  string str(*strp, endp - *strp);
  std::stringstream ss(str);
  ss >> str;

  name = str;
  while (ss >> str) {
    vec.push_back(str);
  }
}

bool FuncTable::checkID(const string & id) {
  for (size_t i = 0; i < id.size(); ++i) {
    if (!isalpha(id[i])) {
      return false;
    }
  }
  return true;
}

void FuncTable::defineFunc(const char * deftemp, const char * temp) {
  vector<string> paraVec;
  string funcName;
  parseDef(&deftemp, funcName, paraVec);
  if (!checkID(funcName)) {
    std::cerr << "Id should be all letters.\n";
    return;
  }
  if (paraVec.empty()) {
    std::cerr << "At least one parameter.\n";
    return;
  }
  else {
    for (size_t i = 0; i < paraVec.size(); ++i) {
      if (!checkID(paraVec[i])) {
        std::cerr << "Id should be all letters.\n";
        return;
      }
    }
    // error handling: each para are unique.
  }
  Expression * expr = parse(&temp);
  if (expr == NULL) {
    std::cout << "Could not parse expression, please try again.\n";
    return;
  }
  // error handing: expression contains parameters that not belong to function.
  Expression * func = new FuncExpression(expr, paraVec);
  if (addFunc(funcName, func)) {
    std::cout << "defined " << funcName << "(";
    for (size_t i = 0; i < paraVec.size() - 1; ++i) {
      std::cout << paraVec[i] << " ";
    }
    std::cout << paraVec[paraVec.size() - 1] << ")\n";
  }
}

bool FuncTable::test(Expression * lexpr, Expression * rexpr) {
  return abs(lexpr->evaluate() - rexpr->evaluate()) < 0.0000000000001;
}

void FuncTable::printTest(const string & lstr, const string & rstr) {
  string temp(lstr);
  string rtemp(rstr);
  temp.erase(temp.begin());
  std::stringstream ss(temp);
  ss >> temp;
  std::cout << temp << "(";
  while (ss >> temp) {
    std::cout << temp << " ";
  }
  std::cout << "= ";
  rtemp.erase(rtemp.end() - 1);
  std::cout << rtemp;
}

void FuncTable::testFunc(const char * ltemp, const char * rtemp) {
  string lstr(ltemp);
  string rstr(rtemp);
  Expression * lExpr = parse(&ltemp);
  Expression * rExpr = parse(&rtemp);
  // error handling: check no variables
  //                 and all ids refer to function.
  printTest(lstr, rstr);
  if (test(lExpr, rExpr)) {
    std::cout << " [correct]\n";
  }
  else {
    std::cout << " [INCORRECT: expected " << lExpr->evaluate() << "]\n";
  }
  delete lExpr;
  delete rExpr;
}

double FuncTable::calcVolumn(string funcName, double step, vector<double> & range) {
  //check range is consistant with func
  double vol = 0.0;
  if (countOpNum(funcName) == 1) {
    for (double i = range[0]; i < range[1]; i += step) {
      double a = i + step;
      stringstream ss;
      ss << "(" << funcName << " " << i << ")";
      string tempStr_1(ss.str());
      const char * temp_1 = tempStr_1.c_str();
      Expression * curr_1 = parse(&temp_1);
      ss.str("");
      ss << "(" << funcName << " " << a << ")";
      string tempStr_2(ss.str());
      const char * temp_2 = tempStr_2.c_str();
      Expression * curr_2 = parse(&temp_2);
      vol += (curr_1->evaluate() + curr_2->evaluate()) / 2 * step;
      delete curr_1;
      delete curr_2;
    }
  }
  else if (countOpNum(funcName) == 2) {
    for (double i = range[0]; i < range[1]; i += step) {
      //      std::cout << "i: " << i << std::endl;
      for (double j = range[2]; j < range[3]; j += step) {
        //std::cout << "j: " << j << std::endl;
        stringstream ss;
        double a = i + step;
        double b = j + step;
        ss << "(" << funcName << " " << i << " " << j << ")";
        string tempStr_1(ss.str());
        const char * temp_1 = tempStr_1.c_str();
        Expression * curr_1 = parse(&temp_1);
        ss.str("");
        ss << "(" << funcName << " " << a << " " << j << ")";
        string tempStr_2(ss.str());
        const char * temp_2 = tempStr_2.c_str();
        Expression * curr_2 = parse(&temp_2);
        ss.str("");
        ss << "(" << funcName << " " << a << " " << b << ")";
        string tempStr_3(ss.str());
        const char * temp_3 = tempStr_3.c_str();
        Expression * curr_3 = parse(&temp_3);
        ss.str("");
        ss << "(" << funcName << " " << i << " " << b << ")";
        string tempStr_4(ss.str());
        const char * temp_4 = tempStr_4.c_str();
        Expression * curr_4 = parse(&temp_4);

        vol += (curr_1->evaluate() + curr_2->evaluate() + curr_3->evaluate() + curr_4->evaluate()) /
               4 * step * step;
        delete curr_1;
        delete curr_2;
        delete curr_3;
        delete curr_4;
      }
    }
  }
  else if (countOpNum(funcName) == 3) {
    for (double i = range[0]; i < range[1]; i += step) {
      //      std::cout << "i: " << i << std::endl;
      for (double j = range[2]; j < range[3]; j += step) {
        //std::cout << "j: " << j << std::endl;
        for (double k = range[4]; k < range[5]; k += step) {
          stringstream ss;
          double a = i + step;
          double b = j + step;
          double c = k + step;
          ss << "(" << funcName << " " << i << " " << j << " " << k << ")";
          string tempStr_1(ss.str());
          const char * temp_1 = tempStr_1.c_str();
          Expression * curr_1 = parse(&temp_1);
          ss.str("");
          ss << "(" << funcName << " " << a << " " << j << " " << k << ")";
          string tempStr_2(ss.str());
          const char * temp_2 = tempStr_2.c_str();
          Expression * curr_2 = parse(&temp_2);
          ss.str("");
          ss << "(" << funcName << " " << a << " " << b << " " << k << ")";
          string tempStr_3(ss.str());
          const char * temp_3 = tempStr_3.c_str();
          Expression * curr_3 = parse(&temp_3);
          ss.str("");
          ss << "(" << funcName << " " << i << " " << b << " " << k << ")";
          string tempStr_4(ss.str());
          const char * temp_4 = tempStr_4.c_str();
          Expression * curr_4 = parse(&temp_4);
          ss << "(" << funcName << " " << i << " " << j << " " << c << ")";
          string tempStr_5(ss.str());
          const char * temp_5 = tempStr_5.c_str();
          Expression * curr_5 = parse(&temp_5);
          ss.str("");
          ss << "(" << funcName << " " << a << " " << j << " " << c << ")";
          string tempStr_6(ss.str());
          const char * temp_6 = tempStr_6.c_str();
          Expression * curr_6 = parse(&temp_6);
          ss.str("");
          ss << "(" << funcName << " " << a << " " << b << " " << c << ")";
          string tempStr_7(ss.str());
          const char * temp_7 = tempStr_7.c_str();
          Expression * curr_7 = parse(&temp_7);
          ss.str("");
          ss << "(" << funcName << " " << i << " " << b << " " << c << ")";
          string tempStr_8(ss.str());
          const char * temp_8 = tempStr_8.c_str();
          Expression * curr_8 = parse(&temp_8);

          vol +=
              (curr_1->evaluate() + curr_2->evaluate() + curr_3->evaluate() + curr_4->evaluate() +
               curr_5->evaluate() + curr_6->evaluate() + curr_7->evaluate() + curr_8->evaluate()) /
              8 * step * step * step;
          delete curr_1;
          delete curr_2;
          delete curr_3;
          delete curr_4;
          delete curr_5;
          delete curr_6;
          delete curr_7;
          delete curr_8;
        }
      }
    }
  }
  else {
    std::cout << "Only able to cope with up to 3 parameters.\n";
  }
  return vol;
}

void FuncTable::generateRandNum(vector<double> & vec, vector<double> & range) {
  for (size_t i = 0; 2 * i < range.size(); ++i) {
    double randn = (double)rand() / RAND_MAX;
    double randnum = randn * (range[2 * i + 1] - range[2 * i]) + range[2 * i];
    vec.push_back(randnum);
  }
}

double FuncTable::mcVolume(string funcName, int times, vector<double> & range) {
  // check #para of func is according to vector
  double avgtotal = 0.0;
  srand(time(NULL));
  for (int i = 0; i < times; ++i) {
    vector<double> randnumVec;
    generateRandNum(randnumVec, range);
    stringstream ss;
    ss << "(" << funcName << " ";
    for (size_t j = 0; j < randnumVec.size(); ++j) {
      ss << randnumVec[j] << " ";
    }
    ss << ")";
    string tempStr(ss.str());
    //    std::cout << tempStr << std::endl;
    const char * temp = tempStr.c_str();
    Expression * curr = parse(&temp);
    avgtotal += curr->evaluate();
    delete curr;
  }
  double avg = avgtotal / times;
  for (size_t i = 0; 2 * i < range.size(); ++i) {
    avg *= (range[2 * i + 1] - range[2 * i]);
  }
  return avg;
}

double calcDis(const vector<double> & v1, const vector<double> & v2) {
  double dist = 0.0;
  assert(v1.size() == v2.size());
  for (size_t i = 0; i < v1.size(); ++i) {
    dist += (v1[i] - v2[i]) * (v1[i] - v2[i]);
  }
  return sqrt(dist);
}

void FuncTable::gradUpdate(bool type,
                           string funcName,
                           double gamma,
                           vector<double> & currVec,
                           vector<double> & newVec) {
  vector<double> gradVec;
  for (size_t i = 0; i < currVec.size(); ++i) {
    double gradient;
    stringstream ss;
    ss << "(" << funcName << " ";
    for (size_t j = 0; j < currVec.size(); ++j) {
      ss << currVec[j] << " ";
    }
    ss << ")";
    string tempStr(ss.str());
    //std::cout << tempStr << std::endl;
    const char * temp = tempStr.c_str();
    Expression * curr = parse(&temp);
    ss.str("");
    ss << "(" << funcName << " ";
    for (size_t j = 0; j < currVec.size(); ++j) {
      if (j != i) {
        ss << currVec[j] << " ";
      }
      else {
        double deltaX = currVec[i] + 0.001;
        ss << deltaX << " ";
      }
    }
    ss << ")";
    string tempStr2(ss.str());
    //        std::cout << tempStr2 << std::endl;
    const char * temp2 = tempStr2.c_str();
    Expression * curr2 = parse(&temp2);
    gradient = (curr2->evaluate() - curr->evaluate()) / 0.001;
    //  std::cout << "gradient = " << gradient << std::endl;
    gradVec.push_back(gradient);
    delete curr;
    delete curr2;
  }
  assert(gradVec.size() == currVec.size());
  for (size_t i = 0; i < currVec.size(); ++i) {
    double idx = 1.0;
    if (!type) {
      idx = -1.0;
    }
    newVec[i] = (currVec[i] - idx * gamma * gradVec[i]);
  }
}

double FuncTable::gradientMethod(bool type,
                                 string funcName,
                                 double gamma,
                                 double convDis,
                                 int rounds,
                                 vector<double> & startPosVec) {
  vector<double> currVec(startPosVec);
  vector<double> newVec(startPosVec);
  for (int i = 0; i < rounds; ++i) {
    // count gradient & update
    gradUpdate(type, funcName, gamma, currVec, newVec);
    //    std::cout << "new: " << newVec[0] << std::endl;
    // check distance
    if (calcDis(newVec, currVec) < convDis) {
      break;
    }
    currVec = newVec;
  }
  // use new to form a expression.
  stringstream ss;
  ss << "(" << funcName << " ";
  for (size_t j = 0; j < newVec.size(); ++j) {
    ss << newVec[j] << " ";
  }
  ss << ")";
  string tempStr(ss.str());
  std::cout << tempStr << std::endl;
  const char * temp = tempStr.c_str();
  Expression * curr = parse(&temp);
  double value = curr->evaluate();
  delete curr;
  return value;
}

void FuncTable::readInput(const char ** strp) {
  skipSpace(strp);
  if (**strp == '\0') {
    std::cerr << "End of line found mid expression!\n";
    return;
  }
  if (**strp == '#') {
    return;
  }
  const char * endp = (*strp + 1);
  while (*endp != ' ') {
    if (*endp == '\0') {
      std::cerr << "Invalid expression.\n";
      return;
    }
    ++endp;
  }
  string command(*strp, endp - *strp);
  *strp = endp + 1;
  skipSpace(strp);
  endp = *strp;
  if (command == "define") {
    while (*endp != '=') {
      if (*endp == '\0') {
        std::cerr << "Expect = but failed to find.\n";
        return;
      }
      ++endp;
    }
    string ldefStr(*strp, endp - *strp);
    *strp = endp + 1;
    while (*endp != '#' && *endp != '\0') {
      ++endp;
    }
    string rdefStr(*strp, endp - *strp);
    defineFunc(ldefStr.c_str(), rdefStr.c_str());
  }
  else if (command == "test") {
    stack<char> bracketStack;
    do {
      if (*endp == '(') {
        bracketStack.push(*endp);
      }
      if (*endp == ')') {
        bracketStack.pop();
      }
      ++endp;
    } while (!bracketStack.empty());
    string ltestStr(*strp, endp - *strp);
    *strp = endp + 1;
    skipSpace(strp);
    while (*endp != '\0' && *endp != '#') {
      ++endp;
    }
    string rtestStr(*strp, endp - *strp);
    //    std::cout << "test " << ltestStr << " = " << rtestStr;
    testFunc(ltestStr.c_str(), rtestStr.c_str());
  }
  else if (command == "numint") {
    string numintStr(*strp);
    std::stringstream numintss(numintStr);
    string funcName;
    numintss >> funcName;
    double width;
    numintss >> width;
    vector<double> rangeVec;
    double rangeTemp;
    while (numintss >> rangeTemp) {
      rangeVec.push_back(rangeTemp);
    }
    std::cout << "Volume: " << calcVolumn(funcName, width, rangeVec) << std::endl;
  }
  else if (command == "mcint") {
    string numintStr(*strp);
    std::stringstream numintss(numintStr);
    string funcName;
    numintss >> funcName;
    int times;
    numintss >> times;
    vector<double> rangeVec;
    double rangeTemp;
    while (numintss >> rangeTemp) {
      rangeVec.push_back(rangeTemp);
    }
    std::cout << "Volume: " << mcVolume(funcName, times, rangeVec) << std::endl;
  }
  else if (command == "min" || command == "max") {
    string gdStr(*strp);
    std::stringstream gdss(gdStr);
    string funcName;
    gdss >> funcName;
    double gamma;
    gdss >> gamma;
    double convergedDistance;
    gdss >> convergedDistance;
    int steps;
    gdss >> steps;
    vector<double> startPosVec;
    double startPos;
    while (gdss >> startPos) {
      startPosVec.push_back(startPos);
    }

    if (command == "min") {
      std::cout << "Minimum: "
                << gradientMethod(true, funcName, gamma, convergedDistance, steps, startPosVec)
                << std::endl;
    }
    else {
      std::cout << "Maximum: "
                << gradientMethod(false, funcName, gamma, convergedDistance, steps, startPosVec)
                << std::endl;
    }
  }
  else {
    std::cerr << "Cannot recognize the command.\n";
  }
  return;
}

#endif
