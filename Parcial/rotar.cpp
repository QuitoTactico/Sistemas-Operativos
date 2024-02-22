#include <iostream>
#include "ioBMPExport.cpp"
#include <cmath>
using namespace std;

vector<vector<Pixel>> rotarImagen(const vector<vector<Pixel>>& imagen, double angulo) {
    // Convierte el ángulo de grados a radianes
    double rad = angulo * M_PI / 180.0;

    // Calcula el tamaño de la nueva imagen
    int ancho = imagen[0].size();
    int alto = imagen.size();
    int nuevoAncho = abs(ancho * cos(rad)) + abs(alto * sin(rad));
    int nuevoAlto = abs(alto * cos(rad)) + abs(ancho * sin(rad));

    // Crea la nueva imagen
    vector<vector<Pixel>> imagenRotada(nuevoAlto, vector<Pixel>(nuevoAncho));

    // Calcula el centro de la imagen original y la nueva imagen
    double centroX = ancho / 2.0;
    double centroY = alto / 2.0;
    double nuevoCentroX = nuevoAncho / 2.0;
    double nuevoCentroY = nuevoAlto / 2.0;

    // Itera sobre cada pixel en la nueva imagen
    for (int y = 0; y < nuevoAlto; y++) {
        for (int x = 0; x < nuevoAncho; x++) {
            // Mapea el pixel en la nueva imagen de vuelta a la imagen original usando una transformación de rotación
            int viejoX = centroX + (x - nuevoCentroX) * cos(rad) + (y - nuevoCentroY) * sin(rad); //Matriz de rotación: cos(angulo) sin(angulo) 
            int viejoY = centroY - (x - nuevoCentroX) * sin(rad) + (y - nuevoCentroY) * cos(rad);//                     -sin(angulo) cos(angulo)

            // Si la ubicación mapeada cae dentro de los límites de la imagen original, usa el valor del pixel de la imagen original
            if (viejoX >= 0 && viejoX < ancho && viejoY >= 0 && viejoY < alto) {
                imagenRotada[y][x] = imagen[viejoY][viejoX];
            }
            // Si cae fuera, establece el pixel a un valor predeterminado (negro en este caso)
            else {
                imagenRotada[y][x] = {0, 0, 0};
            }
        }
    }

    return imagenRotada;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Uso: " << argv[0] << "<nombre_del_archivo_entrada.bmp> <angulo (0 - 359)>" << endl;
        return 1;
    }

    const char* nombreArchivoLecturaBMP = argv[1];
    const int angulo = atoi(argv[2]);

    const vector<vector<Pixel>> imagen = leerArchivoBMP(nombreArchivoLecturaBMP);
    const vector<vector<Pixel>> imagenRotada = rotarImagen(imagen, angulo);

    guardarMatrizEnBMP("rotar.bmp", imagenRotada);
    return 0;
}