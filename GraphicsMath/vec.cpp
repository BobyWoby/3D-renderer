#include "vec.h"



gm::Matrix operator*(double scalar, gm::Matrix A){
    gm::Matrix out = gm::Matrix(A.rows, A.cols);
    for(int i = 0; i < A.rows; i++){
        for(int j = 0; j < A.cols; j++){
            for(int k = 0; k < A.cols; k++){
                out.data[i][j] = A.data[i][k] * scalar;
            }
        }
    }
    return out;
}
namespace gm{
}
