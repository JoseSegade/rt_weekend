#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "rtweekend.h"
#include "hittable.h"

class sphere : public hittable {
 public:
  sphere(const point3& center, double radius, std::shared_ptr<material> mat)
    : m_center(center), m_radius(std::fmax(0.0, radius)), m_mat(mat) {}

  bool hit(
    const ray& r,
    interval ray_t,
    hit_record& rec
  ) const override {
    const vec3 oc = m_center - r.origin();
    const double a = r.direction().length_squared();
    const double h = dot(r.direction(), oc);
    const double c = oc.length_squared() - m_radius * m_radius;

    const double discriminant = h * h - a * c;

    if (discriminant < 0.0) {
      return false;
    }

    const double sqrtd = std::sqrt(discriminant);

    double root = (h - sqrtd) / a;
    if (!ray_t.surrounds(root)) {
      root = (h + sqrtd) / a;
      if (!ray_t.surrounds(root)) {
        return false;
      }
    }
    
    rec.t = root;
    rec.p = r.at(rec.t);
    const vec3 outward_normal = (rec.p - m_center) / m_radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat = m_mat;

    return true;
  }

 private:
  point3 m_center;
  double m_radius;
  std::shared_ptr<material> m_mat;
};

#endif  // _SPHERE_H_
