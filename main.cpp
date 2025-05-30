#include "GraphicsMath/vec.h"
#include "graphics.h"
#include <cstdlib>
#include <string>

constexpr TGAColor white = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green = {0, 255, 0, 255};
constexpr TGAColor red = {0, 0, 255, 255};
constexpr TGAColor blue = {255, 128, 64, 255};
constexpr TGAColor yellow = {0, 200, 255, 255};

std::ostream& operator<<(std::ostream& COUT, gm::Matrix mat){

    for(int i = 0; i < mat.rows; i++){
        for(int j = 0; j < mat.cols; j++){
            COUT << mat.data[i][j] << " ";
        }
        COUT << std::endl;
    }
    return COUT;
}



int main(int argc, char **argv) {
    constexpr int width = 128;
    constexpr int height = 128;

    int a = 3, b = 5, c;
    gm::Matrix mat = gm::Matrix(3, 3);
    double init[3][3] = {{1,3,4}, {1,4,2}, {5,6,2}};
    for(int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++) {
            // mat.data[i][j] = i * 3 + j + 1;
            mat.data[i][j] = init[i][j];
        }
    }
    std::cout << "Test Matrix:\n" << (std::string)mat << std::endl;

    // std::cout << gm::Matrix::determinant(mat.reduce(0, 0)) << std::endl;
    std::cout << "determinant: " << gm::Matrix::determinant(mat) << std::endl;
    std::cout << "inverse:\n " << (std::string)(mat.inverse()) << std::endl;
    std::cout << "test\n" << (std::string)(mat * mat.inverse()) << std::endl;


    // Commented this out to test matrix stuff
    // --------------------------------------

    // TGAImage framebuffer(width, height, TGAImage::RGB);
    // TGAImage diablo(640, 640, TGAImage::RGB);
    // TGAImage zbuf(640, 640, TGAImage::GRAYSCALE);
    // Model parser{};
    // std::vector<gm::vec3d> vertices = parser.getVertices();
    // Model::normalizeVertices(vertices, 640, 640, 255);
    //
    // for (auto face : parser.getFaces()) {
    //     gm::vec3d a = vertices.at(face[0] - 1);
    //     gm::vec3d b = vertices.at(face[1] - 1);
    //     gm::vec3d c = vertices.at(face[2] - 1);
    //
    //     TGAColor rnd;
    //     for (int i = 0; i < 3; i++)
    //         rnd[i] = std::rand() % 255;
    //     draw::triangle(vertices.at(face[0] - 1), vertices.at(face[1] - 1), vertices.at(face[2] - 1), diablo, zbuf, rnd);
    // }
    // zbuf.write_tga_file("zbuffer.tga");
    // diablo.write_tga_file("framebuffer.tga");



    return 0;
}
