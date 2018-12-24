#include "readFreq.h"

#include <stdio.h>

#include <cstdlib>
#include <fstream>

void printSym(std::ostream & s, unsigned sym) {
  if (sym > 256) {
    s << "INV";
  }
  else if (sym == 256) {
    s << "EOF";
  }
  else if (isprint(sym)) {
    char c = sym;
    s << "'" << c << "'";
  }
  else {
    std::streamsize w = s.width(3);
    s << std::hex << sym << std::dec;
    s.width(w);
  }
}
uint64_t * readFrequencies(const char * fname) {
  uint64_t * arr = new uint64_t[257];
  for (int i = 0; i < 257; ++i) {
    arr[i] = 0;
  }
  std::ifstream file(fname);
  if (!file.is_open()) {
    exit(EXIT_FAILURE);
  }
  int c;
  while ((c = file.get()) != EOF) {
    ++arr[c];
  }
  ++arr[256];
  file.close();
  return arr;
}
