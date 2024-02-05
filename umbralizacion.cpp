// g++ umbralizacion.cpp -fopenmp -o umbralizacion
// ./umbralizacion <nombre_del_archivo.bmp>

#include <iostream>
#include <omp.h> 
#include "leerImagenFileExport.cpp"
using namespace std;


vector<vector<char>> umbralizarMP(vector<vector<Pixel>> matriz, int umbral){
    int filas = matriz.size();
    int columnas = matriz[0].size();

    vector<vector<char>> matrizUmbralizada(filas, vector<char>(columnas));

    #pragma omp parallel for
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {

            #pragma opm atomic
            if (matriz[i][j].red > umbral) {
                matrizUmbralizada[i][j] = 1;
            } else {
                matrizUmbralizada[i][j] = 0;
            }
        }
    }
    return matrizUmbralizada;
}

vector<vector<char>> umbralizarMP2(vector<vector<Pixel>> matriz, int umbral){
    int filas = matriz.size();
    int columnas = matriz[0].size();

    vector<vector<char>> matrizUmbralizada(filas, vector<char>(columnas));

    #pragma omp parallel for
    for (int i = 0; i < columnas; ++i) {
        for (int j = 0; j < filas; ++j) {

            #pragma opm atomic
            if (matriz[j][i].red > umbral) {
                matrizUmbralizada[j][i] = 1;
            } else {
                matrizUmbralizada[j][i] = 0;
            }
        }
    }
    return matrizUmbralizada;
}

vector<vector<char>> umbralizarSec(vector<vector<Pixel>> matriz, int umbral){
    int filas = matriz.size();
    int columnas = matriz[0].size();

    vector<vector<char>> matrizUmbralizada(filas, vector<char>(columnas));

    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {

            if (matriz[i][j].red > umbral) {
                matrizUmbralizada[i][j] = 1;
            } else {
                matrizUmbralizada[i][j] = 0;
            }
        }
    }
    return matrizUmbralizada;
}


void verMatrizUmbralizada(int x1, int x2, int y1, int y2, vector<vector<char>> matrizUmbralizada){
        cout << "[Matriz Umbralizada] Algunos valores de escala de grises:" << endl;
        for (int i = x1; i < x2; ++i) {
            for (int j = y1; j < y2; ++j) {
                cout << "(" 
                    << static_cast<int>(matrizUmbralizada[i][j]) << ") ";
            }
            cout << endl;
        }
}


int main(int argc, char* argv[]){
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <nombre_del_archivo.bmp>" << endl;
        return {};
    }

    vector<vector<Pixel>> matriz = generarMatrizDeArchivo(argv[1]);

    // Multiprocesada iterando filas
    double start = omp_get_wtime();
    vector<vector<char>> matrizUmbralizadaMP  = umbralizarMP(matriz, 100);  
    double end = omp_get_wtime();
    cout << "Tiempo de ejecución (MP): " << end - start << " segundos" << endl;

    // Iterando columnas y no filas
    start = omp_get_wtime();
    vector<vector<char>> matrizUmbralizadaMP2 = umbralizarMP2(matriz, 100);
    end = omp_get_wtime();
    cout << "Tiempo de ejecución (MP2): " << end - start << " segundos" << endl;

    // Secuencial (sin OpenMP)
    start = omp_get_wtime();
    vector<vector<char>> matrizUmbralizadaSec = umbralizarSec(matriz, 100); 
    end = omp_get_wtime();
    cout << "Tiempo de ejecución (Secuencial): " << end - start << " segundos" << endl;

    verMatriz(100, 105, 200, 205, matriz);
    verMatrizUmbralizada(100, 105, 200, 205, matrizUmbralizadaMP);

    return 0;
}


// Parece que la versión multiprocesada iterando columnas fue más rápida que la versión iterando filas.
// La versión secuencial siempre fue la más lenta.