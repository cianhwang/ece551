#include "IntArray.h"

#include <assert.h>

#include <cstdio>
#include <cstring>
#include <ostream>

IntArray::IntArray() : data(NULL), numElements(0) {}
IntArray::IntArray(int n) : data(new int[n]), numElements(n) {}

IntArray::IntArray(const IntArray & rhs) :
    data(new int[rhs.numElements]),
    numElements(rhs.numElements) {
  for (int i = 0; i < numElements; ++i) {
    data[i] = rhs.data[i];
  }
}
IntArray::~IntArray() {
  delete[] data;
}

IntArray & IntArray::operator=(const IntArray & rhs) {
  if (this != &rhs) {
    int * temp = new int[rhs.numElements];
    for (int i = 0; i < rhs.numElements; ++i) {
      temp[i] = rhs.data[i];
    }
    delete[] data;
    data = temp;
    numElements = rhs.numElements;
  }
  return *this;
}
const int & IntArray::operator[](int index) const {
  assert(index < numElements);
  return data[index];
}
int & IntArray::operator[](int index) {
  assert(index < numElements);
  return data[index];
}

int IntArray::size() const {
  return numElements;
}

bool IntArray::operator==(const IntArray & rhs) const {
  bool flag = true;
  if (numElements != rhs.numElements) {
    flag = false;
  }
  for (int i = 0; i < numElements; ++i) {
    if (data[i] != rhs.data[i]) {
      flag = false;
    }
  }
  return flag;
}

bool IntArray::operator!=(const IntArray & rhs) const {
  return !((*this).operator==(rhs));
}

std::ostream & operator<<(std::ostream & s, const IntArray & rhs) {
  s.write("{", 1);
  for (int i = 0; i < rhs.size(); ++i) {
    char asString[16];
    snprintf(asString, 16, "%d", rhs[i]);
    s.write(asString, strlen(asString));
    if (i != rhs.size() - 1) {
      s.write(", ", 2);
    }
  }
  s.write("}", 1);
  return s;
}
