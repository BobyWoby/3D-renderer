#include "graphics.h"
#include <cstdlib>

constexpr TGAColor white = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green = {0, 255, 0, 255};
constexpr TGAColor red = {0, 0, 255, 255};
constexpr TGAColor blue = {255, 128, 64, 255};
constexpr TGAColor yellow = {0, 200, 255, 255};
int main(int argc, char **argv) {
    constexpr int width = 128;
    constexpr int height = 128;

    TGAImage framebuffer(width, height, TGAImage::RGB);
    TGAImage diablo(640, 640, TGAImage::RGB);
    TGAImage zbuf(640, 640, TGAImage::GRAYSCALE);

    Model parser{};

    std::vector<vec3d> vertices = parser.getVertices();
    Model::normalizeVertices(vertices, 640, 640, 255);

    for (auto face : parser.getFaces()) {
        vec3d a = vertices.at(face[0] - 1);
        vec3d b = vertices.at(face[1] - 1);
        vec3d c = vertices.at(face[2] - 1);

        TGAColor rnd;
        for (int i = 0; i < 3; i++)
            rnd[i] = std::rand() % 255;
        draw::triangle(vertices.at(face[0] - 1), vertices.at(face[1] - 1), vertices.at(face[2] - 1), diablo, zbuf, rnd);
    }
    zbuf.write_tga_file("zbuffer.tga");
    diablo.write_tga_file("framebuffer.tga");
    return 0;
}
