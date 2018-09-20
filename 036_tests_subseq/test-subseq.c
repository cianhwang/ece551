#include <stdio.h>
#include <stdlib.h>
#define N 5
size_t maxSeq(int * array, size_t n);

void test_maxSeq(int * array, size_t n, size_t expected_ans) {
  size_t temp = maxSeq(array, n);
  printf("temp = %ld\n", temp);
  if (temp != expected_ans) {
    exit(EXIT_FAILURE);
  }
  return;
}

int main(void) {
  int test_case[N][10] = {{0xFFFFFFFF, 0, 3, 4, 5, 67, 8, 9, -5, 0},
                          {1, 1, 1, 3, 4, -5, -4, -3, -2, -3},
                          {1, 2, 1, 3, 5, 8, 2, 4, 6, 9},
                          {1, 2, 3, 4, 5, -5, -4, -3, -2, -1},
                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

  int expected_ans[N] = {6, 4, 4, 5, 1};
  int test_case2[1] = {1};
  test_maxSeq(test_case2, 1, 1);
  test_maxSeq(test_case2, 0, 0);

  for (int i = 0; i < N; ++i) {
    test_maxSeq(*(test_case + i), 10, expected_ans[i]);
  }
  return EXIT_SUCCESS;
}
