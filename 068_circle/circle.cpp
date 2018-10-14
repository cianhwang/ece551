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
    return a * a * acos(x) - 0.5 * a * a * sin(2 * acos(x)) + b * b * acos(y) -
           0.5 * b * b * sin(2 * acos(y));
  }
}
