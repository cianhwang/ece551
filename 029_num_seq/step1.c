// This file is for Step 1.
// You should do
//  Step 1 (A): write seq1
//  Step 1 (B): write main to test seq1
//  Step 1 (C): write printSeq1Range
//  Step 1 (D): add test cases to main to test printSeq1Range
//
// Be sure to #include any header files you need!

#include <stdio.h>
#include <stdlib.h>

int seq1(int x) {
  return 4 * x - 3;
}

void printSeq1Range(int low, int high) {
  if (low < high) {
    for (int i = low; i < high - 1; ++i) {
      printf("%d, ", seq1(i));
    }
    printf("%d", seq1(high - 1));
  }
  printf("\n");
}

int main(void) {
  int temp;
  for (int i = -5; i < 10; ++i) {
    temp = seq1(i);
    printf("seq1(%d) = %d\n", i, temp);
  }
  for (int i = -3; i < 8; ++i) {
    for (int j = -3; j < 8; ++j) {
      printf("printSeq1Range(%d, %d)\n", i, j);
      printSeq1Range(i, j);
    }
  }
  return EXIT_SUCCESS;
}
