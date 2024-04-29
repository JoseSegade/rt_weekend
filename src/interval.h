#ifndef _INTERVAL_H_
#define _INTERVAL_H_

class interval {
 public:
  double min;
  double max;

  interval(): min(+infinity), max(-infinity) {}

  interval(double min, double max) : min(min), max(max) {}

  double size() const {
    return max - min;
  }

  bool contains(double x) const {
    return min <= x && x <= max;
  }

  bool surrounds(double x) const {
    return min < x && x < max;
  }

  double clamp(double x) const {
    if (x < min) return min;
    if (x > max) return max;
    return x;
  }

  static const interval empty;
  static const interval universe;
};

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);

#endif  // _INTERVAL_H_
