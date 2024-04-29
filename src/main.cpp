// CC0 (raytracing.github.io) 2024 - Copy by Meerkat

#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"


int main() {
  hittable_list world;

  const std::shared_ptr<lambertian> ground_material =
    std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
  world.add(std::make_shared<sphere>(
    point3(0.0, -1000.0, 0.0), 1000, ground_material));
  
  for (int a = -11; a < 11; ++a) {
    for (int b = -11; b < 11; ++b) {
      const double choose_mat = random_double();
      const point3 center(
        a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

      if ((center - point3(4.0, 0.2, 0.0)).length() > 0.9) {
        std::shared_ptr<material> sphere_material;

        if (choose_mat < 0.8) {
          const color albedo = color::random() * color::random();
          sphere_material = std::make_shared<lambertian>(albedo);
        } else if (choose_mat < 0.95) {
          const color albedo = color::random(0.5, 1.0);
          const double fuzz = random_double(0.0, 0.5);
          sphere_material = std::make_shared<metal>(albedo, fuzz);
        } else {
          sphere_material = std::make_shared<dielectric>(1.5);
        }

        world.add(std::make_shared<sphere>(
                  center, 0.2, sphere_material));
      }
    }
  }

  const std::shared_ptr<material> material1 = 
    std::make_shared<dielectric>(1.5);
  world.add(std::make_shared<sphere>(
    point3(0.0, 1.0, 0.0), 1.0, material1));
  const std::shared_ptr<material> material2 = 
    std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
  world.add(std::make_shared<sphere>(
    point3(-4.0, 1.0, 0.0), 1.0, material2));
  const std::shared_ptr<material> material3 = 
    std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(std::make_shared<sphere>(
    point3(4.0, 1.0, 0.0), 1.0, material3));

  camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 1200;
  cam.samples_per_pixel = 25;
  cam.max_depth = 10;

  cam.vfov = 20;
  cam.lookfrom = point3(13.0, 2.0, 3.0);
  cam.lookat = point3(0.0, 0.0, 0.0);
  cam.vup = vec3(0.0, 1.0, 0.0);

  cam.defocus_angle = 0.6;
  cam.focus_dist = 10.0;

  cam.render(world);
}
