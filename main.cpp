#include "parseObj.h"
#include "tgaimage.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <regex>
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

void triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage &framebuffer, TGAColor color) {
    vec2<int> ySorted[3] = {vec2(ax, ay), vec2(bx, by), vec2(cx, cy)};
    // ySorted[0] = vec2(ax, ay);
    // ySorted[1] = vec2(bx, by);
    // ySorted[2] = vec2(cx, cy);
    
    int n = sizeof(ySorted)/sizeof(ySorted[0]);
    std::sort(ySorted, ySorted + n, [](vec2<int> l, vec2<int> r){return l.y > r.y;});
    vec2<int> left{0,0}, right{0,0};
    if(ySorted[1].x > ySorted[2].x){
        right = ySorted[1];
        left = ySorted[2];
    }
    else{
        left = ySorted[1];
        right = ySorted[2];

    }
    double ml = (ySorted[0].y - left.y) / (double)(ySorted[0].x - left.x);
    double mr = (ySorted[0].y - right.y) / (double)(ySorted[0].x - right.x);
    for(int i = ySorted[0].y; i > ySorted[1].y; i--){
        int leftBound = (i - ySorted[0].y) / ml + ySorted[0].x;
        int rightBound = (i - ySorted[0].y) / mr + ySorted[0].x;
        for(int j = leftBound; j <= rightBound; j++){
            framebuffer.set(j, i, color);
        }
    }
    
    double mc = (right.y - left.y) / (double)(right.x - left.x);
    if(left.y > right.y){
        ml = (ySorted[0].y - right.y) / (double)(ySorted[0].x - right.x);
    }
    else{
        ml = (ySorted[0].y - left.y) / (double)(ySorted[0].x - left.x);
    }
    for(int i = ySorted[1].y; i > ySorted[2].y; i--){
        int leftBound = (i - ySorted[0].y) / ml + ySorted[0].x;
        int rightBound = (i - right.y) / mc + right.x;
        for(int j = std::min(leftBound, rightBound); j <= std::max(leftBound, rightBound); j++){
            framebuffer.set(j, i, color);
        }
    }

    
    line(ax, ay, bx, by, framebuffer, color);
    line(bx, by, cx, cy, framebuffer, color);
    line(cx, cy, ax, ay, framebuffer, color);
}

int main(int argc, char **argv) {
    constexpr int width = 128;
    constexpr int height = 128;

    TGAImage framebuffer(width, height, TGAImage::RGB);
    triangle(  7, 45, 35, 100, 45,  60, framebuffer, red);
    triangle(120, 35, 90,   5, 45, 110, framebuffer, white);
    triangle(115, 50, 80,  90, 85, 120, framebuffer, green);
    framebuffer.write_tga_file("triangle.tga");
    ObjParser parser{};

    return 0;
}
