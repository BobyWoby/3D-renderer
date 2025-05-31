#include "parseObj.h"
#include <array>
#include <cassert>
#include <cmath>
#include <string>
#include <utility>
#include <vector>



std::vector<std::string> Model::split(std::string str, std::string delim) {
    std::vector<std::string> output;
    while (str.find(delim) != std::string::npos) {
        output.push_back(str.substr(0, str.find(delim)));
        str = str.substr(str.find(delim) + 1);
    }
    output.push_back(str);
    return output;
    // v 0.11526 0.700717 0.0677257
}

void Model::normalizeVertices(std::vector<gm::vec3d> &in, int maxX, int maxY, int maxZ){

    for(auto &vert : in){
        vert.x = ++(vert.x) * maxX / 2;
        vert.y = ++(vert.y) * maxY / 2;
        vert.z = ++(vert.z) * maxZ / 2;
    }
}
void Model::normalizeVertex(gm::vec3d &vert, int maxX, int maxY, int maxZ){
    vert.x = ++(vert.x) * maxX / 2;
    vert.y = ++(vert.y) * maxY / 2;
    vert.z = ++(vert.z) * maxZ / 2;
    // vert.z = std::max(0.0, std::min(vert.z, (double)maxZ));
}

std::vector<gm::vec3d> Model::getVertices() {
    std::ifstream file(filepath);
    std::string line = "";
    std::vector<gm::vec3d> out;
    std::vector<std::string> splitLine;
    if (file.is_open()) {
        while (std::getline(file, line)) {
            splitLine = split(line, " ");
            if (splitLine.at(0) == "v") {
                // std::cout << line << std::endl;
                gm::vec3d points{std::stod(splitLine.at(1)), std::stod(splitLine.at(2)), std::stod(splitLine.at(3))};
                out.push_back(points);
            }
        }
    }
    std::cout << out.size() << std::endl;
    return out;
}

std::vector<gm::vec3i> Model::getFaces(){
    std::ifstream file(filepath);
    std::vector<gm::vec3i> faces;
    std::string line = "";
    std::vector<std::string> splitLine;
    if (file.is_open()) {
        while (std::getline(file, line)) {
            splitLine = split(line, " ");
            if (splitLine.at(0) == "f") {
                gm::vec3i vertices{std::stoi(split(splitLine.at(1), "/").at(0)), std::stoi(split(splitLine.at(2), "/").at(0)), std::stoi(split(splitLine.at(3), "/").at(0))};
                faces.push_back(vertices);
            }
        }
    }
    std::cout << faces.size() << std::endl;

    return faces; 
}

gm::vec3d Model::projectPoint(gm::vec3d pointPos, double focalDist){
    // x' = x / (1 - z /c)
    // y' = y / (1 - z /c)
    return pointPos / (1 - pointPos.z / focalDist);
    
}

gm::vec2d Model::orthoProject(gm::vec3d pointPos){
    return gm::vec2d(pointPos.x, pointPos.y);
}



std::vector<gm::vec<2, gm::vec2d>> Model::getLines(){
    std::vector<gm::vec<2, gm::vec2d>> points;
    std::vector<gm::vec3d> vertices = getVertices();
    normalizeVertices(vertices, 640, 640, 4);
    std::vector<gm::vec3i> faces = getFaces();

    for(auto face : faces){
        // TODO: FIX THIS PERSPECTIVE PROJECTION SOMETIME
        // for(int i = 0; i < 3; i++){
        //     int index1 = face[i] - 1, index2 = face[(i+1)>=3?1:i+1] - 1;
        //     gm::vec<2, gm::vec2d> line = gm::vec<2, gm::vec2d>(ObjParser::projectPoint(vertices.at(index1)), ObjParser::projectPoint(vertices.at(index2)));
        //     std::cout << "ax: " << line[0].x << ", ay: " << line[0].y << ", bx: " << line[1].x << ", by: " << line[1].y << std::endl;
        //     points.push_back(line);
        // }

        for(int i = 0; i < 3; i++){
            int index1 = face[i] - 1, index2 = face[(i+1)>=3?1:i+1] - 1;
            gm::vec2d v1 = gm::vec2d(vertices.at(index1).x, vertices.at(index1).y);
            gm::vec2d v2 = gm::vec2d(vertices.at(index2).x, vertices.at(index2).y);
            gm::vec<2, gm::vec2d> line = gm::vec<2, gm::vec2d>(v1, v2); 
            std::cout << "ax: " << line[0].x << ", ay: " << line[0].y << ", bx: " << line[1].x << ", by: " << line[1].y << std::endl;
            points.push_back(line);
        }

    }

    return points;
}
