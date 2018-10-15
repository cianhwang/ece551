#include "IntMatrix.h"

IntMatrix::IntMatrix() : numRows(0), numColumns(0), rows(new IntArray *) {
  *rows = NULL;
  std::cout << "Default Init." << std::endl;
}
IntMatrix::IntMatrix(int r, int c) : numRows(r), numColumns(c), rows(new IntArray *) {
  *rows = new IntArray[r];
  for (int i = 0; i < r; ++i) {
    (*rows)[i] = IntArray(c);
  }
  std::cout << "2nd Init." << std::endl;
}
IntMatrix::IntMatrix(const IntMatrix & rhs) :
    numRows(rhs.numRows),
    numColumns(rhs.numColumns),
    rows(new IntArray *) {
  *rows = new IntArray[numRows];
  for (int i = 0; i < rhs.numRows; ++i) {
    (*rows)[i] = (*rhs.rows)[i];
  }
  std::cout << "3rd Init." << std::endl;
}
IntMatrix::~IntMatrix() {
  delete[](*rows);
  delete rows;
  std::cout << "Something Distroyed." << std::endl;
}
IntMatrix & IntMatrix::operator=(const IntMatrix & rhs) {
  if (this != &rhs) {
    IntArray ** temp = new IntArray *;
    *temp = new IntArray[rhs.numRows];

    for (int i = 0; i < rhs.numRows; ++i) {
      (*temp)[i] = (*rhs.rows)[i];
    }
    delete[](*rows);
    delete rows;
    rows = temp;
    numRows = rhs.numRows;
    numColumns = rhs.numColumns;
  }
  return *this;
}
int IntMatrix::getRows() const {
  return numRows;
}
int IntMatrix::getColumns() const {
  return numColumns;
}
const IntArray & IntMatrix::operator[](int index) const {
  assert(index < numRows);
  return (*rows)[index];
}
IntArray & IntMatrix::operator[](int index) {
  assert(index < numRows);
  return (*rows)[index];
}

bool IntMatrix::operator==(const IntMatrix & rhs) const {
  bool flag = true;
  if (numRows != rhs.numRows) {
    flag = false;
  }
  for (int i = 0; i < numRows; ++i) {
    if ((*rows)[i] != (*rhs.rows)[i]) {
      flag = false;
    }
  }
  return flag;
}

IntMatrix IntMatrix::operator+(const IntMatrix & rhs) const {
  assert((numRows == rhs.numRows) && (numColumns == rhs.numColumns));
  IntMatrix temp = *this;
  for (int i = 0; i < numRows; ++i) {
    for (int j = 0; j < numColumns; ++j) {
      temp[i][j] += rhs[i][j];
    }
  }
  return temp;
}

std::ostream & operator<<(std::ostream & s, const IntMatrix & rhs) {
  s.write("[ ", 2);
  for (int i = 0; i < rhs.getRows(); ++i) {
    s << rhs[i];
    if (i != rhs.getRows() - 1) {
      s.write(",\n", 2);
    }
  }
  s.write(" ]", 2);
  return s;
}
