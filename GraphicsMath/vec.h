#pragma once
#include <cassert>
#include <cmath>
#include <cstddef>
#include <string>
#include <utility>
#include <iostream>
#include <vector>

namespace gm{
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
    struct Matrix {
        public:
            int rows, cols;
            double **data;

            Matrix(int rows, int cols):rows(rows), cols(cols){
                alloc();
            }
            Matrix(double** init, int rows, int cols): rows(rows), cols(cols){
                alloc();
                for(int i = 0; i < rows; i++){
                    for(int j = 0; j < cols; j++){
                        data[i][j] = init[i][j];
                    }
                }
            }
            Matrix(const Matrix& other): Matrix(other.data, other.rows, other.cols){} // copy constructor
            Matrix(Matrix&& other) noexcept: data(std::exchange(other.data, nullptr)), rows(other.rows), cols(other.cols){}
            Matrix& operator=(const Matrix& other) noexcept {
                return *this = Matrix(other);
            }
            Matrix& operator=(Matrix&& other) noexcept{
                std::swap(this->data, other.data);
                rows = other.rows;
                cols = other.cols;
                return *this;
            }

            operator std::string() const{
                std::string out = "";
                for(int i = 0; i < rows; i++){
                    for(int j = 0; j < cols; j++){
                        out += std::to_string(data[i][j]) + " ";
                    }
                    out += '\n';
                }
                return out;
            }

            Matrix reduce(int excludeX, int excludeY){
                Matrix out = Matrix(this->rows - 1, this->cols -1);
                for(int i = 0; i < this->rows; i++){
                    for(int j = 0; j < this->cols; j++){
                        if(i == excludeX || j == excludeY) continue;
                        int iIndex = (i > excludeX)? i-1:i;
                        int jIndex = (j > excludeY)? j-1:j;
                        out.data[iIndex][jIndex] = this->data[i][j];
                    }
                }
                return out;
            }

            Matrix inverse(){
                // inverse = 1 / det(A) * Adj(A)
                // Adj(A) = transpose(cofactor(A))
                // cofactor(i, j) = -1^(i + j) * A[i][j] * det(reduce(A))

                if(rows != cols){
                    std::cerr << "Can't find inverse of a non-square matrix!" << std::endl;
                }
                assert(rows == cols);

                
                // calculate the cofactor Matrix
                Matrix out(this->rows, this->cols);
                for(int i = 0; i < this->rows; i++){
                    for(int j = 0; j< this->cols; j++){
                        //calculate the cofactor element
                        out.data[i][j] = std::pow(-1, i+j) * this->data[i][j] * determinant(this->reduce(i,j));
                    }
                }
                double det = determinant(*this);

                if(det == 0) std::cerr << "Can't find the inverse of a singular matrix!" << std::endl;
                assert(det != 0);
                //take the cofactor Matrix and transpose it to get the adjoint, then multiply by the recipricoal of the determinant
                return  out.transpose() * (1.0 / det);
            }

            Matrix transpose(){
                Matrix out(this->cols, this->rows);
                for(int i = 0; i < this->rows; i++){
                    for(int j = 0; j < this->cols; j++){
                        out.data[j][i] = this->data[i][j];
                    }
                }
                return out;
            }

            static double determinant(Matrix in){
                if(in.rows != in.cols){
                    std::cerr << "Can't find determinant of a non-square matrix!" << std::endl;
                    return 0;
                }
                assert(in.rows >= 2 && in.cols >= 2);
                assert(in.rows == in.cols);
                if(in.rows == 2){
                    // determinant of a 2x2 Matrix is ad - bc
                    return in.data[0][0] * in.data[1][1] - in.data[0][1] * in.data[1][0];
                }
                double sum = 0;
                for(int i = 0; i < in.cols; i++){
                    // Expand along the 1st row
                    std::cout << std::endl << i << " col: " << in.data[0][i] << std::endl;
                    std::cout << "-------------" << in.data[0][i] << std::endl;
                    std::cout << "std::pow(-1, " << i << "): " << std::pow(-1, i) << std::endl;
                    std::cout << "gm::Matrix::determinant(in.reduce(0, " << i << ")): " << gm::Matrix::determinant(in.reduce(0, i)) << std::endl;

                    sum += in.data[0][i] * std::pow(-1, i) * gm::Matrix::determinant(in.reduce(0, i));
                }
                return sum;
            }


            Matrix operator*( double scalar){
                Matrix out = Matrix(rows, cols);
                for(int i = 0; i < rows; i++){
                    for(int j = 0; j < cols; j++){
                        for(int k = 0; k < this->cols; k++){
                            out.data[i][j] = this->data[i][k] * scalar;
                        }
                    }
                }
                return out;
            }
            Matrix operator*(const Matrix& other){
                if(this->cols != other.rows){
                    std::cerr << "The two matrices you are trying to multiply have incompatible dimensions";
                    return *this;
                }
                Matrix out = Matrix(rows, other.cols);
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
            ~Matrix(){
                if(data == nullptr || data[0] == nullptr) return;
                for(int i = 0; i < rows; i++){
                    delete[] data[i]; // there's a segfault here for some reason idk y
                }
                delete[] data; // also an invalid read here according to valgrind 
            }
            
    };
}


