#include <iostream>
#include <sstream>

#include "MatrixGenerator.cpp"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Uso: " << argv[0] << " <tipo> <filas> <columnas>\n";
        //std::cerr << "Donde <tipo> puede ser 'int', 'real' o 'char'.\n";
        std::cerr << "Donde <tipo> puede ser 'int' o 'real'.\n";
        return 1;
    }

    std::string tipo(argv[1]);
    int filas, columnas;
    
    std::istringstream(argv[2]) >> filas;
    std::istringstream(argv[3]) >> columnas;

    if (tipo != "int" && tipo != "real") {
        std::cerr << "Tipo no válido. Use 'int' o 'real'.\n";
        return 1;
    }

    if (filas <= 0 || columnas <= 0) {
        std::cerr << "El número de filas y columnas debe ser mayor que cero.\n";
        return 1;
    }

    if (tipo == "int") {
        MatrixGenerator<int> intMatrix(filas, columnas);
        intMatrix.printMatrix();
    } else if (tipo == "real") {
        MatrixGenerator<double> doubleMatrix(filas, columnas);
        doubleMatrix.printMatrix();
    } 

    return 0;
}
