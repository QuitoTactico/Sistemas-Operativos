////g++ -c VisorMatriz.cpp -std=c++11 `pkg-config --cflags --libs opencv4`

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "VisorMatriz.h"


// Constructor que inicializa la matriz con la matriz dada como parámetro
VisorMatrizImg::VisorMatrizImg(const std::vector<std::vector<std::vector<int>>>& inputMatrix) {
    int rows = inputMatrix.size();
    int cols = inputMatrix[0].size();
      
    // Crea una matriz cv::Mat de tamaño rows x cols y 3 canales para color
    matrizImage = cv::Mat(rows, cols, CV_8UC3);
        
    // Rellena la matriz cv::Mat con los valores de la matriz de entrada
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Convierte el valor de int a cv::Vec3b (BGR)
            matrizImage.at<cv::Vec3b>(i, j) = cv::Vec3b(inputMatrix[i][j][0], inputMatrix[i][j][1], inputMatrix[i][j][2]);
        }
    }
}

// Método para visualizar la matriz como una imagen
void VisorMatrizImg::show(const std::string titulo) {
    cv::imshow(titulo, matrizImage);

    std::cout << "<<esc>> para cerar el visor \n";
    // Espera la entrada del teclado durante 100 milisegundos
    // Si se presiona la tecla ESC (27), se sale del programa
    while (true) {
        int key = cv::waitKey(100); // Espera 100 ms
        if (key == 27) { // Si la tecla presionada es ESC
            break; // Sale del bucle
        }
    }
}


