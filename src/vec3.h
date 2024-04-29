#ifndef _VEC3_H_
#define _VEC3_H_

#include <cmath>
#include <iostream>

class vec3 {
 public:
  double e[3];

  vec3() : e { 0.0, 0.0, 0.0 } {}
  vec3(double e0, double e1, double e2) : e { e0, e1, e2 } {}

  double x() const { return e[0]; }
  double y() const { return e[1]; }
  double z() const { return e[2]; }

  vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
  double operator[](int i) const { return e[i]; }
  double& operator[](int i) { return e[i]; }

  vec3& operator+=(const vec3& v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
  }

  vec3& operator*=(double t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
  }

  vec3& operator/=(double t) {
    return *this *= 1.0/t;
  }

  double length_squared() const {
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
  }

  double length() const {
    return std::sqrt(length_squared());
  }

  bool near_zero() const {
    const double s = 1e-8;
    return 
      (std::fabs(e[0]) < s)
      && (std::fabs(e[1]) < s)
      && (std::fabs(e[2]) < s);
  }

  static vec3 random() {
    return vec3(
      random_double(),
      random_double(),
      random_double());
  }

  static vec3 random(double min, double max) {
    return vec3(
      random_double(min, max),
      random_double(min, max),
      random_double(min, max));
  }
};

using point3 = vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
  return out << v.e[0] << " " << v.e[1] << " " << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
  return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
  return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
  return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3& u) {
  return vec3(t * u.e[0], t * u.e[1], t * u.e[2]);
}

inline vec3 operator*(const vec3& u, double t) {
  return t * u;
}

inline vec3 operator/(const vec3& v, double t) {
  return (1 / t) * v;
}

inline double dot(const vec3& u, const vec3& v) {
  return 
    u.e[0] * v.e[0] +
    u.e[1] * v.e[1] +
    u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
  return vec3(
    u.e[1] * v.e[2] - u.e[2] * v.e[1],
    u.e[2] * v.e[0] - u.e[0] * v.e[2],
    u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(const vec3& v) {
  return v / v.length();
}

inline vec3 random_in_unit_disk() {
  while (true) {
    const vec3 p =
      vec3(random_double(-1.0, 1.0), random_double(-1.0, 1.0), 0.0);
    if (p.length_squared() < 1.0) {
      return p;
    }
  }
}

inline vec3 random_in_unit_sphere() {
  while (true) {
    const vec3 p = vec3::random(-1.0, 1.0);
    if (p.length_squared() < 1) {
      return p;
    }
  }
}

inline vec3 random_unit_vector() {
  return unit_vector(random_in_unit_sphere());
}

inline vec3 random_on_hemisphere(const vec3& normal) {
  const vec3 on_unit_sphere = random_unit_vector();
  if (dot(on_unit_sphere, normal) > 0.0) {
    return on_unit_sphere;
  } else {
    return -on_unit_sphere;
  }
}

inline vec3 reflect(const vec3& v, const vec3& n) {
  return v - 2.0 * dot(v, n) * n;
}

inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
  const double cos_theta = fmin(dot(-uv, n), 1.0);
  const vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
  const vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
  return r_out_perp + r_out_parallel;
}

#endif  // _VEC3_H_
