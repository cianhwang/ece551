#ifndef CIRCLE_H
#define CIRCLE_H
#include "point.h"

class Circle
{
  Point center;
  const double radius;

 public:
  Circle();
  Circle(Point & p, double rad);
  void move(double dx, double dy);
  double intersectionArea(const Circle & otherCircle);
};

#endif
