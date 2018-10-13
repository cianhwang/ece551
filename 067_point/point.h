#ifndef POINT_H
#define POINT_H

class Point
{
 public:
  double x;
  double y;
  Point() : x(0), y(0) {}
  void move(double dx, double dy);
  double distanceFrom(const Point & p);
};

#endif
