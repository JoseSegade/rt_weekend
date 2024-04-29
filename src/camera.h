#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "rtweekend.h"
#include "hittable.h"
#include "material.h"

class camera {
 public:
  double aspect_ratio = 1.0;
  int image_width = 100;
  int samples_per_pixel = 10;
  int max_depth = 10;

  double vfov = 90.0;
  point3 lookfrom = point3(0.0, 0.0, 0.0);
  point3 lookat = point3(0.0, 0.0, -1.0);
  vec3 vup = vec3(0.0, 1.0, 0.0);

  double defocus_angle = 0.0;
  double focus_dist = 10;

  void render(const hittable& world) {
    initialize();

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    for (int y = 0; y < image_height; ++y) {
      std::clog << "\rScanlines remaining: " << (image_height - y) 
        << " " << std::flush;
      for (int x = 0; x < image_width; ++x) {
        color pixel_color(0.0, 0.0, 0.0);
        for (int sample = 0; sample < samples_per_pixel; ++sample) {
          ray r = get_ray(x, y);
          pixel_color += ray_color(r, max_depth, world);
        }
        write_color(std::cout, pixel_color * pixel_samples_scale);
      }
    }

    std::clog << "\rDone.         \n";
  }

 private:
  int image_height;
  double pixel_samples_scale;
  point3 center;
  point3 pixel00_loc;
  vec3 pixel_delta_u;
  vec3 pixel_delta_v;
  vec3 u;
  vec3 v;
  vec3 w;
  vec3 defocus_disk_u;
  vec3 defocus_disk_v;

  void initialize() {
    image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    pixel_samples_scale = 1.0 / samples_per_pixel;

    center = lookfrom;

    const double theta = degrees_to_radians(vfov);
    const double h = tan(theta / 2.0);
    const double viewport_height = 2.0 * h * focus_dist;
    const double viewport_width = viewport_height *
      (static_cast<double>(image_width) / image_height);

    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    const vec3 viewport_u = viewport_width * u;
    const vec3 viewport_v = viewport_height * -v;

    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    const point3 viewport_upper_left = center
      - (focus_dist * w) - viewport_u / 2.0 - viewport_v / 2.0;
    pixel00_loc = viewport_upper_left
      + 0.5 * (pixel_delta_u + pixel_delta_v);

    const double defocus_radius = focus_dist 
      * std::tan(degrees_to_radians(defocus_angle / 2.0));
    defocus_disk_u = u * defocus_radius;
    defocus_disk_v = v * defocus_radius;
  }

  ray get_ray(int i, int j) const {
    const vec3 offset = sample_square();
    const point3 pixel_sample = pixel00_loc
      + ((i + offset.x()) * pixel_delta_u)
      + ((j + offset.y()) * pixel_delta_v);

    const point3 ray_origin = (defocus_angle <= 0.0) ?
      center : defocus_disk_sample();
    const vec3 ray_direction = pixel_sample - ray_origin;

    return ray(ray_origin, ray_direction);
  }

  vec3 sample_square() const {
    return vec3(random_double() - 0.5, random_double() - 0.5, 0.0);
  }

  point3 defocus_disk_sample() const {
    const vec3 p = random_in_unit_disk();
    return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
  }

  color ray_color(const ray& r, int depth, const hittable& world) {
    if (depth <= 0) {
      return color(0.0, 0.0, 0.0);
    }
    hit_record rec;
    if (world.hit(r, interval(0.001, infinity), rec)) {
      ray scattered;
      color attenuation;
      if (rec.mat->scatter(r, rec, attenuation, scattered)) {
        return attenuation * ray_color(scattered, depth - 1, world);
      }
      return color(0.0, 0.0, 0.0);
    }
    
    const vec3 unit_direction = unit_vector(r.direction());
    const double a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
  }

};

#endif  // _CAMERA_H_
