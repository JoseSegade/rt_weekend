#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "rtweekend.h"
class hit_record;
class material {
 public:
  virtual ~material() = default;
  virtual bool scatter(
    const ray& r_in,
    const hit_record& rec,
    color& attenuation,
    ray& scattered
  ) const {
    return false;
  }
};

class lambertian : public material {
 public:
  lambertian(const color& albedo) : m_albedo(albedo) {}

  bool scatter(
    const ray& r_in,
    const hit_record& rec,
    color& attenuation,
    ray& scattered
  ) const override {
    vec3 scatter_direction = rec.normal + random_unit_vector();

    if (scatter_direction.near_zero()) {
      scatter_direction = rec.normal;
    }

    scattered = ray(rec.p, scatter_direction);
    attenuation = m_albedo;
    return true;
  }

 private:
  color m_albedo;
};

class metal : public material {
 public:
  metal(const color& albedo, double fuzz) 
  : m_albedo(albedo), m_fuzz(fuzz < 1.0 ? fuzz : 1.0) {}

  bool scatter(
    const ray& r_in,
    const hit_record& rec,
    color& attenuation,
    ray& scattered
  ) const override {
    vec3 reflected = reflect(r_in.direction(), rec.normal);
    reflected = unit_vector(reflected) + (m_fuzz * random_unit_vector());
    scattered = ray(rec.p, reflected);
    attenuation = m_albedo;
    return (dot(scattered.direction(), rec.normal) > 0.0);
  }

 private:
  color m_albedo;
  double m_fuzz;
};

class dielectric : public material {
 public:
  dielectric(double refraction_index) : m_refraction_index(refraction_index) {}

  bool scatter(
    const ray& r_in,
    const hit_record& rec,
    color& attenuation,
    ray& scattered
  ) const override {
    attenuation = color(1.0, 1.0, 1.0);
    const double ri = rec.front_face 
      ? (1.0 / m_refraction_index) : m_refraction_index;

    const vec3 unit_direction = unit_vector(r_in.direction());
    const double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
    const double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    const bool cannot_refract = ri * sin_theta > 1.0;
    vec3 direction;

    if (cannot_refract || reflectance(cos_theta, ri) > random_double()) {
      direction = reflect(unit_direction, rec.normal);
    } else {
      direction = refract(unit_direction, rec.normal, ri);
    }

    scattered = ray(rec.p, direction);
    return true;
  }
  
 private:
  double m_refraction_index;

  constexpr double reflectance(double cosine, double refraction_index) const {
    double r0 = (1.0 - refraction_index) / (1.0 + refraction_index);
    r0 = r0 * r0;
    return r0 + (1.0 - r0) * std::pow((1.0 - cosine), 5.0);
  }
};

#endif  // _MATERIAL_H_
