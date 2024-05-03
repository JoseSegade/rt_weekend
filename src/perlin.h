#ifndef _PERLIN_H_
#define _PERLIN_H_

#include "rtweekend.h"

class perlin {
 public:
  perlin() {
    m_randvec = new vec3[m_point_count];
    for (int i = 0; i < m_point_count; ++i) {
      m_randvec[i] = unit_vector(vec3::random(-1.0, 1.0));
    }

    m_perm_x = perlin_generate_perm();
    m_perm_y = perlin_generate_perm();
    m_perm_z = perlin_generate_perm();
  }

  ~perlin() {
    delete[] m_randvec;
    delete[] m_perm_x;
    delete[] m_perm_y;
    delete[] m_perm_z;
  }

  double noise(const point3& p) const {
    double u = p.x() - floor(p.x());
    double v = p.y() - floor(p.y());
    double w = p.z() - floor(p.z());
    u = u * u * (3.0 - 2.0 * u);
    v = v * v * (3.0 - 2.0 * v);
    w = w * w * (3.0 - 2.0 * w);

    const int i = static_cast<int>(floor(p.x()));
    const int j = static_cast<int>(floor(p.y()));
    const int k = static_cast<int>(floor(p.z()));
    
    vec3 c[2][2][2];

    for (int di = 0; di < 2; ++di) {
      for (int dj = 0; dj < 2; ++dj) {
        for (int dk = 0; dk < 2; ++dk) {
          c[di][dj][dk] = m_randvec[
            m_perm_x[(i + di) & 255] ^
            m_perm_y[(j + dj) & 255] ^
            m_perm_z[(k + dk) & 255]
          ];
        }
      }
    }

    return perlin_interp(c, u, v, w);
  }

  double turb(const point3& p, int depth) const {
    double accum = 0.0;
    vec3 temp_p = p;
    double weight = 1.0;

    for (int i = 0; i < depth; ++i) {
      accum += weight * noise(temp_p);
      weight *= 0.5;
      temp_p *= 2.0;
    }

    return fabs(accum);
  }

 private:
  static const int m_point_count = 256;
  vec3* m_randvec;
  int* m_perm_x;
  int* m_perm_y;
  int* m_perm_z;

  static int* perlin_generate_perm() {
    int* p = new int[m_point_count];

    for (int i = 0; i < m_point_count; ++i) {
      p[i] = i;
    }

    permute(p, m_point_count);

    return p;
  }

  static void permute(int* p, int n) {
    for (int i = n - 1; i > 0; --i) {
      const int target = random_int(0, i);
      int tmp = p[i];
      p[i] = p[target];
      p[target] = tmp;
    }
  }

  static double perlin_interp(
    vec3 c[2][2][2], double u, double v, double w) {
    const double uu = u * u * (3.0 - 2.0 * u);
    const double vv = v * v * (3.0 - 2.0 * v);
    const double ww = w * w * (3.0 - 2.0 * w);

    double accum = 0.0;
    for (int i = 0; i < 2; ++i) {
      for (int j = 0; j < 2; ++j) {
        for (int k = 0; k < 2; ++k) {
          const vec3 weight_v(u - i, v - j, w - k);
          accum += (i * uu + (1 - i) * (1 - uu))
                 * (j * vv + (1 - j) * (1 - vv))
                 * (k * ww + (1 - k) * (1 - ww))
                 * dot(c[i][j][k], weight_v);
        }
      }
    }

    return accum;
  }

};

#endif  // _PERLIN_H_
