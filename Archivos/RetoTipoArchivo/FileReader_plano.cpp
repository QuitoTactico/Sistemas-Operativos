#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <typeinfo>
#include <stdexcept>

// Incluir la implementación de la clase MatrixGenerator
#include "MatrixGenerator.cpp"

// Incluir la implementación de la clase FileCreator
#include "FileCreator.cpp"

template <typename T>
class MatrixReader {
public:


    static void readMatrixFromFile(const std::string& filename) {
        std::ifstream file(filename);

        if (file.is_open()) {
            std::string line;

            // Leer y mostrar Tipo de Datos
            if (std::getline(file, line)) {
                // Separar la línea de Tipo de Datos para obtener el tipo ('i' o 'd')
                std::size_t colonPos = line.find(':');
                if (colonPos != std::string::npos && colonPos < line.size() - 1) {
                    char dataType = line[colonPos + 1];
                    // Determinar si es 'd' o 'i' y mostrar el tipo correspondiente
                    if (dataType == 'd') {
                        std::cout << "Tipo de datos en las celdas: (double) real" << std::endl;
                    } else if (dataType == 'i') {
                        std::cout << "Tipo de datos en las celdas: (int) entero" << std::endl;
                    } 
                }
            } else {
                std::cerr << "Error: No se pudo leer la línea de Tipo de Datos." << std::endl;
                file.close();
                //return;
            }


            //===============================================
            // Leer y mostrar Dimensiones
            if (std::getline(file, line)) {
                //std::cout << line << std::endl;
                // Separar la línea de dimensiones para obtener filas y columnas
                std::size_t colonPos = line.find(':');
                if (colonPos != std::string::npos && colonPos < line.size() - 1) {
                    std::string dimensions = line.substr(colonPos + 1);
                    std::istringstream dimStream(dimensions);
                    std::string rowsStr, colsStr;

                    // Leer el número antes de 'x' (filas)
                    if (std::getline(dimStream, rowsStr, 'x')) {
                        // Leer el número después de 'x' (columnas)
                        if (std::getline(dimStream, colsStr)) {
                            try {
                                int rows = std::stoi(rowsStr);
                                int cols = std::stoi(colsStr);
                                std::cout << "Dimensión de la matriz = ";
                                std::cout << " rows: " << rows;
                                std::cout << " cols: " << cols << std::endl;

                                //===================================================
                                // Leer y mostrar Datos
                                std::cout << "Datos extraidos:" << std::endl;
                                if (!std::getline(file, line)) {
                                        std::cerr << "Error: No se pudo leer la línea de datos." << std::endl;
                                        return;
                                }

                                for (int i = 0; i <= rows; ++i) {
                                    if (std::getline(file, line)) {
                                       // Separar la línea por valores y presentar con ancho de 10 caracteres y separador '|'
                                        std::istringstream valueStream(line);
                                        std::string value;
                                        std::cout << '|';
                                        while (valueStream >> value) {
                                            std::cout << std::setw(8) << value << '|';
                                        }
                                        std::cout << std::endl;
                                    } else {
                                        //std::cerr << "Error: No se pudo leer la línea de datos." << std::endl;
                                        break;
                                    }
                                }
                            } catch (const std::invalid_argument& e) {
                                std::cerr << "Error: No se puede convertir la dimensión a un entero." << std::endl;
                            }

                        } else {
                            std::cerr << "Error: Formato incorrecto en la línea de dimensiones." << std::endl;
                        }
                    } else {
                        std::cerr << "Error: Formato incorrecto en la línea de dimensiones." << std::endl;
                    }
                } else {
                    std::cerr << "Error: Formato incorrecto en la línea de dimensiones." << std::endl;
                }
            } else {
                std::cerr << "Error: No se pudo leer la línea de Dimensiones." << std::endl;
            }

            file.close();
            
        } else {
            std::cerr << "Error al abrir el archivo: " << filename << std::endl;
        }
    }

};

int main() {
    // Crear una matriz de punto flotante de 3x4
    MatrixGenerator<double> doubleMatrix(5, 4);
    //MatrixGenerator<int> intMatrix(5, 4);

    // Crear un archivo de texto plano
    FileCreator<double>::createPlainTextFile(doubleMatrix.matrix, "A1_real.txt", doubleMatrix.rows, doubleMatrix.cols);
    //FileCreator<int>::createPlainTextFile(intMatrix.matrix, "A1_int.txt", intMatrix.rows, intMatrix.cols);

    // Leer y mostrar el contenido del archivo
    MatrixReader<double>::readMatrixFromFile("A1_real.txt");
    //MatrixReader<int>::readMatrixFromFile("A1_int.txt");

    return 0;
}
