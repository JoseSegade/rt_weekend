#ifndef _CONSTANT_MEDIUM_H_
#define _CONSTANT_MEDIUM_H_

#include "rtweekend.h"
#include "hittable.h"
#include "material.h"
#include "texture.h"

class constant_medium : public hittable {
 public:
  constant_medium(
    std::shared_ptr<hittable> boundary,
    double density,
    std::shared_ptr<texture> tex)
  : m_boundary(boundary)
  , m_neg_inv_density(-1.0 / density)
  , m_phase_function(std::make_shared<isotropic>(tex)) {}

  constant_medium(
    std::shared_ptr<hittable> boundary,
    double density,
    const color& albedo)
  : m_boundary(boundary)
  , m_neg_inv_density(-1.0 / density)
  , m_phase_function(std::make_shared<isotropic>(albedo)) {}

  bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
    const bool enableDebug = false;
    const bool debugging = enableDebug && random_double() < 0.0001;

    hit_record rec1, rec2;

    if (!m_boundary->hit(r, interval::universe, rec1)) {
      return false;
    }

    if (!m_boundary->hit(r, interval(rec1.t + 0.0001, infinity), rec2)) {
      return false;
    }

    if (debugging) {
      std::clog << "\nt_min=" << rec1.t << ", t_max=" << rec2.t << "\n";
    }

    if (rec1.t < ray_t.min) {
      rec1.t = ray_t.min;
    }
    if (rec2.t > ray_t.max) {
      rec2.t = ray_t.max;
    }

    if (rec1.t >= rec2.t) {
      return false;
    }

    if (rec1.t < 0.0) {
      rec1.t = 0.0;
    }

    const double ray_lenght = r.direction().length();
    const double distance_inside_boundary = (rec2.t - rec1.t) * ray_lenght;
    const double hit_distance = m_neg_inv_density * log(random_double());

    if (hit_distance > distance_inside_boundary) {
      return false;
    }

    rec.t = rec1.t + hit_distance / ray_lenght;
    rec.p = r.at(rec.t);

    if (debugging) {
      std::clog << "hit_distance = " << hit_distance << "\n"
        << "rec.t = " << rec.t << "\n"
        << "rec.p = " << rec.p << "\n";
    }

    rec.normal = vec3(1.0, 0.0, 0.0);
    rec.front_face = true;
    rec.mat = m_phase_function;

    return true;
  }

  aabb bounding_box() const override { return m_boundary->bounding_box(); }

 private:
  std::shared_ptr<hittable> m_boundary;
  double                    m_neg_inv_density;
  std::shared_ptr<material> m_phase_function;
};

#endif  // _CONSTANT_MEDIUM_H_
