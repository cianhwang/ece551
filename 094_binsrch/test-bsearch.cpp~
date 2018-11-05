#include <cmath>
#include <iostream>

#include "function.h"

int binarySearchForZero(Function<int, int> * f, int low, int high);

class CountedIntFn : public Function<int, int>
{
 protected:
  unsigned remaining;
  Function<int, int> * f;
  const char * mesg;

 public:
  CountedIntFn(unsigned n, Function<int, int> * fn, const char * m) :
      remaining(n),
      f(fn),
      mesg(m) {}
  virtual int invoke(int arg) {
    if (remaining == 0) {
      fprintf(stderr, "Too many function invocations in %s\n", mesg);
      exit(EXIT_FAILURE);
    }
    remaining--;
    return f->invoke(arg);
  }
  virtual ~CountedIntFn() {}
};

void check(Function<int, int> * f, int low, int high, int expected_ans) {
  int ans = binarySearchForZero(f, low, high);
  std::cout << "ans = " << ans << std::endl;
  if (ans != expected_ans) {
    exit(EXIT_FAILURE);
  }
  return;
}

class func1 : public Function<int, int>
{
 public:
  virtual int invoke(int arg) { return 10000000 * log(0.01 * (arg / 100.0 + 1)); }
  virtual ~func1() {}
};

int main(void) {
  Function<int, int> * f1 = new func1;
  Function<int, int> * c = new CountedIntFn(14, f1, "c1");
  check(c, 0, 10000, 9900);
  Function<int, int> * c2 = new CountedIntFn(7, f1, "c2");
  check(c2, 0, 100, 99);
  Function<int, int> * c3 = new CountedIntFn(20, f1, "c3");
  check(c3, 10000, 1000000, 10000);
  Function<int, int> * c4 = new CountedIntFn(2, f1, "c4");
  check(c4, 9898, 9906, 9900);
  delete f1;
  delete c;
  delete c2;
  delete c3;
  delete c4;
  return EXIT_SUCCESS;
}
