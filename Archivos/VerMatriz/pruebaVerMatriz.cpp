//g++ -o vmi pruebaVerMatriz.cpp VisorMatriz.o -std=c++11 `pkg-config --cflags --libs opencv4`

#include <vector>
#include "VisorMatriz.h"

int main() {
    int m = 255; // Número de filas
    int n = 255; // Número de columnas
    int c = 3;   // Número de canales de color 3=RGB o 1=Gris

    // Crea una matriz de tamaño m x n x c
    std::vector<std::vector<std::vector<int>>> inputMatrix(m, std::vector<std::vector<int>>(n, std::vector<int>(c)));

    // Rellena la matriz con valores
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            // Asigna valores para cada píxel. Para imagenes en gris, el color es igual
            inputMatrix[i][j][0] = i;       //canal ROJO
            inputMatrix[i][j][1] = j;       //canal VERDE
            inputMatrix[i][j][2] = 256 - i; //canal AZUL
        }
    }

    // Crea un objeto VisorMatrizImg con la matriz generada
    VisorMatrizImg matrix2imagen(inputMatrix);

    // Visualiza la matriz como una imagen
    matrix2imagen.show("Matrix de color");

    return 0;
}
