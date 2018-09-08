#include <stdio.h>
#include <stdlib.h>

struct retire_info_tag {
  int months;
  double contribution;
  double rate_of_return;
};
typedef struct retire_info_tag retire_info_t;

double balance_Monthly(double balance, double contribution, double rate_of_return) {
  return balance * (1 + rate_of_return) + contribution;
}

void retirement(int startAge, double initial, retire_info_t working, retire_info_t retired) {
  double balance = initial;
  for (int i = 0; i < working.months; ++i) {
    printf(
        "Age %3d month %2d you have $%.2lf\n", (i + startAge) / 12, (i + startAge) % 12, balance);
    balance = balance_Monthly(balance, working.contribution, working.rate_of_return);
  }
  for (int i = 0; i < retired.months; ++i) {
    printf("Age %3d month %2d you have $%.2lf\n",
           (i + startAge + working.months) / 12,
           (i + startAge + working.months) % 12,
           balance);
    balance = balance_Monthly(balance, retired.contribution, retired.rate_of_return);
  }
}

int main(void) {
  retire_info_t working = {.months = 489, .contribution = 1000, .rate_of_return = 0.045 / 12};
  retire_info_t retired = {.months = 384, .contribution = -4000, .rate_of_return = 0.010 / 12};
  int Age = 327;
  double Savings = 21345;
  retirement(Age, Savings, working, retired);

  return EXIT_SUCCESS;
}
