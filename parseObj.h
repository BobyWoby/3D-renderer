#pragma once
#include <string>
#include <utility>
#include <vector>
#include <array>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>

template<typename T> class vec2{
private:

public:
    T x, y;
    
    vec2(T x, T y): x(x), y(y){}

    T& operator[](int index){
        switch(index){
            case 0:
                return x;
            case 1:
                return y;
            default:
                std::cerr << "Not a valid index" << std::endl;
        }
    }
    vec2& operator=(const vec2& in){
        this->x = in.x;
        this->y = in.y;
        return *this;
    }
};
template<typename T> class vec3{
private:

public:
    T x, y, z;
    
    vec3(T x, T y, T z): x(x), y(y), z(z){}

    T& operator[](int index){
        switch(index){
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            default:
                std::cerr << "Not a valid index" << std::endl;
        }
    }
};

class ObjParser {
private:
    std::string filepath;
    std::vector<std::string> split(std::string str, std::string delim);    
public:
    ObjParser(){
       filepath = "/home/bobywoby/dev/c++/tinyrenderer/obj/diablo3_pose/diablo3_pose.obj"; 
    }    
    
    ObjParser(std::string f){
       filepath = f; 
    }    

    std::vector<vec3<double>> getVertices();
    static void normalizeVertices(std::vector<vec3<double>> &in, int maxX, int maxY, int maxZ);
    std::vector<vec3<int>> getFaces();
    std::vector<vec2<vec2<double>>> getLines();
    static vec2<double> projectPoint(vec3<double> pointPos, vec3<double> camPos, vec3<double> pointOrientation, vec3<double> screenSpacePos);
    static vec2<double> projectPoint(vec3<double> pointPos, vec3<double> camPos, vec3<double> screenSpacePos);
    static vec2<double> projectPoint(vec3<double> pointPos);
    static vec2<double> orthoProject(vec3<double> pointPos);
};

