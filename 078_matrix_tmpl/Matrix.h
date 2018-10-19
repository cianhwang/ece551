#ifndef __T_MATRIX_H___
#define __T_MATRIX_H___

#include <assert.h>

#include <cstdlib>
#include <iostream>
#include <vector>
using std::vector;

//YOUR CODE GOES HERE!
template<typename T>
class Matrix
{
 private:
  int nRow;
  int nCol;
  vector<T> ** rows;
  template<typename U>
  friend std::ostream & operator<<(std::ostream & s, const Matrix<U> & rhs);

 public:
  Matrix() : nRow(0), nCol(0), rows(new vector<T> *) { *rows = NULL; }
  Matrix(int r, int c);
  Matrix(const Matrix & rhs);
  ~Matrix();
  Matrix & operator=(const Matrix & rhs);
  int getRows() const { return nRow; }
  int getColumns() const { return nCol; }
  const vector<T> & operator[](int index) const;
  vector<T> & operator[](int index);
  bool operator==(const Matrix & rhs) const;
  Matrix operator+(const Matrix & rhs) const;
};

template<typename T>
Matrix<T>::Matrix(int r, int c) : nRow(r), nCol(c), rows(new vector<T> *) {
  *rows = new vector<T>[r];
  for (int i = 0; i < r; ++i) {
    (*rows)[i] = vector<T>(c);
  }
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T> & rhs) : nRow(rhs.nRow), nCol(rhs.nCol), rows(new vector<T> *) {
  *rows = new vector<T>[nRow];
  for (int i = 0; i < nRow; ++i) {
    (*rows)[i] = (*rhs.rows)[i];
  }
}

template<typename T>
Matrix<T>::~Matrix() {
  delete[](*rows);
  delete rows;
}

template<typename T>
Matrix<T> & Matrix<T>::operator=(const Matrix<T> & rhs) {
  if (this != &rhs) {
    vector<T> ** temp = new vector<T> *;
    *temp = new vector<T>[rhs.nRow];

    for (int i = 0; i < rhs.nRow; ++i) {
      (*temp)[i] = (*rhs.rows)[i];
    }
    delete[](*rows);
    delete rows;
    rows = temp;
    nRow = rhs.nRow;
    nCol = rhs.nCol;
  }
  return *this;
}

template<typename T>
vector<T> & Matrix<T>::operator[](int index) {
  assert(index < nRow);
  return (*rows)[index];
}

template<typename T>
const vector<T> & Matrix<T>::operator[](int index) const {
  assert(index < nRow);
  return (*rows)[index];
}

template<typename T>
bool Matrix<T>::operator==(const Matrix<T> & rhs) const {
  bool flag = true;
  if (nRow != rhs.nRow) {
    flag = false;
  }
  for (int i = 0; i < nRow; ++i) {
    if ((*rows)[i] != (*rhs.rows)[i]) {
      flag = false;
      break;
    }
  }
  return flag;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> & rhs) const {
  assert((nRow == rhs.nRow) && (nCol == rhs.nCol));
  Matrix<T> temp = *this;
  for (int i = 0; i < nRow; ++i) {
    for (int j = 0; j < nCol; ++j) {
      (*temp.rows)[i][j] += (*rhs.rows)[i][j];
    }
  }
  return temp;
}

template<typename T>
std::ostream & operator<<(std::ostream & s, const Matrix<T> & rhs) {
  s << "[ ";
  for (int i = 0; i < rhs.getRows(); ++i) {
    s << "{";
    const vector<T> v = (*rhs.rows)[i];
    for (int j = 0; j < rhs.getColumns(); ++j) {
      s << v[j];
      if (j != rhs.getColumns() - 1) {
        s << ", ";
      }
    }
    s << "}";
    if (i != rhs.getRows() - 1) {
      s << ",\n";
    }
  }
  s << " ]";
  return s;
}
#endif
