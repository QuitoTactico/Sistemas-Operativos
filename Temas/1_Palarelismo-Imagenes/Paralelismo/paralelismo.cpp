// g++ paralelismo.cpp -o paralelismoNO            ->    ./paralelismoNO 
// g++ paralelismo.cpp -fopenmp -o paralelismoSI   ->    ./paralelismoSI
// lo único fué usar #pragma omp (parallel for | atomic)

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <omp.h>

using namespace std;

int generarNumeroAleatorio(int min, int max){
    return min + rand() % (max - min + 1);
}

// para no duplicar la memoria, mejor le paso su apuntador
// al ser constante, disminuye el
void imprimirMatriz(const vector<vector<int>>& matriz){
    for(const auto& fila: matriz){
        for(int elemento: fila){
            cout << elemento << "\t";
        }
        cout << endl;
    }
}

vector<int> sumaColumnas(const vector<vector<int>>& matriz){
    int filas = matriz.size();
    int columnas = matriz[0].size();

    vector<int> suma(columnas, 0);  //(cantidad de posiciones, autorellenar con)
    
    #pragma omp parallel for
    for(int j = 0; j < filas; j++){
        for(int i = 0; i < columnas; i++){
            #pragma omp atomic
            suma[i] += matriz[j][i];
        }
    }

    return suma;
}

int main(){
    // semilla para número aleatorio
    srand(time(0));
    int filas, columnas;
    cout << "Cuantas filas? -> ";
    cin >> filas;
    cout << "\n" << "Cuantas columnas? -> ";
    cin >> columnas;
    cout << "\n";

    // le mando el número de filas y un vector de m posiciones lleno de ceros
    // se creó una matriz llena de ceros
    vector<vector<int>> matriz(filas, vector<int>(columnas, 0));

    for(int j=0; j < filas; j++){
        for(int i = 0; i < columnas; i++){
            matriz[j][i] = generarNumeroAleatorio(1,10);    //-----------------------------------
        }
    }

    cout << "Matriz generada" << endl;

    // se le preguna al usuario si se imprime o no, y se actua consiguiente
    char imprimir;
    cout << "Imprimir matriz? (s/n) -> ";
    cin >> imprimir;
    if(imprimir == 's'){
        imprimirMatriz(matriz);
    }

    auto start_time= chrono::high_resolution_clock::now();
    vector<int> sumaCols = sumaColumnas(matriz);
    auto end_time = chrono::high_resolution_clock::now();
    auto duracion = chrono::duration_cast<chrono::microseconds>(end_time-start_time);

    
    cout << "Suma de columnas:" << endl;

    if(imprimir == 's'){
        for(int valor: sumaCols){
            cout << valor << "\t";
        }
        cout << endl;
    }
    
    
    cout << "Duracion: " << duracion.count() << endl;

    return 0;
}
