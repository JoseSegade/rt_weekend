#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "rtweekend.h"
#include "hittable.h"

class sphere : public hittable {
 public:
  sphere(const point3& center, double radius, std::shared_ptr<material> mat)
  : m_center1(center), m_radius(std::fmax(0.0, radius)), m_mat(mat),
    m_is_moving(false) {
    const vec3 rvec = vec3(radius, radius, radius);
    bbox = aabb(m_center1 - rvec, m_center1 + rvec);
  }

  sphere(const point3& center1, const point3& center2, double radius, 
         std::shared_ptr<material> mat)
  : m_center1(center1), m_radius(std::fmax(0.0, radius)), m_mat(mat),
    m_is_moving(true) {
    const vec3 rvec = vec3(radius, radius, radius);
    const aabb box1(center1 - rvec, center1 + rvec);
    const aabb box2(center2 - rvec, center2 + rvec);
    bbox = aabb(box1, box2);

    m_center_vec = center2 - center1;
  }

  bool hit(
    const ray& r,
    interval ray_t,
    hit_record& rec
  ) const override {
    point3 center = m_is_moving ? sphere_center(r.time()) : m_center1;
    const vec3 oc = center - r.origin();
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
    const vec3 outward_normal = (rec.p - center) / m_radius;
    rec.set_face_normal(r, outward_normal);
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat = m_mat;

    return true;
  }

  aabb bounding_box() const override { return bbox; }

 private:
  point3 m_center1;
  double m_radius;
  std::shared_ptr<material> m_mat;
  bool m_is_moving;
  vec3 m_center_vec;
  aabb bbox;

  point3 sphere_center(double time) const {
    return m_center1 + time * m_center_vec;
  }

  static void get_sphere_uv(const point3& p, double& u, double& v) {
    const double theta = std::acos(-p.y());
    const double phi = std::atan2(-p.z(), p.x()) + pi;

    u = phi / (2.0 * pi);
    v = theta / pi;
  }
};

#endif  // _SPHERE_H_
