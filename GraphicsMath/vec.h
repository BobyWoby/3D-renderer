#pragma once
#include <cassert>
#include <cstddef>
#include <utility>
#include <iostream>
#include <vector>

namespace {
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

template<> struct vec<2, double>{
public:
    double x, y;
    
    vec<2,double>(double x, double y): x(x), y(y){}

    double& operator[](int index){
        switch(index){
            assert(index >=0 && index <=1);
            case 0:
                return x;
            case 1:
                return y;
            default:
                std::cerr << "Not a valid index" << std::endl;
                return x;
        }
    }
    vec<2, double> operator+(const vec<2, double>& other){
        return vec<2, double>(x + other.x, y + other.y);
    }
    vec<2, double> operator-(const vec<2, double>& other){
        return vec<2, double>(x - other.x, y - other.y);
    }
    double dot(vec<2, double> other){
        return x * other.x + y * other.y;
    }
};

template<> struct vec<3, double>{
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
    vec<3, double> operator+(const vec<3, double>& other){
        return vec<3, double>(x + other.x, y + other.y, z + other.z);
    }
    vec<3, double> operator-(const vec<3, double>& other){
        return vec<3, double>(x - other.x, y - other.y, z - other.z);
    }
    double dot(vec<3, double> other){
        return x * other.x + y * other.y + z * other.z;
    }
};

template<> struct vec<4, double>{
public:
    double x, y, z, w;
    vec<4,double>(double x, double y, double z, double w): x(x), y(y), z(z), w(w){}
    double& operator[](int index){
        switch(index){
            assert(index >=0 && index <=3);
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            case 3:
                return w;
            default:
                std::cerr << "Not a valid index" << std::endl;
                return x;
        }
    }
    vec<4, double> operator+(const vec<4, double>& other){
        return vec<4, double>(x + other.x, y + other.y, z + other.z, w + other.w);
    }
    vec<4, double> operator-(const vec<4, double>& other){
        return vec<4, double>(x - other.x, y - other.y, z - other.z, w + other.w);
    }
    double dot(vec<4, double> other){
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }
};

// Type aliases
typedef vec<2, double> vec2d;
typedef vec<3, double> vec3d;
typedef vec<4, double> vec4d;
typedef vec<3, int> vec3i;

// MATRICES
struct mat{
public:
    int rows, cols;
    double **data;

    mat(double** init, int rows, int cols): rows(rows), cols(cols){
        alloc();

        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                data[i][j] = init[i][j];
            }
        }
    }
    
    void alloc(){
        data = new double*[rows];
        for(int i = 0; i < rows; i++){
            data[i] = new double[cols];
        }
    }
    ~mat(){
        for(int i = 0; i < rows; i++){
            delete[] data[i];
        }
        delete[] data;
    }
};

}
    

