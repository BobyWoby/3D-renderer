#include "parseObj.h"
#include <array>
#include <cassert>
#include <cmath>
#include <string>
#include <utility>
#include <vector>



std::vector<std::string> ObjParser::split(std::string str, std::string delim) {
    std::vector<std::string> output;
    while (str.find(delim) != std::string::npos) {
        output.push_back(str.substr(0, str.find(delim)));
        str = str.substr(str.find(delim) + 1);
    }
    output.push_back(str);
    return output;
    // v 0.11526 0.700717 0.0677257
}

void ObjParser::normalizeVertices(std::vector<vec3<double>> &in, int maxX, int maxY, int maxZ){

    for(auto &vert : in){
        vert.x = ++(vert.x) * maxX / 2;
        vert.y = ++(vert.y) * maxY / 2;
        vert.z = ++(vert.z) * maxZ / 2;
    }
}

std::vector<vec3<double>> ObjParser::getVertices() {
    std::ifstream file(filepath);
    std::string line = "";
    std::vector<vec3<double>> out;
    std::vector<std::string> splitLine;
    if (file.is_open()) {
        while (std::getline(file, line)) {
            splitLine = split(line, " ");
            if (splitLine.at(0) == "v") {
                // std::cout << line << std::endl;
                vec3<double> points{std::stod(splitLine.at(1)), std::stod(splitLine.at(2)), std::stod(splitLine.at(3))};
                out.push_back(points);
            }
        }
    }
    std::cout << out.size() << std::endl;
    return out;
}

std::vector<vec3<int>> ObjParser::getFaces(){
    std::ifstream file(filepath);
    std::vector<vec3<int>> faces;
    std::string line = "";
    std::vector<std::string> splitLine;
    if (file.is_open()) {
        while (std::getline(file, line)) {
            splitLine = split(line, " ");
            if (splitLine.at(0) == "f") {
                vec3<int> vertices{std::stoi(split(splitLine.at(1), "/").at(0)), std::stoi(split(splitLine.at(2), "/").at(0)), std::stoi(split(splitLine.at(3), "/").at(0))};
                faces.push_back(vertices);
            }
        }
    }
    std::cout << faces.size() << std::endl;

    return faces; 
}

vec2<double> ObjParser::projectPoint(vec3<double> pointPos, vec3<double> camPos, vec3<double> pointOrientation, vec3<double> screenSpacePos){
    int x = pointPos.x - camPos.x;
    int y = pointPos.y - camPos.y;
    int z = pointPos.z - camPos.z;
    
    double dx = std::cos(pointOrientation.y) * (std::sin(pointOrientation.z) * y + std::cos(pointOrientation.z) * x) - z * std::sin(pointOrientation.y);
    double dy = std::sin(pointOrientation.x) * (std::cos(pointOrientation.y) * z + std::sin(pointOrientation.y * (std::sin(pointOrientation.z) * y + std::cos(pointOrientation.z) * x))) +
        std::cos(pointOrientation.x) * (std::cos(pointOrientation.z) * y - std::sin(pointOrientation.z) * x);
    double dz = std::cos(pointOrientation.x) * (std::cos(pointOrientation.y) * z + std::sin(pointOrientation.y * (std::sin(pointOrientation.z) * y + std::cos(pointOrientation.z) * x))) - 
        std::sin(pointOrientation.x) * (std::cos(pointOrientation.z) * y - std::sin(pointOrientation.z) * x);

    
    double bx = screenSpacePos.z / dz * dx + screenSpacePos.x;
    double by = screenSpacePos.z / dz * dy + screenSpacePos.y;

    return vec2(bx, by);    
}
vec2<double> ObjParser::projectPoint(vec3<double> pointPos, vec3<double> camPos, vec3<double> screenSpacePos){
    double dx = pointPos.x - camPos.x;
    double dy = pointPos.y - camPos.y;
    double dz = pointPos.z - camPos.z;
    
    double bx = screenSpacePos.z / dz * dx + screenSpacePos.x;
    double by = screenSpacePos.z / dz * dy + screenSpacePos.y;
    
    return vec2(bx, by);
}
vec2<double> ObjParser::projectPoint(vec3<double> pointPos){
    return projectPoint(pointPos, vec3<double>(0,0,0), vec3<double>(0,0,1));
}

vec2<double> ObjParser::orthoProject(vec3<double> pointPos){
    return vec2<double>(pointPos.x, pointPos.y);
}


std::vector<vec2<vec2<double>>> ObjParser::getLines(){
    std::vector<vec2<vec2<double>>> points;
    std::vector<vec3<double>> vertices = getVertices();
    normalizeVertices(vertices, 640, 640, 4);
    std::vector<vec3<int>> faces = getFaces();

    for(auto face : faces){
        // TODO: FIX THIS PERSPECTIVE PROJECTION SOMETIME
        // for(int i = 0; i < 3; i++){
        //     int index1 = face[i] - 1, index2 = face[(i+1)>=3?1:i+1] - 1;
        //     vec2<vec2<double>> line = vec2<vec2<double>>(ObjParser::projectPoint(vertices.at(index1)), ObjParser::projectPoint(vertices.at(index2)));
        //     std::cout << "ax: " << line[0].x << ", ay: " << line[0].y << ", bx: " << line[1].x << ", by: " << line[1].y << std::endl;
        //     points.push_back(line);
        // }

        for(int i = 0; i < 3; i++){
            int index1 = face[i] - 1, index2 = face[(i+1)>=3?1:i+1] - 1;
            vec2<double> v1 = vec2<double>(vertices.at(index1).x, vertices.at(index1).y);
            vec2<double> v2 = vec2<double>(vertices.at(index2).x, vertices.at(index2).y);
            vec2<vec2<double>> line = vec2<vec2<double>>(v1, v2); 
            std::cout << "ax: " << line[0].x << ", ay: " << line[0].y << ", bx: " << line[1].x << ", by: " << line[1].y << std::endl;
            points.push_back(line);
        }

    }

    return points;
}
