#include <iostream>

#include "FileCreator.cpp"


int main(int argc, char *argv[]) {
    if (argc != 5) {
        std::cerr << "Uso: " << argv[0] << " <tipo> <filas> <columnas> <archivo>\n";
        std::cerr << "Donde <tipo> puede ser 'int' o 'real'.\n";
        return 1;
    }

    std::string tipo(argv[1]);
    int filas, columnas;
    std::istringstream(argv[2]) >> filas;
    std::istringstream(argv[3]) >> columnas;
    std::string archivo(argv[4]);

    if ((tipo != "int" && tipo != "real") || filas <= 0 || columnas <= 0) {
        std::cerr << "Error: Argumentos no válidos.\n";
        return 1;
    }

    if (tipo == "int") {
        MatrixGenerator<int> intMatrix(filas, columnas);
        intMatrix.printMatrix();
        FileCreator<int>::createPlainTextFile(intMatrix.matrix, archivo + "_int.txt", filas, columnas);
        FileCreator<int>::createBinaryFile(intMatrix.matrix, archivo + "_int.bin", filas, columnas);
    } else if (tipo == "real") {
        MatrixGenerator<double> doubleMatrix(filas, columnas);
        doubleMatrix.printMatrix();
        FileCreator<double>::createPlainTextFile(doubleMatrix.matrix, archivo + "_real.txt", filas, columnas);
        FileCreator<double>::createBinaryFile(doubleMatrix.matrix, archivo + "_real.bin", filas, columnas);
    }

    return 0;
}
