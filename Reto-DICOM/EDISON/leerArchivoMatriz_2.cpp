#include <iostream>
#include <fstream>
#include <vector>

template <typename T>
class MatrixLoader {
public:
    MatrixLoader() : rows(0), cols(0) {}

    void loadMatrixFromTextFile(const std::string& filename, int specifiedRows, int specifiedCols) {
        std::ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            std::cerr << "Error al abrir el archivo de texto " << filename << std::endl;
            return;
        }

        matrix.clear();
        T value;
        int currentCols = 0;
        while (inputFile >> value) {
            row.push_back(value);
            ++currentCols;
            if (specifiedCols > 0 && currentCols >= specifiedCols) {
                break;
            }
        }

        matrix.push_back(row);
        row.clear();
        ++rows;

        while (inputFile >> value) {
            row.push_back(value);
            ++currentCols;
            if (specifiedCols > 0 && currentCols >= specifiedCols) {
                matrix.push_back(row);
                row.clear();
                ++rows;
                currentCols = 0;
            }
        }

        inputFile.close();
        std::cout << "Matriz cargada desde el archivo de texto: " << filename << std::endl;
    }

    void loadMatrixFromBinaryFile(const std::string& filename, int specifiedRows, int specifiedCols) {
        std::ifstream inputFile(filename, std::ios::binary);
        if (!inputFile.is_open()) {
            std::cerr << "Error al abrir el archivo binario " << filename << std::endl;
            return;
        }

        matrix.clear();
        T value;
        int currentCols = 0;
        while (inputFile.read(reinterpret_cast<char*>(&value), sizeof(T))) {
            row.push_back(value);
            ++currentCols;
            if (specifiedCols > 0 && currentCols >= specifiedCols) {
                break;
            }
        }

        matrix.push_back(row);
        row.clear();
        ++rows;

        while (inputFile.read(reinterpret_cast<char*>(&value), sizeof(T))) {
            row.push_back(value);
            ++currentCols;
            if (specifiedCols > 0 && currentCols >= specifiedCols) {
                matrix.push_back(row);
                row.clear();
                ++rows;
                currentCols = 0;
            }
        }

        inputFile.close();
        std::cout << "Matriz cargada desde el archivo binario: " << filename << std::endl;
    }

    void displayMatrix() const {
        std::cout << "Matriz cargada:" << std::endl;
        for (const auto& row : matrix) {
            std::cout << "|";
            for (const auto& value : row) {
                std::cout << value << '|';
            }
            std::cout << std::endl;
        }
    }

private:
    int rows;
    int cols;
    std::vector<std::vector<T>> matrix;
    std::vector<T> row;
};

int main(int argc, char* argv[]) {
    if (argc != 6) {
        std::cerr << "Uso: " << argv[0] << " <tipo de datos> <nombre del archivo> <plano/binario> <filas> <columnas>" << std::endl;
        return 1;
    }

    std::string dataType = argv[1];
    std::string filename = argv[2];
    std::string format = argv[3];
    int specifiedRows = std::stoi(argv[4]);
    int specifiedCols = std::stoi(argv[5]);

    if (specifiedRows <= 0 || specifiedCols <= 0) {
        std::cerr << "Número de filas y columnas deben ser mayores que cero." << std::endl;
        return 1;
    }

    MatrixLoader<int> intMatrixLoader;
    MatrixLoader<float> floatMatrixLoader;

    if (dataType == "int") {
        if (format == "plano") {
            intMatrixLoader.loadMatrixFromTextFile(filename, specifiedRows, specifiedCols);
        } else if (format == "binario") {
            intMatrixLoader.loadMatrixFromBinaryFile(filename, specifiedRows, specifiedCols);
        } else {
            std::cerr << "Formato no válido. Las opciones son: plano, binario." << std::endl;
            return 1;
        }
        intMatrixLoader.displayMatrix();
    } else if (dataType == "float") {
        if (format == "plano") {
            floatMatrixLoader.loadMatrixFromTextFile(filename, specifiedRows, specifiedCols);
        } else if (format == "binario") {
            floatMatrixLoader.loadMatrixFromBinaryFile(filename, specifiedRows, specifiedCols);
        } else {
            std::cerr << "Formato no válido. Las opciones son: plano, binario." << std::endl;
            return 1;
        }
        floatMatrixLoader.displayMatrix();
    } else {
        std::cerr << "Tipo de datos no válido. Las opciones son: int, float." << std::endl;
        return 1;
    }

    return 0;
}
