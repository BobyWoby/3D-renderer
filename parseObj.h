#pragma once
#include <string>
#include <utility>
#include <vector>
#include <array>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <cassert>

template<int n, typename T> struct vec{
    T data[n] = {0};
    
    T& operator[](const int i)       { assert(i>=0 && i<n); return data[i]; }
    T  operator[](const int i) const { assert(i>=0 && i<n); return data[i]; }

};
template<typename T> struct vec<2, T>{

    T x, y;
    
    vec<2,T>(T x, T y): x(x), y(y){
        
    }

    T& operator[](int index){
        assert(index >=0 && index <=1);
        switch(index){
            case 0:
                return x;
            case 1:
                return y;
            default:
                std::cerr << "Not a valid index" << std::endl;
                return x;
        }
    }
    vec<2,T>& operator=(const vec<2,T>& in){
        this->x = in.x;
        this->y = in.y;
        return *this;
    }
};

template<int n, typename T> std::ostream& operator<<(std::ostream& out, const vec<n, T>& v) {
    for (int i=0; i<n; i++) out << v[i] << " ";
    return out;
}

template<> struct vec<3, double>{
private:

public:
    double x, y, z;
    
    vec<3,double>(double x, double y, double z): x(x), y(y), z(z){}

    double& operator[](int index){
        switch(index){
            assert(index >=0 && index <=2);
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            default:
                std::cerr << "Not a valid index" << std::endl;
                return x;
        }
    }

    double dot(vec<3, double> other){
        return x * other.x + y * other.y + z * other.z;
    }
};


// Type aliases
typedef vec<2, double> vec2d;
typedef vec<3, double> vec3d;
typedef vec<3, int> vec3i;





class Model {
private:
    std::string filepath;
    std::vector<std::string> split(std::string str, std::string delim);    
public:
    Model(){
       filepath = "/home/bobywoby/dev/c++/tinyrenderer/obj/diablo3_pose/diablo3_pose.obj"; 
    }    
    
    Model(std::string f){
       filepath = f; 
    }    

    std::vector<vec3d> getVertices();
    static void normalizeVertices(std::vector<vec3d> &in, int maxX, int maxY, int maxZ);
    std::vector<vec3i> getFaces();
    std::vector<vec<2, vec2d>> getLines();
    static vec2d projectPoint(vec3d pointPos, vec3d camPos, vec3d pointOrientation, vec3d screenSpacePos);
    static vec2d projectPoint(vec3d pointPos, vec3d camPos, vec3d screenSpacePos);
    static vec2d projectPoint(vec3d pointPos);
    static vec2d orthoProject(vec3d pointPos);
};

