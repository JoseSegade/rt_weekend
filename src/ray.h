#ifndef _RAY_H_
#define _RAY_H_

#include "vec3.h"

class ray {
 public:
  ray() {}

  ray(const point3& origin, const vec3& dir) 
  : m_origin(origin), m_dir(dir), m_time(0.0) {}

  ray(const point3& origin, const vec3& dir, double time) 
  : m_origin(origin), m_dir(dir), m_time(time) {}

  const point3& origin() const { return m_origin; }
  const vec3& direction() const { return m_dir; }
  double time() const { return m_time; }

  point3 at(double t) const {
    return m_origin + t * m_dir;
  }

 private:
  point3 m_origin;
  vec3 m_dir;
  double m_time;
};

#endif  // _RAY_H_

