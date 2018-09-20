#include <stdlib.h>

size_t maxSeq(int * array, size_t n) {
  size_t archCount = 0, currCount = 0;
  for (size_t i = 0; i < n; ++i) {
    if (i == 0 || array[i] > array[i - 1]) {
      ++currCount;
    }
    else {
      currCount = 1;
    }

    if (currCount > archCount) {
      archCount = currCount;
    }
  }
  return archCount;
}
