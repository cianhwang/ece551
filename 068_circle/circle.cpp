#include "circle.h"

#include <cmath>
#include <iostream>

Circle::Circle() : center(), radius(0) {}
Circle::Circle(Point & p, double rad) : center(p), radius(rad) {}
void Circle::move(double dx, double dy) {
  center.move(dx, dy);
}
double Circle::intersectionArea(const Circle & otherCircle) {
  double dis = center.distanceFrom(otherCircle.center);
  double a = radius;
  double b = otherCircle.radius;
  if (dis >= (a + b)) {
    return 0.0;
  }
  else if (dis <= std::abs(radius - otherCircle.radius)) {
    double minRadius = radius < otherCircle.radius ? radius : otherCircle.radius;
    return M_PI * minRadius * minRadius;
  }
  else {
    double x = (a * a - b * b + dis * dis) / (2 * dis * a);
    double y = (-a * a + b * b + dis * dis) / (2 * dis * b);
    int signA = 1;
    int signB = 1;
    if (x < 0) {
      signA = -1;
      std::cout << "x < 0" << std::endl;
    }
    if (y < 0) {
      signB = -1;
      std::cout << "y < 0" << std::endl;
    }
    return 2 * a * a * acos(x) - signA * a * a * sin(acos(x)) * x + 2 * b * b * acos(y) -
           signB * b * b * sin(acos(y)) * y;
  }
}
