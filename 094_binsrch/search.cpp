#include "function.h"

int binarySearchForZero(Function<int, int> * f, int low, int high) {
  int temp = low + (high - low) / 2;
  if (temp <= low) {
    return low;
  }
  int ans = f->invoke(temp);
  if (ans < 0) {
    return binarySearchForZero(f, temp, high);
  }
  else if (ans > 0) {
    return binarySearchForZero(f, low, temp);
  }
  else {
    return temp;
  }
}
