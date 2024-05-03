#ifndef _RTWEEKEND_H_
#define _RTWEEKEND_H_

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
  return degrees * (pi / 180.0);
}

inline double random_double() {
  return static_cast<double>(rand()) / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
  return min + (max - min) * random_double();
}

inline int random_int(int min, int max) {
  return static_cast<int>(random_double(min, max + 1.0));
}

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif  // _RTWEEKEND_H_
