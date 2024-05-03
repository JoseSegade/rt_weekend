#ifndef _HITTABLE_H_
#define _HITTABLE_H_

#include "rtweekend.h"
#include "aabb.h"

class material;

class hit_record {
 public:
  point3 p;
  vec3 normal;
  std::shared_ptr<material> mat;
  double t;
  double u;
  double v;
  bool front_face;

  void set_face_normal(const ray& r, const vec3& outward_normal) {
    front_face = dot(r.direction(), outward_normal) < 0.0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class hittable {
 public:
  virtual ~hittable() = default;

  virtual bool hit(
    const ray& r,
    interval ray_t,
    hit_record& rec
  ) const = 0;

  virtual aabb bounding_box() const = 0;
};

class translate : public hittable {
 public:
  translate(std::shared_ptr<hittable> object, const vec3& offset)
  : m_object(object), m_offset(offset) {
    m_bbox = object->bounding_box() + m_offset;
  }

  bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
    ray offset_r(r.origin() - m_offset, r.direction(), r.time());

    if (!m_object->hit(offset_r, ray_t, rec)) {
      return false;
    }

    rec.p += m_offset;
    return true;
  };

  aabb bounding_box() const override { return m_bbox; }

 private:
  std::shared_ptr<hittable> m_object;
  vec3 m_offset;
  aabb m_bbox;
};

class rotate_y : public hittable {
 public:
  rotate_y(std::shared_ptr<hittable> object, double angle_degrees)
  : m_object(object) {
    const double radians = degrees_to_radians(angle_degrees);
    m_sin_theta = sin(radians);
    m_cos_theta = cos(radians);
    m_bbox = object->bounding_box();

    point3 min( infinity,  infinity,  infinity);
    point3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; ++i) {
      for (int j = 0; j < 2; ++j) {
        for (int k = 0; k < 2; ++k) {
          const double x = i * m_bbox.x.max + (1 - i) * m_bbox.x.min;
          const double y = j * m_bbox.y.max + (1 - j) * m_bbox.y.min;
          const double z = k * m_bbox.z.max + (1 - k) * m_bbox.z.min;

          const double newx =  m_cos_theta * x + m_sin_theta * z;
          const double newz = -m_sin_theta * x + m_cos_theta * z;

          const vec3 tester(newx, y, newz);

          for (int c = 0; c < 3; ++c) {
            min[c] = fmin(min[c], tester[c]);
            max[c] = fmax(max[c], tester[c]);
          }
        }
      }
    }

    m_bbox = aabb(min, max);
  }

  bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
    point3 origin = r.origin();
    vec3 direction = r.direction();

    origin[0] = m_cos_theta * r.origin()[0] - m_sin_theta * r.origin()[2];
    origin[2] = m_sin_theta * r.origin()[0] + m_cos_theta * r.origin()[2];

    direction[0] =
      m_cos_theta * r.direction()[0] - m_sin_theta * r.direction()[2];
    direction[2] =
      m_sin_theta * r.direction()[0] + m_cos_theta * r.direction()[2];

    const ray rotated_r(origin, direction, r.time());

    if (!m_object->hit(rotated_r, ray_t, rec)) {
      return false;
    }

    point3 p = rec.p;
    p[0] =  m_cos_theta * rec.p[0] + m_sin_theta * rec.p[2];
    p[2] = -m_sin_theta * rec.p[0] + m_cos_theta * rec.p[2];

    vec3 normal = rec.normal;
    normal[0] =  m_cos_theta * rec.normal[0] + m_sin_theta * rec.normal[2];
    normal[2] = -m_sin_theta * rec.normal[0] + m_cos_theta * rec.normal[2];

    rec.p = p;
    rec.normal = normal;

    return true;
  }

  aabb bounding_box() const override { return m_bbox; }

 private:
  std::shared_ptr<hittable> m_object;
  double m_sin_theta;
  double m_cos_theta;
  aabb m_bbox;
};

#endif  // _HITTABLE_H_
