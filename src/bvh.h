#ifndef _BVH_H_
#define _BVH_H_

#include "rtweekend.h"

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"

#include <algorithm>

class bvh_node : public hittable {
 public:
  bvh_node(hittable_list list)
  : bvh_node(list.objects, 0, list.objects.size()) {}

  bvh_node(
    std::vector<std::shared_ptr<hittable>>& objects,
    size_t start,
    size_t end) {
    m_bbox = aabb::empty;
    for (size_t object_index = start; object_index < end; ++object_index) {
      m_bbox = aabb(m_bbox, objects[object_index]->bounding_box());
    }
    int axis = m_bbox.longest_axis();

    auto comparator = (axis == 0) ? box_x_compare
                    : (axis == 1) ? box_y_compare
                                  : box_z_compare;

    const size_t object_span = end - start;
    if (object_span == 1) {
      m_left = objects[start];
      m_right = objects[start];
    } else if (object_span == 2) {
      m_left = objects[start];
      m_right = objects[start + 1];
    } else {
      std::sort(objects.begin() + start, objects.begin() + end, comparator);

      const size_t mid = start + object_span / 2;
      m_left = std::make_shared<bvh_node>(objects, start, mid);
      m_right = std::make_shared<bvh_node>(objects, mid, end);
    }
  }

  bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
    if (!m_bbox.hit(r, ray_t)) {
      return false;
    }

    bool hit_left = m_left->hit(r, ray_t, rec);
    bool hit_right = m_right->hit(
      r,
      interval(ray_t.min, hit_left ? rec.t : ray_t.max),
      rec);

    return hit_left || hit_right;
  }

  aabb bounding_box() const override { return m_bbox; }

 private: 
  std::shared_ptr<hittable> m_left;
  std::shared_ptr<hittable> m_right;
  aabb m_bbox;

  static bool box_compare(
    const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b,
    int axis_index) {
    const interval a_axis_interval =
      a->bounding_box().axis_interval(axis_index);
    const interval b_axis_interval =
      b->bounding_box().axis_interval(axis_index);

    return a_axis_interval.min < b_axis_interval.min;
  }

  static bool box_x_compare(
    const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
    return box_compare(a, b, 0);
  }

  static bool box_y_compare(
    const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
    return box_compare(a, b, 1);
  }

  static bool box_z_compare(
    const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
    return box_compare(a, b, 2);
  }
};

#endif  // _BVH_H_
