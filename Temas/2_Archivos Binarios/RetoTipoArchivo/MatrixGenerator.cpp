
#pragma once

#include <iostream>
#include <iomanip>
#include <random>
#include <vector>

template <typename T>
class MatrixGenerator {
public:

    int rows;
    int cols;
    std::vector<std::vector<T>> matrix;
    
    MatrixGenerator(int rows, int cols)
        : rows(rows), cols(cols), matrix(rows, std::vector<T>(cols)) {
            generateMatrix();
        }

    void printMatrix() const {
        std::cout << "Matriz GENERADA:" << std::endl;
        for (const auto& row : matrix) {
            std::cout << "|";
            for (const auto& value : row) {
                    std::cout << std::setw(8) << value << '|';
            }
            std::cout << std::endl;
        }
    }


private:
    void generateMatrix() {
        std::random_device rd;
        std::mt19937 generator(rd());

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                matrix[i][j] = generateRandomValue(generator);
            }
        }

    }

    T generateRandomValue(std::mt19937& generator) {
        if constexpr (std::is_integral<T>::value) {
            std::uniform_int_distribution<T> distribution(0, 9);
            return distribution(generator) + '0';
        } else if constexpr (std::is_floating_point<T>::value) {
            std::uniform_real_distribution<T> distribution(1.0, 100.0);
            return distribution(generator);
        } 
    }
};
