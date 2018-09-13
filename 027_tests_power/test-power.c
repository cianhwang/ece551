#include <stdio.h>
#include <stdlib.h>

unsigned power(unsigned x, unsigned y);
void run_check(unsigned x, unsigned y, unsigned expected_ans) {
  printf("x = %d, y = %d, ans = %d\n", x, y, power(x, y));
  if (expected_ans != power(x, y)) {
    exit(EXIT_FAILURE);
  }
}
int main(void) {
  unsigned arrX[10] = {2, 2, -1, 2.5, 0};
  unsigned arrY[10] = {3, 0, 1, 2, 0};
  unsigned arrAns[10] = {8, 1, 4294967295, 4, 1};

  for (int i = 0; i < 5; ++i) {
    run_check(arrX[i], arrY[i], arrAns[i]);
  }

  return EXIT_SUCCESS;
}
