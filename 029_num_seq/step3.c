// This file is for Step 3.
// You should do
//  Step 3 (A): write seq3
//  Step 3 (B): write main to test seq3
//  Step 3 (C): write countEvenInSeq3Range
//  Step 3 (D): add test cases to main to test countEvenInSeq3Range
//
// Be sure to #include any header files you need!

#include <stdio.h>
#include <stdlib.h>

int seq3(int x, int y) {
  return (15 - 3 * y) + (y - 3) * (x + 5);
}

int countEvenInSeq3Range(int xLow, int xHi, int ylow, int yHi) {
  int counter = 0;
  for (int i = xLow; i < xHi; ++i) {
    for (int j = ylow; j < yHi; ++j) {
      counter += (seq3(i, j) % 2 == 0);
    }
  }
  return counter;
}

int main(void) {
  for (int i = -5; i < 6; ++i) {
    for (int j = -5; j < 6; ++j) {
      printf("seq3(%d, %d) = %d\n", i, j, seq3(i, j));
    }
  }
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      printf("countEvenInSeq3Range(%d, %d, %d, %d) = %d\n",
             i,
             2,
             j,
             3,
             countEvenInSeq3Range(i, 2, j, 3));
    }
  }

  return EXIT_SUCCESS;
}
