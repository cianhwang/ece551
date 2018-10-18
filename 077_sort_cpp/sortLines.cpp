#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::string;

int main(int argc, char ** argv) {
  if (argc < 1) {
    std::cerr << "ERROR." << std::endl;
  }
  if (argc == 1) {
    std::vector<string> v;
    string s;
    while (std::getline(std::cin, s)) {
      v.push_back(s);
    }

    std::sort(v.begin(), v.end());
    for (std::vector<string>::const_iterator it = v.begin(); it != v.end(); ++it) {
      std::cout << *it << std::endl;
    }
  }
  else {
    for (int i = 1; i < argc; ++i) {
      std::vector<string> v;
      string s;
      std::ifstream inFile;
      inFile.open(argv[i]);
      while (std::getline(inFile, s)) {
        v.push_back(s);
      }
      std::sort(v.begin(), v.end());
      for (std::vector<string>::const_iterator it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << std::endl;
      }
      inFile.close();
    }
  }
  return EXIT_SUCCESS;
}
