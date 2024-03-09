#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <typeinfo>

#include "MatrixGenerator.cpp"

template <typename T>
class FileCreator {
public:
    static void createPlainTextFile(const std::vector<std::vector<T>>& matrix, const std::string& filename, int rows, int cols) {
        std::ofstream file(filename);

        if (file.is_open()) {
            file << "Tipo de Datos:" << typeid(T).name() << std::endl;
            file << "Dimensiones: " << rows << " x " << cols << std::endl;
            file << "Datos:\n";

            for (const auto& row : matrix) {
                for (const auto& value : row) {
                    //file << std::setw(8) << value;
                    file << " " << value;
                }
                file << '\n';
            }

            file.close();
            std::cout << "Archivo plano creado con éxito: " << filename << std::endl;
        } else {
            std::cerr << "Error al abrir el archivo: " << filename << std::endl;
        }
    }

    static void createBinaryFile(const std::vector<std::vector<T>>& matrix, const std::string& filename, int rows, int cols) {
        std::ofstream file(filename, std::ios::binary);

        if (file.is_open()) {
            file.write(reinterpret_cast<const char*>(&typeid(T)), sizeof(std::type_info));
            file.write(reinterpret_cast<const char*>(&rows), sizeof(int));
            file.write(reinterpret_cast<const char*>(&cols), sizeof(int));

            for (const auto& row : matrix) {
                file.write(reinterpret_cast<const char*>(row.data()), cols * sizeof(T));
            }

            file.close();
            std::cout << "Archivo binario creado con éxito: " << filename << std::endl;
        } else {
            std::cerr << "Error al abrir el archivo: " << filename << std::endl;
        }
    }
};
