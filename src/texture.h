#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "perlin.h"
#include "rtw_image.h"
#include "rtweekend.h"

class texture {
 public:
  virtual ~texture() = default;

  virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture {
 public:
  solid_color(const color& albedo) : m_albedo(albedo) {}

  solid_color(double red, double green, double blue)
  : m_albedo(red, green, blue) {}

  color value(double u, double v, const point3& p) const override {
    return m_albedo;
  }

 private:
  color m_albedo;
};

class checker_texture : public texture {
 public:
  checker_texture(
    double scale,
    std::shared_ptr<texture> even,
    std::shared_ptr<texture> odd)
  : m_inv_scale(1.0 / scale), m_even(even), m_odd(odd) {}

  checker_texture(double scale, const color& c1, const color& c2) 
  : m_inv_scale(1.0 / scale)
  , m_even(std::make_shared<solid_color>(c1))
  , m_odd(std::make_shared<solid_color>(c2)) {}

  color value(double u, double v, const point3& p) const override {
    const int xInt = static_cast<int>(std::floor(m_inv_scale * p.x()));
    const int yInt = static_cast<int>(std::floor(m_inv_scale * p.y()));
    const int zInt = static_cast<int>(std::floor(m_inv_scale * p.z()));

    const bool isEven = (xInt + yInt + zInt) % 2 == 0;

    return isEven ? m_even->value(u, v, p) : m_odd->value(u, v, p);
  }

 private:
  double m_inv_scale;
  std::shared_ptr<texture> m_even;
  std::shared_ptr<texture> m_odd;
};

class image_texture : public texture {
 public:
  image_texture(const char* filename) : m_image(filename) {}

  color value(double u, double v, const point3& p) const override {
    if (m_image.height() <= 0) {
      return color(0.0, 1.0, 1.0);
    }

    u = interval(0.0, 1.0).clamp(u);
    v = 1.0 - interval(0.0, 1.0).clamp(v);

    const int i = static_cast<int>(u * m_image.width());
    const int j = static_cast<int>(v * m_image.height());
    const unsigned char* pixel = m_image.pixel_data(i, j);

    const double color_scale = 1.0 / 255.0;
    return color(
      color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
  }

 private:
  rtw_image m_image;
};

class noise_texture : public texture {
 public:
  noise_texture() {}

  noise_texture(double scale) : m_scale(scale) {}

  color value(double u, double v, const point3& p) const override {
    return color(0.5, 0.5, 0.5) 
      * (1.0 + sin(m_scale * p.z() + 10 * m_noise.turb(p, 7.0)));
  }

 private:
  perlin m_noise;
  double m_scale;
};

#endif  // _TEXTURE_H_
