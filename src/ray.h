#ifndef _RAY_H_
#define _RAY_H_

#include "vec3.h"

class ray {
 public:
  ray() {}

  ray(const point3& origin, const vec3& dir) : m_origin(origin), m_dir(dir) {}

  const point3& origin() const { return m_origin; }
  const vec3& direction() const { return m_dir; }

  point3 at(double t) const {
    return m_origin + t * m_dir;
  }

 private:
  point3 m_origin;
  vec3 m_dir;
};

#endif  // _RAY_H_

