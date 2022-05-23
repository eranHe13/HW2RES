//
// Created by ypinhas on 5/11/2022.
//

#ifndef PNEVERLAND_MATRIX2D_H
#define PNEVERLAND_MATRIX2D_H
#include <vector>
#include <stdexcept>

template <typename T>
class SquareMatrix2D {
    std::vector<T> data;
    unsigned int size;
public:
    SquareMatrix2D() {}

    T& operator()(unsigned int x, unsigned int y) {
        if (x >= size || y>= size)
            throw std::out_of_range("invalid access"); // Throw something more appropriate
        return data[size*y + x]; // Stride-aware access
    }

    void add(T edge){

    }
};

#endif //PNEVERLAND_MATRIX2D_H
