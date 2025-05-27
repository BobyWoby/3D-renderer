#pragma once
#include <cassert>
#include <cmath>
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
    struct mat {
        public:
            int rows, cols;
            double **data;

            mat(int rows, int cols):rows(rows), cols(cols){
                alloc();
            }
            mat(double** init, int rows, int cols): rows(rows), cols(cols){
                alloc();
                for(int i = 0; i < rows; i++){
                    for(int j = 0; j < cols; j++){
                        data[i][j] = init[i][j];
                    }
                }
            }

            mat reduce(int excludeX, int excludeY){
                mat out = mat(this->rows - 1, this->cols -1);
                for(int i = 0; i < this->rows; i++){
                    for(int j = 0; j < this->cols; j++){
                        if(i == excludeX || j == excludeY) continue;
                        out.data[i][j] = this->data[i][j];
                    }
                }
                return out;
            }

            mat inverse(){
                // inverse = 1 / det(A) * Adj(A)
                // Adj(A) = transpose(cofactor(A))
                // cofactor(i, j) = -1^(i + j) * A[i][j] * det(reduce(A))
                
                // calculate the cofactor matrix
                mat out(this->rows, this->cols);
                for(int i = 0; i < this->rows; i++){
                    for(int j = 0; j< this->cols; j++){
                        //calculate the cofactor element
                        out.data[i][j] = std::pow(-1, i+j) * this->data[i][j] * determinant(this->reduce(i,j));
                    }
                }

                //take the cofactor matrix and transpose it to get the adjoint, then multiply by the recipricoal of the determinant
                return  out.transpose() * (1.0 / determinant(*this));
            }

            mat transpose(){
                mat out(this->cols, this->rows);
                for(int i = 0; i < this->rows; i++){
                    for(int j = 0; j < this->cols; j++){
                        out.data[j][i] = this->data[i][j];
                    }
                }
                return out;
            }

            static double determinant(mat in){
                assert(in.rows >= 2 && in.cols >= 2);
                if(in.rows != in.cols){
                    std::cerr << "Can't find determinant of a non-square matrix!" << std::endl;
                    return 0;
                }
                if(in.rows == 2){
                    // determinant of a 2x2 matrix is ad - bc
                    return in.data[0][0] * in.data[1][1] - in.data[0][1] * in.data[1][0];
                }
                double sum = 0;
                for(int i = 0; i < in.cols; i++){
                    sum += in.data[0][i] * std::pow(-1, i) * determinant(in.reduce(0, i));
                }
                return sum;
            }

            mat operator*( double scalar){
                mat out = mat(rows, cols);
                for(int i = 0; i < rows; i++){
                    for(int j = 0; j < cols; j++){
                        for(int k = 0; k < this->cols; k++){
                            out.data[i][j] = this->data[i][k] * scalar;
                        }
                    }
                }
                return out;
            }
            mat operator*(const mat& other){
                if(this->cols != other.rows){
                    std::cerr << "The two matrices you are trying to multiply have incompatible dimensions";
                    return *this;
                }
                mat out = mat(rows, other.cols);
                for(int i = 0; i < this->rows; i++){
                    for(int j = 0; j < other.cols; j++){
                        for(int k = 0; k < this->cols; k++){
                            out.data[i][j] = this->data[i][k] * other.data[k][j];
                        }
                    }
                }
                return out;
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
    mat operator*(double scalar, mat A){
        mat out = mat(A.rows, A.cols);
        for(int i = 0; i < A.rows; i++){
            for(int j = 0; j < A.cols; j++){
                for(int k = 0; k < A.cols; k++){
                    out.data[i][j] = A.data[i][k] * scalar;
                }
            }
        }
        return out;
    }
}


