// This file is for Step 2.
// You should do
//  Step 2 (A): write seq2
//  Step 2 (B): write main to test seq2
//  Step 2 (C): write sumSeq2
//  Step 2 (D): add test cases to main to test sumSeq2
//
// Be sure to #include any header files you need!
#include <stdio.h>
#include <stdlib.h>

int seq2(int x) {
  return (x + 3) * x + 1;
}

int sumSeq2(int low, int high) {
  int tempSum = 0;
  for (int i = low; i < high; ++i) {
    tempSum += seq2(i);
  }
  return tempSum;
}

int main(void) {
  for (int i = -10; i < 15; ++i) {
    printf("seq2(%d) = %d\n", i, seq2(i));
  }

  for (int i = -5; i < 10; ++i) {
    for (int j = -5; j < 10; ++j) {
      printf("sumSeq2(%d, %d) = %d\n", i, j, sumSeq2(i, j));
    }
  }

  return EXIT_SUCCESS;
}
