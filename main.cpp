#include "parseObj.h"
#include "tgaimage.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <regex>
#include <thread>
#include <utility>

constexpr TGAColor white = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green = {0, 255, 0, 255};
constexpr TGAColor red = {0, 0, 255, 255};
constexpr TGAColor blue = {255, 128, 64, 255};
constexpr TGAColor yellow = {0, 200, 255, 255};
void line(int ax, int ay, int bx, int by, TGAImage &framebuffer,
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

void scanLineTriangle(int ax, int ay, int bx, int by, int cx, int cy,
                      TGAImage &framebuffer, TGAColor color) {
  vec2<int> ySorted[3] = {vec2(ax, ay), vec2(bx, by), vec2(cx, cy)};
  int n = sizeof(ySorted) / sizeof(ySorted[0]);
  std::sort(ySorted, ySorted + n,
            [](vec2<int> l, vec2<int> r) { return l.y > r.y; });
  vec2<int> left{0, 0}, right{0, 0};
  if (ySorted[1].x > ySorted[2].x) {
    right = ySorted[1];
    left = ySorted[2];
  } else {
    left = ySorted[1];
    right = ySorted[2];
  }
  double ml = (ySorted[0].y - left.y) / (double)(ySorted[0].x - left.x);
  double mr = (ySorted[0].y - right.y) / (double)(ySorted[0].x - right.x);
  for (int i = ySorted[0].y; i > ySorted[1].y; i--) {
    int leftBound = (i - ySorted[0].y) / ml + ySorted[0].x;
    int rightBound = (i - ySorted[0].y) / mr + ySorted[0].x;
    for (int j = leftBound; j <= rightBound; j++) {
      framebuffer.set(j, i, color);
    }
  }

  double mc = (right.y - left.y) / (double)(right.x - left.x);
  if (left.y > right.y) {
    ml = (ySorted[0].y - right.y) / (double)(ySorted[0].x - right.x);
  } else {
    ml = (ySorted[0].y - left.y) / (double)(ySorted[0].x - left.x);
  }
  for (int i = ySorted[1].y; i > ySorted[2].y; i--) {
    int leftBound = (i - ySorted[0].y) / ml + ySorted[0].x;
    int rightBound = (i - right.y) / mc + right.x;
    for (int j = std::min(leftBound, rightBound);
         j <= std::max(leftBound, rightBound); j++) {
      framebuffer.set(j, i, color);
    }
  }
  line(ax, ay, bx, by, framebuffer, color);
  line(bx, by, cx, cy, framebuffer, color);
  line(cx, cy, ax, ay, framebuffer, color);
}

double signedTriangleArea(int ax, int ay, int bx, int by, int cx, int cy) {
  return 0.5 * (cx * (by - ay) + bx * (ay - cy) + ax * (cy - by));
}

void coloredTriangle(int ax, int ay, int az, int bx, int by, int bz, int cx, int cy, int cz,
              TGAImage &framebuffer) {
  int bbminx = std::min(std::min(ax, bx), cx);
  int bbminy = std::min(std::min(ay, by), cy);
  int bbmaxx = std::max(std::max(ax, bx), cx);
  int bbmaxy = std::max(std::max(ay, by), cy);
  double total_area = signedTriangleArea(ax, ay, bx, by, cx, cy);
  if (total_area<1) return; // backface culling + discarding triangles that cover less than a pixel

#pragma omp parallel for
  for (int i = bbminx; i < bbmaxx; i++) {
    for (int j = bbminy; j < bbmaxy; j++) {
      double alpha = signedTriangleArea(i, j, bx, by, cx, cy) / total_area;
      double beta = signedTriangleArea(ax, ay, i, j, cx, cy) / total_area;
      double gamma = signedTriangleArea(ax, ay, bx, by, i, j) / total_area;
      if (alpha < 0 || beta < 0 || gamma < 0)
        continue;
      if (alpha > 0.2 && beta > 0.2 && gamma > 0.2) continue;
      uint8_t blue = alpha * az;
      uint8_t red = beta * bz;
      uint8_t green = gamma * cz;
      TGAColor color{{blue, green, red, 1}, 3};
      framebuffer.set(i, j, color);
    }
  }
}

void triangle(int ax, int ay, int bx, int by, int cx, int cy,
              TGAImage &framebuffer, TGAColor color) {
  int bbminx = std::min(std::min(ax, bx), cx);
  int bbminy = std::min(std::min(ay, by), cy);
  int bbmaxx = std::max(std::max(ax, bx), cx);
  int bbmaxy = std::max(std::max(ay, by), cy);
  double total_area = signedTriangleArea(ax, ay, bx, by, cx, cy);
  if (total_area<1) return; // backface culling + discarding triangles that cover less than a pixel

#pragma omp parallel for
  for (int i = bbminx; i < bbmaxx; i++) {
    for (int j = bbminy; j < bbmaxy; j++) {
      double alpha = signedTriangleArea(i, j, bx, by, cx, cy) / total_area;
      double beta = signedTriangleArea(ax, ay, i, j, cx, cy) / total_area;
      double gamma = signedTriangleArea(ax, ay, bx, by, i, j) / total_area;
      if (alpha < 0 || beta < 0 || gamma < 0)
        continue;
      framebuffer.set(i, j, color);
    }
  }
}

void triangle(vec2<double> a, vec2<double> b, vec2<double> c,
              TGAImage &framebuffer, TGAColor color) {

  return triangle(a.x, a.y, b.x, b.y, c.x, c.y, framebuffer, color);
}

int main(int argc, char **argv) {
  constexpr int width = 128;
  constexpr int height = 128;

  TGAImage framebuffer(width, height, TGAImage::RGB);
  TGAImage diablo(640, 640, TGAImage::RGB);
  coloredTriangle(7, 45, 255, 35, 100, 255, 45, 60, 255, framebuffer);
  // triangle(120, 35, 90, 5, 45, 110, framebuffer, white);
  // triangle(115, 83, 80, 90, 85, 120, framebuffer, green);
  // triangle(vec2<double>(115, 83), vec2<double>(80, 90), vec2<double>(85, 120), framebuffer, green);

  framebuffer.write_tga_file("triangle.tga");
  ObjParser parser{};

  // std::vector<vec2<vec2<double>>> lines = parser.getLines();
  std::vector<vec3<double>> vertices = parser.getVertices();
  ObjParser::normalizeVertices(vertices, 640, 640, 4);

  for (auto face : parser.getFaces()) {
    vec2<double> a = ObjParser::orthoProject(vertices.at(face[0] - 1));
    vec2<double> b = ObjParser::orthoProject(vertices.at(face[1] - 1));
    vec2<double> c = ObjParser::orthoProject(vertices.at(face[2] - 1));
    // std::cout << "ax: " << a.x << ", ay: " << a.y << ", bx: " << b.x << ", by: " << b.y << ", cx: " << c.x << ", cy: " << c.y << std::endl;

    TGAColor rnd;
    for (int i = 0; i < 3; i++)
      rnd[i] = std::rand() % 255;

    triangle(a, b, c, diablo, rnd);
  }
  // for(auto l : lines){
  //     line(l[0].x, l[0].y, l[1].x, l[1].y, framebuffer, red);
  // }

  diablo.write_tga_file("framebuffer.tga");
  return 0;
}
