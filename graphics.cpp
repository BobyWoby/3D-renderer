#include "graphics.h"
#include "tgaimage.h"
#include <cmath>
#include <cstdint>

using namespace draw;

void draw::line(int ax, int ay, int bx, int by, TGAImage &framebuffer,
                TGAColor color) {
  bool steep = std::abs(by - ay) > std::abs(bx - ax);
  if (steep) {
    std::swap(ax, ay);
    std::swap(bx, by);
  }
  if (ax > bx) {
    std::swap(ax, bx);
    std::swap(ay, by);
  }
  float y = ay;
  for (int x = ax; x < bx; x++) {
    // double t = (double)(x - ax) / (bx - ax);
    // int y = ay + t * (by - ay);
    if (steep)
      framebuffer.set(y, x, color);
    else
      framebuffer.set(x, y, color);
    y += (by - ay) / (float)(bx - ax);
  }
}

double draw::signedTriangleArea(int ax, int ay, int bx, int by, int cx,
                                int cy) {
  return 0.5 * (cx * (by - ay) + bx * (ay - cy) + ax * (cy - by));
}

void draw::triangle(int ax, int ay, int az, int bx, int by, int bz, int cx,
                    int cy, int cz, TGAImage &framebuffer, TGAImage &zbuffer,
                    TGAColor color) {
  int bbminx = std::min(std::min(ax, bx), cx);
  int bbminy = std::min(std::min(ay, by), cy);
  int bbmaxx = std::max(std::max(ax, bx), cx);
  int bbmaxy = std::max(std::max(ay, by), cy);
  double total_area = signedTriangleArea(ax, ay, bx, by, cx, cy);

#pragma omp parallel for
  for (int i = bbminx; i < bbmaxx; i++) {
    for (int j = bbminy; j < bbmaxy; j++) {
      double alpha = signedTriangleArea(i, j, bx, by, cx, cy) / total_area;
      double beta = signedTriangleArea(ax, ay, i, j, cx, cy) / total_area;
      double gamma = signedTriangleArea(ax, ay, bx, by, i, j) / total_area;
      if (alpha < 0 || beta < 0 || gamma < 0)
        continue;
      uint8_t depth = alpha * az + beta * bz + gamma * cz;
      if (depth <= zbuffer.get(i, j)[0])
        continue;
      zbuffer.set(i, j, {depth});
      framebuffer.set(i, j, color);
    }
  }
}
void draw::triangle(vec3d a, vec3d b, vec3d c,
                    TGAImage &framebuffer, TGAImage &zbuffer, TGAColor color) {
  draw::triangle(a.x, a.y, a.z, b.x, b.y, b.z, c.x, c.y, c.z, framebuffer,
                 zbuffer, color);
}
