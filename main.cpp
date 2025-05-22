#include "parseObj.h"
#include "tgaimage.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
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

int main(int argc, char **argv) {
    constexpr int width = 640;
    constexpr int height = 640;
    TGAImage framebuffer(width, height, TGAImage::RGB);

    // int ax = 7, ay = 3;
    // int bx = 12, by = 37;
    // int cx = 62, cy = 53;
    // line(ax, ay, bx, by, framebuffer, blue);
    // line(cx, cy, bx, by, framebuffer, green);
    // line(cx, cy, ax, ay, framebuffer, yellow);
    // line(ax, ay, cx, cy, framebuffer, red);
    // framebuffer.set(ax, ay, white);
    // framebuffer.set(bx, by, white);
    // framebuffer.set(cx, cy, white);

    ObjParser parser{};
    // parser.getVertices();
    // parser.getFaces();
   
    std::vector<vec2<vec2<double>>> lines = parser.getLines();
    for(auto l : lines){
        line(l[0].x, l[0].y, l[1].x, l[1].y, framebuffer, red);
    }
    
    framebuffer.write_tga_file("framebuffer.tga");
    vec2<double> point = ObjParser::projectPoint(vec3<double>(0.11526, 0.700717, 0.0677257), vec3<double>(0,0,0), vec3<double>(0,0,1));
    std::cout << point.x << ", " << point.y << std::endl;

    return 0;
}
