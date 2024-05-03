// CC0 (raytracing.github.io) 2024 - Copy by Meerkat

#include "bvh.h"
#include "constant_medium.h"
#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "quad.h"
#include "texture.h"

void bouncing_spheres() {
  hittable_list world;

  const std::shared_ptr<texture> checker = 
    std::make_shared<checker_texture>(
      0.32, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
  const std::shared_ptr<lambertian> ground_material =
    std::make_shared<lambertian>(checker);
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
          const point3 center2 = 
            center + vec3(0.0, random_double(0.0, 0.5), 0.0);
          world.add(std::make_shared<sphere>(
                    center, center2, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          const color albedo = color::random(0.5, 1.0);
          const double fuzz = random_double(0.0, 0.5);
          sphere_material = std::make_shared<metal>(albedo, fuzz);
          world.add(std::make_shared<sphere>(
                    center, 0.2, sphere_material));
        } else {
          sphere_material = std::make_shared<dielectric>(1.5);
          world.add(std::make_shared<sphere>(
                    center, 0.2, sphere_material));
        }
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

  world = hittable_list(std::make_shared<bvh_node>(world));

  camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 25;
  cam.max_depth = 10;
  cam.background = color(0.70, 0.80, 1.00);

  cam.vfov = 20;
  cam.lookfrom = point3(13.0, 2.0, 3.0);
  cam.lookat = point3(0.0, 0.0, 0.0);
  cam.vup = vec3(0.0, 1.0, 0.0);

  cam.defocus_angle = 0.6;
  cam.focus_dist = 10.0;

  cam.render(world);
}

void checkered_spheres() {
  hittable_list world;

  const std::shared_ptr<texture> checker =
    std::make_shared<checker_texture>(
      0.32, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

  world.add(std::make_shared<sphere>(point3(0.0, -10.0, 0.0), 10.0,
            std::make_shared<lambertian>(checker)));
  world.add(std::make_shared<sphere>(point3(0.0, 10.0, 0.0), 10.0,
            std::make_shared<lambertian>(checker)));

  camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth = 50;
  cam.background = color(0.70, 0.80, 1.00);

  cam.vfov = 20;
  cam.lookfrom = point3(13.0, 2.0, 3.0);
  cam.lookat = point3(0.0, 0.0, 0.0);
  cam.vup = vec3(0.0, 1.0, 0.0);

  cam.defocus_angle = 0.0;

  cam.render(world);
}

void earth() {
  const std::shared_ptr<texture> earth_texture =
    std::make_shared<image_texture>("earthmap.jpg");
  const std::shared_ptr<material> earth_surface = 
    std::make_shared<lambertian>(earth_texture);
  const std::shared_ptr<hittable> globe = 
    std::make_shared<sphere>(point3(0.0, 0.0, 0.0), 2.0, earth_surface);

  camera cam;
  
  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 20;
  cam.max_depth = 10;
  cam.background = color(0.70, 0.80, 1.00);

  cam.vfov = 20;
  cam.lookfrom = point3(0.0, 0.0, 12.0);
  cam.lookat = point3(0.0, 0.0, 0.0);
  cam.vup = vec3(0.0, 1.0, 0.0);

  cam.defocus_angle = 0.0;

  cam.render(hittable_list(globe));
}

void perlin_spheres() {
  hittable_list world;

  const std::shared_ptr<texture> pertex =
    std::make_shared<noise_texture>(4.0);

  world.add(std::make_shared<sphere>(
    point3(0.0, -1000.0, 0), 1000.0, std::make_shared<lambertian>(pertex)));
  world.add(std::make_shared<sphere>(
    point3(0.0, 2.0, 0), 2.0, std::make_shared<lambertian>(pertex)));

  camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 25;
  cam.max_depth = 10;
  cam.background = color(0.70, 0.80, 1.00);

  cam.vfov = 20;
  cam.lookfrom = point3(13.0, 2.0, 3.0);
  cam.lookat = point3(0.0, 0.0, 0.0);
  cam.vup = vec3(0.0, 1.0, 0.0);

  cam.defocus_angle  = 0.0;

  cam.render(world);
}

void quads() {
  hittable_list world;

  std::shared_ptr<material> left_red =
    std::make_shared<lambertian>(color(1.0, 0.2, 0.2));
  std::shared_ptr<material> back_green =
    std::make_shared<lambertian>(color(0.2, 1.0, 0.2));
  std::shared_ptr<material> right_blue =
    std::make_shared<lambertian>(color(0.2, 0.2, 1.0));
  std::shared_ptr<material> upper_orange =
    std::make_shared<lambertian>(color(1.0, 0.5, 0.0));
  std::shared_ptr<material> lower_teal =
    std::make_shared<lambertian>(color(0.2, 0.8, 0.8));

  world.add(std::make_shared<quad>(
    point3(-3.0, -2.0, 5.0),
    vec3(0.0, 0.0, -4.0),
    vec3(0.0, 4.0, 0.0),
    left_red));
  world.add(std::make_shared<quad>(
    point3(-2.0, -2.0, 0.0),
    vec3(4.0, 0.0, 0.0),
    vec3(0.0, 4.0, 0.0),
    back_green));
  world.add(std::make_shared<quad>(
    point3(3.0, -2.0, 1.0),
    vec3(0.0, 0.0, 4.0),
    vec3(0.0, 4.0, 0.0),
    right_blue));
  world.add(std::make_shared<quad>(
    point3(-2.0, 3.0, 1.0),
    vec3(4.0, 0.0, 0.0),
    vec3(0.0, 0.0, 4.0),
    upper_orange));
  world.add(std::make_shared<quad>(
    point3(-2.0, -3.0, 5.0),
    vec3(4.0, 0.0, 0.0),
    vec3(0.0, 0.0, -4.0),
    lower_teal));

  camera cam;

  cam.aspect_ratio = 1.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 25;
  cam.max_depth = 10;
  cam.background = color(0.70, 0.80, 1.00);

  cam.vfov = 80;
  cam.lookfrom = point3(0.0, 0.0, 9.0);
  cam.lookat = point3(0.0, 0.0, 0.0);
  cam.vup = vec3(0.0, 1.0, 0.0);

  cam.defocus_angle = 0.0;

  cam.render(world);
}

void simple_light() {
  hittable_list world;

  std::shared_ptr<texture> pertex = std::make_shared<noise_texture>(4.0);

  world.add(std::make_shared<sphere>(
    point3(0.0, -1000.0, 0.0), 1000.0, std::make_shared<lambertian>(pertex)));
  world.add(std::make_shared<sphere>(
    point3(0.0, 2.0, 0.0), 2.0, std::make_shared<lambertian>(pertex)));

  std::shared_ptr<material> difflight =
    std::make_shared<diffuse_light>(color(4.0, 4.0, 4.0));
  world.add(std::make_shared<sphere>(
    point3(0.0, 7.0, 0.0), 2.0, difflight));
  world.add(std::make_shared<quad>(
    point3(3.0, 1.0, -2.0), vec3(2.0, 0.0, 0.0), vec3(0.0, 2.0, 0.0),
    difflight));

  camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 50;
  cam.max_depth = 10;
  cam.background = color(0.0, 0.0, 0.0);

  cam.vfov = 20.0;
  cam.lookfrom = point3(26.0, 3.0, 6.0);
  cam.lookat = point3(0.0, 2.0, 0.0);
  cam.vup = vec3(0.0, 1.0, 0.0);

  cam.defocus_angle = 0.0;

  cam.render(world);
}

void cornell_box() {
  hittable_list world;

  const std::shared_ptr<material> red =
    std::make_shared<lambertian>(color(0.65, 0.05, 0.05));
  const std::shared_ptr<material> white =
    std::make_shared<lambertian>(color(0.73, 0.73, 0.73));
  const std::shared_ptr<material> green =
    std::make_shared<lambertian>(color(0.12, 0.45, 0.15));
  const std::shared_ptr<material> light =
    std::make_shared<diffuse_light>(color(15.0, 15.0, 15.0));

  world.add(std::make_shared<quad>(
    point3(555.0, 0.0, 0.0),
    vec3(0.0, 555.0, 0.0),
    vec3(0.0, 0.0, 555.0),
    green));
  world.add(std::make_shared<quad>(
    point3(0.0, 0.0, 0.0),
    vec3(0.0, 555.0, 0.0),
    vec3(0.0, 0.0, 555.0),
    red));
  world.add(std::make_shared<quad>(
    point3(343.0, 544.0, 343.0),
    vec3(-130.0, 0.0, 0.0),
    vec3(0.0, 0.0, -105.0),
    light));
  world.add(std::make_shared<quad>(
    point3(0.0, 0.0, 0.0),
    vec3(555.0, 0.0, 0.0),
    vec3(0.0, 0.0, 555.0),
    white));
  world.add(std::make_shared<quad>(
    point3(555.0, 555.0, 555.0),
    vec3(-555.0, 0.0, 0.0),
    vec3(0.0, 0.0, -555.0),
    white));
  world.add(std::make_shared<quad>(
    point3(0.0, 0.0, 555.0),
    vec3(555.0, 0.0, 0.0),
    vec3(0.0, 555.0, 0.0),
    white));

  {
    std::shared_ptr<hittable> box1 =
      box(point3(0.0, 0.0, 0.0), point3(165.0, 330.0, 165.0), white);
    box1 = std::make_shared<rotate_y>(box1, 15.0);
    box1 = std::make_shared<translate>(box1, vec3(265.0, 0.0, 295.0));
    world.add(box1);
  }

  {
    std::shared_ptr<hittable> box2 =
      box(point3(0.0, 0.0, 0.0), point3(165.0, 165.0, 165.0), white);
    box2 = std::make_shared<rotate_y>(box2, -18.0);
    box2 = std::make_shared<translate>(box2, vec3(130.0, 0.0, 65.0));
    world.add(box2);
  }

  camera cam;
  
  cam.aspect_ratio = 1.0;
  cam.image_width = 600;
  cam.samples_per_pixel = 100;
  cam.max_depth = 15;
  cam.background = color(0.0, 0.0, 0.0);

  cam.vfov = 40.0;
  cam.lookfrom = point3(278.0, 278.0, -800.0);
  cam.lookat = point3(278.0, 278.0, 0.0);
  cam.vup = vec3(0.0, 1.0, 0.0);

  cam.defocus_angle = 0.0;

  cam.render(world);
}

void cornell_smoke() {
  hittable_list world;

  const std::shared_ptr<material> red =
    std::make_shared<lambertian>(color(0.65, 0.05, 0.05));
  const std::shared_ptr<material> white =
    std::make_shared<lambertian>(color(0.73, 0.73, 0.73));
  const std::shared_ptr<material> green =
    std::make_shared<lambertian>(color(0.12, 0.45, 0.15));
  const std::shared_ptr<material> light =
    std::make_shared<diffuse_light>(color(7.0, 7.0, 7.0));

  world.add(std::make_shared<quad>(
    point3(555.0, 0.0, 0.0),
    vec3(0.0, 555.0, 0.0),
    vec3(0.0, 0.0, 555.0),
    green));
  world.add(std::make_shared<quad>(
    point3(0.0, 0.0, 0.0),
    vec3(0.0, 555.0, 0.0),
    vec3(0.0, 0.0, 555.0),
    red));
  world.add(std::make_shared<quad>(
    point3(113.0, 554.0, 127.0),
    vec3(330.0, 0.0, 0.0),
    vec3(0.0, 0.0, 305.0),
    light));
  world.add(std::make_shared<quad>(
    point3(0.0, 0.0, 0.0),
    vec3(555.0, 0.0, 0.0),
    vec3(0.0, 0.0, 555.0),
    white));
  world.add(std::make_shared<quad>(
    point3(555.0, 555.0, 555.0),
    vec3(-555.0, 0.0, 0.0),
    vec3(0.0, 0.0, -555.0),
    white));
  world.add(std::make_shared<quad>(
    point3(0.0, 0.0, 555.0),
    vec3(555.0, 0.0, 0.0),
    vec3(0.0, 555.0, 0.0),
    white));

  {
    std::shared_ptr<hittable> box1 =
      box(point3(0.0, 0.0, 0.0), point3(165.0, 330.0, 165.0), white);
    box1 = std::make_shared<rotate_y>(box1, 15.0);
    box1 = std::make_shared<translate>(box1, vec3(265.0, 0.0, 295.0));
    world.add(std::make_shared<constant_medium>(
      box1, 0.01, color(0.0, 0.0, 0.0)));
  }

  {
    std::shared_ptr<hittable> box2 =
      box(point3(0.0, 0.0, 0.0), point3(165.0, 165.0, 165.0), white);
    box2 = std::make_shared<rotate_y>(box2, -18.0);
    box2 = std::make_shared<translate>(box2, vec3(130.0, 0.0, 65.0));
    world.add(std::make_shared<constant_medium>(
      box2, 0.01, color(1.0, 1.0, 1.0)));
  }

  camera cam;
  
  cam.aspect_ratio = 1.0;
  cam.image_width = 600;
  cam.samples_per_pixel = 100;
  cam.max_depth = 15;
  cam.background = color(0.0, 0.0, 0.0);

  cam.vfov = 40.0;
  cam.lookfrom = point3(278.0, 278.0, -800.0);
  cam.lookat = point3(278.0, 278.0, 0.0);
  cam.vup = vec3(0.0, 1.0, 0.0);

  cam.defocus_angle = 0.0;

  cam.render(world);
}

void final_scene(int image_width, int samples_per_pixel, int max_depth) {
  hittable_list boxes1;

  const std::shared_ptr<material> ground =
    std::make_shared<lambertian>(color(0.48, 0.83, 0.53));

  const int boxes_per_side = 20;
  const double w = 100.0;
  for (int i = 0; i < boxes_per_side; ++i) {
    for (int j = 0; j < boxes_per_side; ++j) {
      const double x0 = -1000.0 + i * w;
      const double z0 = -1000.0 + j * w;
      const double y0 = 0.0;
      const double x1 = x0 + w;
      const double y1 = random_double(1.0, 101.0);
      const double z1 = z0 + w;

      boxes1.add(box(point3(x0, y0, z0), point3(x1, y1, z1), ground));
    }
  }

  hittable_list world;

  world.add(std::make_shared<bvh_node>(boxes1));

  const std::shared_ptr<material> light =
    std::make_shared<diffuse_light>(color(7.0, 7.0, 7.0));
  world.add(std::make_shared<quad>(
            point3(123.0, 554.0, 147.0),
            vec3(300.0, 0.0, 0.0),
            vec3(0.0, 0.0, 265.0),
            light));

  const point3 center1 = point3(400.0, 400.0, 200.0);
  const point3 center2 = center1 + vec3(30.0, 0.0, 0.0);
  const std::shared_ptr<material> sphere_material =
    std::make_shared<lambertian>(color(0.7, 0.3, 0.1));
  world.add(std::make_shared<sphere>(
            center1, center2, 50.0, sphere_material));

  world.add(std::make_shared<sphere>(
            point3(260.0, 150.0, 45.0),
            50.0,
            std::make_shared<dielectric>(1.5)));
  world.add(std::make_shared<sphere>(
            point3(0.0, 150.0, 145.0),
            50.0,
            std::make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)));

  std::shared_ptr<hittable> boundary =
    std::make_shared<sphere>(
      point3(360.0, 150.0, 145.0),
      70.0,
      std::make_shared<dielectric>(1.5));
  world.add(boundary);
  world.add(std::make_shared<constant_medium>(
            boundary,
            0.2,
            color(0.2, 0.4, 0.9)));
  boundary = std::make_shared<sphere>(
    point3(0.0, 0.0, 0.0), 5000.0, std::make_shared<dielectric>(1.5));
  world.add(std::make_shared<constant_medium>(
            boundary,
            0.0001,
            color(1.0, 1.0, 1.0)));

  const std::shared_ptr<material> emat =
    std::make_shared<lambertian>(
      std::make_shared<image_texture>("earthmap.jpg"));
  world.add(std::make_shared<sphere>(
            point3(400.0, 200.0, 400.0),
            100.0,
            emat));
  const std::shared_ptr<texture> pertex =
    std::make_shared<noise_texture>(0.2);
  world.add(std::make_shared<sphere>(
            point3(220.0, 280.0, 300.0),
            80.0,
            std::make_shared<lambertian>(pertex)));

  hittable_list boxes2;
  const std::shared_ptr<material> white =
    std::make_shared<lambertian>(color(0.73, 0.73, 0.73));
  const int sphere_number = 1000;
  for (int j = 0; j < sphere_number; ++j) {
    boxes2.add(std::make_shared<sphere>(point3::random(0.0, 165.0), 10, white));
  }

  world.add(std::make_shared<translate>(
    std::make_shared<rotate_y>(
      std::make_shared<bvh_node>(boxes2),
      15),
    vec3(-100.0, 270.0, 395.0)));

  camera cam;

  cam.aspect_ratio = 1.0;
  cam.image_width = image_width;
  cam.samples_per_pixel = samples_per_pixel;
  cam.max_depth = max_depth;
  cam.background = color(0.0, 0.0, 0.0);

  cam.vfov = 40;
  cam.lookfrom = point3(478.0, 278.0, -600.0);
  cam.lookat = point3(278.0, 278.0, 0.0);
  cam.vup = vec3(0.0, 1.0, 0.0);

  cam.defocus_angle = 0.0;

  cam.render(world);
}

int main() {
  switch(9) {
    case 1:
      bouncing_spheres();
      break;
    case 2:
      checkered_spheres();
      break;
    case 3:
      earth();
      break;
    case 4:
      perlin_spheres();
      break;
    case 5:
      quads();
      break;
    case 6:
      simple_light();
      break;
    case 7:
      cornell_box();
      break;
    case 8:
      cornell_smoke();
      break;
    case 9:
      final_scene(800, 10000, 40);
      break;
    default:
      break;
  }
  return 0;
}
