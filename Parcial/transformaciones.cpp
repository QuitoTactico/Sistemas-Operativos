#include <iostream>
#include "ioBMPExport.cpp"
#include <cmath>
using namespace std;

vector<vector<Pixel>> cizallarImagen(const vector<vector<Pixel>>& imagen, double angulo) {
    // Convierte el ángulo de grados a radianes
    double rad = angulo * M_PI / 180.0;

    // Calcula el tamaño de la nueva imagen
    int ancho = imagen[0].size();
    int alto = imagen.size();
    int nuevoAncho = abs(ancho + alto * tan(rad));

    // Crea la nueva imagen
    vector<vector<Pixel>> imagenRotada(alto, vector<Pixel>(nuevoAncho));

    return imagenRotada;
}

vector<vector<Pixel>> rotarImagen2(const vector<vector<Pixel>>& imagen, double angulo) {
    // Convierte el ángulo de grados a radianes
    double rad = angulo * M_PI / 180.0;

    // Calcula el tamaño de la nueva imagen
    int ancho = imagen[0].size();
    int alto = imagen.size();
    int nuevoAncho = abs(ancho * cos(rad)) + abs(alto * sin(rad));
    int nuevoAlto = abs(alto * cos(rad)) + abs(ancho * sin(rad));

    // Inicializa la nueva imagen completamente negra
    vector<vector<Pixel>> imagenRotada(nuevoAlto, vector<Pixel>(nuevoAncho, {0,0,0}));

    // Calcula el centro de la imagen original y la nueva imagen
    double centroX = ancho / 2.0;
    double centroY = alto / 2.0;
    double nuevoCentroX = nuevoAncho / 2.0;
    double nuevoCentroY = nuevoAlto / 2.0;

    // Itera sobre cada pixel de la antigua imagen
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            
            // Matriz de rotación simplificada (2x2): 
            // ------------------------------
            // |  cos(angulo) | sin(angulo) | 
            // |----------------------------|
            // | -sin(angulo) | cos(angulo) |
            // ------------------------------

            // Para cada pixel en la vieja imagen, busca cuál sería la posición de ese pixel en la imagen nueva
            int nuevoX = ((x - centroX) *   cos(rad)  + (y - centroY) * sin(rad)) + nuevoCentroX; 
            int nuevoY = ((x - centroX) * (-sin(rad)) + (y - centroY) * cos(rad)) + nuevoCentroY;

            
            imagenRotada[nuevoY][nuevoX] = imagen[y][x];
        }
    }

    return imagenRotada;
}

vector<vector<Pixel>> rotarImagen(const vector<vector<Pixel>>& imagen, double angulo) {
    // Convierte el ángulo de grados a radianes
    double rad = angulo * M_PI / 180.0;

    // Calcula el tamaño de la nueva imagen
    int ancho = imagen[0].size();
    int alto = imagen.size();
    int nuevoAncho = abs(ancho * cos(rad)) + abs(alto * sin(rad));
    int nuevoAlto = abs(alto * cos(rad)) + abs(ancho * sin(rad));

    // Inicializa la nueva imagen, posiciones vacías
    vector<vector<Pixel>> imagenRotada(nuevoAlto, vector<Pixel>(nuevoAncho));

    // Calcula el centro de la imagen original y la nueva imagen
    double centroX = ancho / 2.0;
    double centroY = alto / 2.0;
    double nuevoCentroX = nuevoAncho / 2.0;
    double nuevoCentroY = nuevoAlto / 2.0;

    // Itera sobre cada pixel de la nueva imagen
    for (int y = 0; y < nuevoAlto; y++) {
        for (int x = 0; x < nuevoAncho; x++) {
            
            // Matriz de rotación simplificada (2x2): 
            // ------------------------------
            // |  cos(angulo) | sin(angulo) | 
            // |----------------------------|
            // | -sin(angulo) | cos(angulo) |
            // ------------------------------

            // Para cada pixel en la nueva imagen, busca cuál sería la posición de ese pixel en la imagen original, haciendo todo el proceso al contrario
            int viejoX = ((x - nuevoCentroX) *   cos(-rad)  + (y - nuevoCentroY) * sin(-rad)) + centroX ; 
            int viejoY = ((x - nuevoCentroX) * (-sin(-rad)) + (y - nuevoCentroY) * cos(-rad)) + centroY ;

            // Si la ubicación en el pixel original cae dentro de los límites (de la original), usa el valor de esa posición en el nuevo pixel
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

    const char* nombreArchivo = argv[1];
    const int angulo = atoi(argv[2]);
    const vector<vector<Pixel>> imagen = leerArchivoBMP(nombreArchivo);

    int opcion;
    cout << "1. Rotar la imagen (Horariamente)\n2. Cizallar la imagen en x (Arriba hacia la derecha)\n" ;
    cin >> opcion;

    if(opcion == 1){
        cout << "Archivo input: " << nombreArchivo << "\nArchivo output: " << "rotar.bmp" << "\nÁngulo: " << angulo;

        const vector<vector<Pixel>> imagenRotada = rotarImagen(imagen, angulo);
        guardarMatrizEnBMP("rotar.bmp", imagenRotada);

        const vector<vector<Pixel>> imagenRotada2 = rotarImagen2(imagen, angulo);
        guardarMatrizEnBMP("rotar2.bmp", imagenRotada2);
    }else if(opcion == 2){
        cout << "Archivo input: " << nombreArchivo << "\nArchivo output: " << "cizallar.bmp" << "\nÁngulo: " << angulo;

        const vector<vector<Pixel>> imagenCizallada = cizallarImagen(imagen, angulo);

        guardarMatrizEnBMP("rotar.bmp", imagenCizallada);
    }else{
        cout << "Qué le pasa pa";
    }


    return 0;
}