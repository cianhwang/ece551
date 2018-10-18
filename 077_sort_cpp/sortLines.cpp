#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using std::string;

int main() {
  std::vector<string> v;
  string s;
  bool flag = false;
  while (std::getline(std::cin, s)) {
    v.push_back(s);
    if (!flag) {
      flag = true;
    }
  }
  if (!flag) {
    std::cerr << "ERROR!" << std::endl;
  }

  std::sort(v.begin(), v.end());
  for (std::vector<string>::const_iterator it = v.begin(); it != v.end(); ++it) {
    std::cout << *it << std::endl;
  }

  return EXIT_SUCCESS;
}
