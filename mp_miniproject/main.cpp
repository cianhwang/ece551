#include <ctype.h>
#include <cstdio>
#include <string.h>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <stack>
#include "expr_utils.h"

using std::string;
using std::vector;
using std::map;
using std::pair;
using std::stack;
using std::stringstream;

void parseDef(const char **strp, string &name, vector<string> &vec){
  skipSpace(strp);
  if (**strp == '\0') {
    std::cerr << "End of line found mid expression!\n";
  }
  if (**strp == '(') {
    // (id id ...)
    *strp = *strp + 1;
    skipSpace(strp);
  }
  const char *endp = *strp;
  while(*endp != ')'){
    ++endp;
  }
  string str(*strp, endp-*strp);
  std::stringstream ss(str);
  ss >> str;

  name = str;
  while(ss >> str){
    vec.push_back(str);
  }
}

bool checkID(const string &id){
  for (size_t i = 0; i < id.size(); ++i){
    if (!isalpha(id[i])){
      return false;
    }
  } 
  return true;
}

void defineFunc(FuncTable &funcTable, const char *deftemp, const char *temp){
  vector<string> paraVec;
  string funcName;
  parseDef(&deftemp, funcName, paraVec);
  if (!checkID(funcName)){
    std::cerr << "Id should be all letters.\n";
    return;
  }
  if (paraVec.empty()){
    std::cerr << "At least one parameter.\n";
    return;
  }
  else{
    for (size_t i = 0; i < paraVec.size(); ++i){
      if (!checkID(paraVec[i])){
	std::cerr << "Id should be all letters.\n";
	return;
      }
    }
    // error handling: each para are unique.
  }
  Expression *expr = parse(funcTable, &temp);
  if (expr == NULL){
    std::cout << "Could not parse expression, please try again.\n";
    return;
  }
  // error handing: expression contains parameters that not belong to function.
  Expression *func = new FuncExpression(expr, paraVec);
  if (funcTable.addFunc(funcName, func)){
    std::cout << "defined " << funcName << "(";
    for (size_t i = 0; i < paraVec.size()-1; ++i){
      std::cout << paraVec[i] << " ";
    }
    std::cout << paraVec[paraVec.size()-1] << ")\n";
  }
}

bool test(Expression * lexpr, Expression * rexpr){
  return abs(lexpr->evaluate()-rexpr->evaluate()) < 0.0000000000001;
}

void printTest(const string &lstr, const string &rstr){
  string temp(lstr);
  string rtemp(rstr);
  temp.erase(temp.begin());
  std::stringstream ss(temp);
  ss >> temp;
  std::cout << temp << "(";
  while (ss >> temp){
    std::cout << temp << " ";
  }
  std::cout << "= ";
  rtemp.erase(rtemp.end()-1);
  std::cout << rtemp;
}

void testFunc(FuncTable &funcTable, const char *ltemp, const char *rtemp){
  string lstr(ltemp);
  string rstr(rtemp);
  Expression *lExpr = parse(funcTable, &ltemp);
  Expression *rExpr = parse(funcTable, &rtemp);
  // error handling: check no variables 
  //                 and all ids refer to function.
  printTest(lstr, rstr);
  if (test(lExpr, rExpr)){
    std::cout << " [correct]\n";
  }
  else{
    std::cout << " [INCORRECT: expected "<< lExpr->evaluate() <<"]\n";
  }
  delete lExpr;
  delete rExpr;
}

