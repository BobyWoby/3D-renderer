#pragma once

#include "GraphicsMath/vec.h"
#include "parseObj.h"
#include "tgaimage.h"


namespace draw{
    void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color);

    double signedTriangleArea(int ax, int ay, int bx, int by, int cx, int cy);
    void triangle(int ax, int ay, int az, int bx, int by, int bz, int cx, int cy, int cz, TGAImage &framebuffer, TGAImage &zbuffer, TGAColor color);
    void triangle(int ax, int ay, int az, int bx, int by, int bz, int cx, int cy, int cz, TGAImage &framebuffer, std::vector<double> &zbuffer, TGAColor color);
    void triangle(gm::vec3d a, gm::vec3d b, gm::vec3d c, TGAImage &framebuffer, std::vector<double> &zbuffer, TGAColor color);
    void triangle(gm::vec3d a, gm::vec3d b, gm::vec3d c, TGAImage &framebuffer, TGAImage &zbuffer, TGAColor color);

    gm::vec3d rot(gm::vec3d in, gm::vec3d rotation);
}
