#ifndef _POINT_H_
#define _POINT_H_

typedef struct Point {
  unsigned short object_id;
  float porjection;

  Point(unsigned short object_id_ = 0, float projection_ = 0) {
    object_id = object_id_;
    porjection = projection_;
  }

  bool operator<(const Point& another) const {
    return this->porjection < another.porjection;
  }

  bool operator>(const Point& another) const {
    return this->porjection > another.porjection;
  }

  void operator=(Point& another) {
    this->object_id = another.object_id;
    this->porjection = another.porjection;
  }

} Point;

#endif
