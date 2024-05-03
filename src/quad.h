#ifndef _QUAD_H_
#define _QUAD_H_

#include "hittable_list.h"
#include "rtweekend.h"
#include "hittable.h"

class quad : public hittable {
 public:
  quad(
    const point3& q,
    const vec3& u,
    const vec3 v,
    std::shared_ptr<material> mat)
  : m_q(q), m_u(u), m_v(v), m_mat(mat) {
    const vec3 n = cross(u, v);
    m_normal = unit_vector(n);
    m_d = dot(m_normal, m_q);
    m_w = n / dot(n, n);

    set_bounding_box();
  }

  virtual void set_bounding_box() {
    const aabb bbox_diagonal1 = aabb(m_q, m_q + m_u + m_v);
    const aabb bbox_diagonal2 = aabb(m_q + m_u, m_q + m_v);

    m_bbox = aabb(bbox_diagonal1, bbox_diagonal2);
  }

  aabb bounding_box() const override { return m_bbox; }

  bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
    const double denom = dot(m_normal, r.direction());

    if (fabs(denom) < 1e-8) {
      return false;
    }

    const double t = (m_d - dot(m_normal, r.origin())) / denom;
    if (!ray_t.contains(t)) {
      return false;
    }

    const point3 intersection = r.at(t);
    const vec3 planar_hitpt_vector = intersection - m_q;
    const double alpha = dot(m_w, cross(planar_hitpt_vector, m_v));
    const double beta = dot(m_w, cross(m_u, planar_hitpt_vector));

    if(!is_interior(alpha, beta, rec)) {
      return false;
    }

    rec.t = t;
    rec.p = intersection;
    rec.mat = m_mat;
    rec.set_face_normal(r, m_normal);

    return true;
  }

  virtual bool is_interior(double a, double b, hit_record& rec) const {
    const interval unit_interval = interval(0.0, 1.0);

    if (!unit_interval.contains(a) || !unit_interval.contains(b)) {
      return false;
    }
    
    rec.u = a;
    rec.v = b;
    return true;
  }

 private:
  point3 m_q;
  vec3 m_u;
  vec3 m_v;
  vec3 m_w;
  std::shared_ptr<material> m_mat;
  aabb m_bbox;
  vec3 m_normal;
  double m_d;
};

inline std::shared_ptr<hittable_list> box(
  const point3&a, const point3& b, std::shared_ptr<material> mat) {
  std::shared_ptr<hittable_list> sides = std::make_shared<hittable_list>();

  const point3 min = point3(
    fmin(a.x(), b.x()),
    fmin(a.y(), b.y()),
    fmin(a.z(), b.z()));
  const point3 max = point3(
    fmax(a.x(), b.x()),
    fmax(a.y(), b.y()),
    fmax(a.z(), b.z()));

  const vec3 dx = vec3(max.x() - min.x(), 0.0, 0.0);
  const vec3 dy = vec3(0.0, max.y() - min.y(), 0.0);
  const vec3 dz = vec3(0.0, 0.0, max.z() - min.z());

  sides->add(
    std::make_shared<quad>(
      point3(min.x(), min.y(), max.z()), dx, dy, mat));
  sides->add(
    std::make_shared<quad>(
      point3(max.x(), min.y(), max.z()), -dz, dy, mat));
  sides->add(
    std::make_shared<quad>(
      point3(max.x(), min.y(), min.z()), -dx, dy, mat));
  sides->add(
    std::make_shared<quad>(
      point3(min.x(), min.y(), min.z()), dz, dy, mat));
  sides->add(
    std::make_shared<quad>(
      point3(min.x(), max.y(), max.z()), dx, -dz, mat));
  sides->add(
    std::make_shared<quad>(
      point3(min.x(), min.y(), min.z()), dx, dz, mat));

  return sides;
}

#endif  // _QUAD_H_