double calcVolumn(FuncTable &funcTable, string funcName, double step, vector<double> &range){
  //check range is consistant with func
  double vol = 0.0;
  if (funcTable.countOpNum(funcName) == 1){
    for (double i = range[0]; i < range[1]; i += step){
      stringstream ss;
      ss << "(" << funcName << " " << i << ")";
      const char *temp_1 = ss.str().c_str();
      Expression *curr_1 = parse(funcTable, &temp_1);
      ss.str("");
      ss << "(" << funcName << " " << i+step << ")";
      const char *temp_2 = ss.str().c_str();
      Expression *curr_2 = parse(funcTable, &temp_2);
      vol += (curr_1->evaluate()+curr_2->evaluate())/2*step;
      delete curr_1;
      delete curr_2;
    }
  }
  else if (funcTable.countOpNum(funcName) == 2){
    for (double i = range[0]; i < range[1]; i += step){
      //      std::cout << "i: " << i << std::endl;
      for (double j = range[2]; j < range[3]; j += step){
	//	std::cout << "j: " << j << std::endl;
	stringstream ss;
	ss << "(" << funcName << " " << i  << " " << j << ")";
	const char *temp_1 = ss.str().c_str();
	Expression *curr_1 = parse(funcTable, &temp_1);
	ss.str("");
	ss << "(" << funcName << " " << i+step << " " << j << ")";
	const char *temp_2 = ss.str().c_str();
	Expression *curr_2 = parse(funcTable, &temp_2);
	ss.str("");
	ss << "(" << funcName << " " << i+step << " " << j+step << ")";
	const char *temp_3 = ss.str().c_str();
	Expression *curr_3 = parse(funcTable, &temp_3);
	ss.str("");
	ss << "(" << funcName << " " << i << " " << j+step << ")";
	const char *temp_4 = ss.str().c_str();
	Expression *curr_4 = parse(funcTable, &temp_4);

	vol += (curr_1->evaluate()+curr_2->evaluate()
		+ curr_3->evaluate()+curr_4->evaluate())/4*step*step;
	delete curr_1;
	delete curr_2;
	delete curr_3;
	delete curr_4;
      }
    }
  }
  else{
    std::cout << "too complex..\n";
  }
  return vol;
}

void readInput(FuncTable &funcTable, const char **strp){
  skipSpace(strp);
  if (**strp == '\0') {
    std::cerr << "End of line found mid expression!\n";
    return;
  }
  if (**strp == '#'){
    return;
  }
  const char *endp = (*strp + 1);
  while(*endp != ' '){
    if (*endp == '\0'){
      std::cerr << "Invalid expression.\n";
      return;
    }
    ++endp;
  }
  string command(*strp, endp-*strp);
  *strp = endp + 1;
  skipSpace(strp);
  endp = *strp;
  if (command == "define"){
    while(*endp != '='){
      if (*endp == '\0'){
	std::cerr << "Expect = but failed to find.\n";
	return;
      }
      ++endp;
    }
    string ldefStr(*strp, endp-*strp);
    *strp = endp + 1;
    while(*endp != '#' && *endp != '\0'){
      ++endp;
    }
    string rdefStr(*strp, endp-*strp); 
    defineFunc(funcTable, ldefStr.c_str(), rdefStr.c_str());
  }
  else if (command == "test"){
    stack<char> bracketStack;
    do{
      if (*endp == '('){
	bracketStack.push(*endp);
      }
      if (*endp == ')'){
	bracketStack.pop();
      }
      ++endp;
    }while(!bracketStack.empty());
    string ltestStr(*strp, endp-*strp);
    *strp = endp + 1;
    skipSpace(strp);
    while( *endp != '\0' && *endp != '#'){
      ++endp;
    }
    string rtestStr(*strp, endp-*strp);
    //    std::cout << "test " << ltestStr << " = " << rtestStr;
    testFunc(funcTable, ltestStr.c_str(), rtestStr.c_str());
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
    while(numintss >> rangeTemp){
      rangeVec.push_back(rangeTemp);
    }
    std::cout << "Volumn: " << calcVolumn(funcTable, funcName, width, rangeVec)<< std::endl;
  }
  else{
    std::cerr << "Cannot recognize the command.\n";
  }
  return;
}

int main(void) {

  FuncTable funcTable;
  char * line = NULL;
  size_t sz;
  while (getline(&line, &sz, stdin) != -1) {
    const char * temp = line;
    //    std::cout << "Read expression: " << line;
    readInput(funcTable, &temp);
  }
  free(line);

  /*  
  const char * temp = "(+ x y)";
  const char * deftemp = "(f x y)";
  FuncTable funcTable;
  defineFunc(funcTable, deftemp, temp);

  const char * temp2 = "(f (f a 1) (f 1 b))";
  const char * deftemp2 = "(g a b)";
  defineFunc(funcTable, deftemp2, temp2);

  const char * temp3 = "(g 5.0 3.0)";
  Expression *Temp = parse(funcTable, &temp3);

  std::cout << Temp->evaluate() << std::endl;


  const char * temp4 = "(f 1.5 5.3)";
  Expression *Temp2 = parse(funcTable, &temp4);

  std::cout << Temp2->evaluate() << std::endl;

  delete Temp;
  delete Temp2;
  */
  return EXIT_SUCCESS;
}
