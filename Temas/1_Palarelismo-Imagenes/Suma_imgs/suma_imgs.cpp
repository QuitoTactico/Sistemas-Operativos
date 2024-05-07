#include <iostream>
#include "ioBMPExport.cpp"
using namespace std;

vector<vector<Pixel>> sumarImagenes(vector<vector<Pixel>> imagen1, vector<vector<Pixel>> imagen2){
    // i want the maximum size of the two images to be the filas
    int filas = max(imagen1.size(), imagen2.size());
    int columnas = max(imagen1[0].size(), imagen2[0].size());

    vector<vector<Pixel>> imagenesSumadas(filas, vector<Pixel>(columnas));

    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {

            if ((i >= imagen1.size() || j >= imagen1[0].size()) && (i >= imagen2.size() || j >= imagen2[0].size())) {
                imagenesSumadas[i][j].red = 0;
                imagenesSumadas[i][j].green = 0;
                imagenesSumadas[i][j].blue = 0;
                continue;
            }
            if (i >= imagen1.size() || j >= imagen1[0].size()) {
                imagenesSumadas[i][j] = imagen2[i][j];
                continue;
            }
            if (i >= imagen2.size() || j >= imagen2[0].size()) {
                imagenesSumadas[i][j] = imagen1[i][j];
                continue;
            }

            imagenesSumadas[i][j].red = (imagen1[i][j].red + imagen2[i][j].red)/2;
            imagenesSumadas[i][j].green = (imagen1[i][j].green + imagen2[i][j].green)/2;
            imagenesSumadas[i][j].blue = (imagen1[i][j].blue + imagen2[i][j].blue)/2;
        }
    }
    return imagenesSumadas;
}

int main(int argc, char* argv[]){
    if (argc != 3) {
        cerr << "Uso: " << argv[0] << " <imagen1.bmp> <imagen2.bmp>" << endl;
        return 1;
    }

    const vector<vector<Pixel>> imagen1 = leerArchivoBMP(argv[1]);
    const vector<vector<Pixel>> imagen2 = leerArchivoBMP(argv[2]);


    vector<vector<Pixel>> imagenesSumadas  = sumarImagenes(imagen1, imagen2);  



    guardarMatrizEnBMP("suma_imgs.bmp", imagenesSumadas);

    return 0;
}