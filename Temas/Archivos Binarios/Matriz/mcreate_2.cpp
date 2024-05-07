#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
#include <type_traits>
#include <vector>

template <typename T>
class MatrixGenerator {
public:
    MatrixGenerator(int rows, int cols)
        : rows(rows), cols(cols), matrix(rows, std::vector<T>(cols)) {}

    void generateMatrix() {
        std::random_device rd;
        std::mt19937 generator(rd());

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                matrix[i][j] = generateRandomValue(generator);
            }
        }

        printMatrix();
    }

    void saveMatrixToFile(const std::string& filename, bool binary) const {
        std::ofstream outputFile;
        if (binary) {
            outputFile.open(filename, std::ios::out | std::ios::binary);
        } else {
            outputFile.open(filename);
        }

        if (!outputFile.is_open()) {
            std::cerr << "Error al abrir el archivo " << filename << std::endl;
            return;
        }

        for (const auto& row : matrix) {
            for (const auto& value : row) {
                outputFile << value << ' ';
            }
            outputFile << '\n';
        }

        outputFile.close();
        std::cout << "Matriz guardada en el archivo: " << filename << std::endl;
    }

private:
    void printMatrix() const {
        std::cout << "Matriz generada:" << std::endl;
        for (const auto& row : matrix) {
            std::cout << "|";
            for (const auto& value : row) {
                std::cout << std::setw(8) << value << '|';
            }
            std::cout << std::endl;
        }
    }

    T generateRandomValue(std::mt19937& generator) {
        if constexpr (std::is_integral<T>::value) {
            std::uniform_int_distribution<T> distribution(0, 9);
            return distribution(generator) + '0';
        } else if constexpr (std::is_floating_point<T>::value) {
            std::uniform_real_distribution<T> distribution(1.0, 100.0);
            return distribution(generator);
        } else {
            static_assert(std::is_same<T, char>::value, "Unsupported data type");
            std::uniform_int_distribution<int> distribution(0, 51);
            int asciiOffset = (distribution(generator) < 26) ? 'A' : 'a';
            return static_cast<T>(distribution(generator) + asciiOffset);
        }
    }

    int rows;
    int cols;
    std::vector<std::vector<T>> matrix;
};

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Uso: " << argv[0] << " <filas> <columnas> <tipo de datos> <nombre del archivo>" << std::endl;
        return 1;
    }

    int rows = std::stoi(argv[1]);
    int cols = std::stoi(argv[2]);
    std::string dataType = argv[3];
    std::string filename = argv[4];
    bool isBinary = false;  // Opcional: puedes modificar esto según tus necesidades.

    if (dataType == "int") {
        MatrixGenerator<int> matrixGenerator(rows, cols);
        matrixGenerator.generateMatrix();
        matrixGenerator.saveMatrixToFile(filename, isBinary);
    } else if (dataType == "float") {
        MatrixGenerator<float> matrixGenerator(rows, cols);
        matrixGenerator.generateMatrix();
        matrixGenerator.saveMatrixToFile(filename, isBinary);
    } else {
        std::cerr << "Tipo de datos no válido. Las opciones son: int, float." << std::endl;
        return 1;
    }

    return 0;
}
