#include <iostream>
#include "ioBMPExport.cpp"
#include <cmath>
#include <omp.h>
using namespace std;

/*  Calculamos las dimensiones de la nueva imagen
    Inicializamos esa nueva imagen en negro
    Calculamos los centros de la vieja y la nueva imagen
    Iterando sobre los píxeles de la vieja imagen:
        Le restamos el viejo centro
        Le aplicamos la matriz simplificada de rotación
        Le sumamos el nuevo centro
        Obtenemos las coordenadas correspondientes al nuevo pixel
        Si está en los límites de la nueva imagen (a veces se sale por culpa del redondeo) lo pintamos

    Cada pixel de la vieja pinta uno en la nueva. A cada pixel de la nueva le corresponde
    sólo un pixel de la vieja.
    Pero hay algunos píxeles de la nueva imagen en donde la fórmula no cae a causa del redondeo,
    así que nunca son pintados. La imagen rotada queda con un patrón de puntos negros.
    El redondeo generó un desgarro (separó dos resultados vecinos hacia lados opuestos o en diferentes ejes).
*/
vector<vector<Pixel>> rotarImagen(const vector<vector<Pixel>>& imagen, double angulo) {
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

    // Esto estaba causando el patrón chistoso en los bordes jajjs
    //int nuevoX, nuevoY;

    // Itera sobre cada pixel de la antigua imagen
    #pragma omp parallel
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            
            // Matriz de rotación simplificada (2x2): 
            //  ---------------------------- 
            // |  cos(angulo) | sin(angulo) | 
            // |----------------------------|
            // | -sin(angulo) | cos(angulo) |
            //  ---------------------------- 

            // Para cada pixel en la vieja imagen, busca cuál sería la posición de ese pixel en la imagen nueva
            int nuevoX = ((x - centroX) *   cos(rad)  + (y - centroY) * sin(rad)) + nuevoCentroX; 
            int nuevoY = ((x - centroX) * (-sin(rad)) + (y - centroY) * cos(rad)) + nuevoCentroY;

            // Si la ubicación en el pixel nuevo cae dentro de los límites, usa el valor de la posicion del original en el nuevo pixel
            if (nuevoX >= 0 && nuevoX < nuevoAncho && nuevoY >= 0 && nuevoY < nuevoAlto) {
                imagenRotada[nuevoY][nuevoX] = imagen[y][x];
            }

        }
    }

    return imagenRotada;
}


/*  Calculamos las dimensiones de la nueva imagen
    Inicializamos los espacios de esa nueva imagen, queda vacía mas no negra
    Calculamos los centros de la vieja y la nueva imagen
    Iterando sobre los píxeles de la nueva imagen haremos el proceso inverso:
        Le restamos el nuevo centro
        Le aplicamos la matriz simplificada de rotación pero en dirección opuesta
        Le sumamos el viejo centro
        Obtenemos las coordenadas correspondientes al viejo pixel
        Si está en los límites de la vieja imagen, lo pintamos
        Si no está en los límites (pasa con los bordes), lo pintamos de negro

    Cada pixel de la nueva imagen es pintado por uno de la vieja. Pero a dos o más píxeles
    de la nueva les puede corresponder ese mismo pixel de la vieja.
    De esta forma no habrán puntos negros en la imagen, los espacios se llenarán
    con píxeles repetidos.
*/
vector<vector<Pixel>> rotarImagenInversa(const vector<vector<Pixel>>& imagen, double angulo) {
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
    #pragma omp parallel
    for (int y = 0; y < nuevoAlto; y++) {
        for (int x = 0; x < nuevoAncho; x++) {
            
            // Matriz de rotación inversa simplificada (2x2): 
            //  ------------------------------
            // |  cos(-angulo) | sin(-angulo) | 
            // |------------------------------|
            // | -sin(-angulo) | cos(-angulo) |
            //  ------------------------------

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


/*  Calculamos la dimensión horizontal de la nueva imagen (sólo cambia la horizontal)
    Inicializamos esa nueva imagen en negro
    Calculamos los centros de la vieja y la nueva imagen 
    Iterando sobre los píxeles de la vieja imagen:
        Le restamos el viejo centro
        Le aplicamos la matriz simplificada de cizallamiento
        Le sumamos el nuevo centro
        Obtenemos las coordenadas correspondientes al nuevo pixel
        Si está en los límites de la nueva imagen (a veces se sale por culpa del redondeo) lo pintamos

    Cada pixel de la vieja pinta uno en la nueva. A cada pixel de la nueva le corresponde
    sólo un pixel de la vieja, como en la rotación normal.
    En este caso no queda un patrón de píxeles negros al interior de la imagen, ya que el redondeo
    sólo mueve los píxeles horizontalmente, pero de forma que sigan siendo contínuos/vecinos, no desgarra
    (separar hacia lados opuestos o en diferentes ejes).
*/
vector<vector<Pixel>> cizallarImagen(const vector<vector<Pixel>>& imagen, double angulo) {
    // Convierte el ángulo de grados a radianes
    double rad = angulo * M_PI / 180.0;

    // Calcula el tamaño de la nueva imagen
    int ancho = imagen[0].size();
    int alto = imagen.size();
    int nuevoAncho = ancho + abs(alto * tan(rad));

    // Crea la nueva imagen
    vector<vector<Pixel>> imagenCizallada(alto, vector<Pixel>(nuevoAncho, {0,0,0}));

    // Calcula el centro de la imagen original y la nueva imagen
    double centroX = ancho / 2.0;
    double centroY = alto / 2.0;
    double nuevoCentroX = nuevoAncho / 2.0;

    // Itera sobre cada pixel de la antigua imagen
    #pragma omp parallel
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            
            // Matriz de cizallamiento en X simplificada (2x2): 
            //  ---------------------------
            // |      1      | tan(angulo) | 
            // |---------------------------|
            // |      0      |      1      |
            //  ---------------------------

            // Para cada pixel en la vieja imagen, busca cuál sería la posición de ese pixel en la imagen nueva
            // En el cizallamiento, sólo cambia su posición en X
            int nuevoX = ((x - centroX) + (y - centroY) * tan(rad)) + nuevoCentroX; 
            
            // Si la ubicación en el pixel nuevo cae dentro de los límites, usa el valor de la posicion del original en el nuevo pixel
            if (nuevoX >= 0 && nuevoX < nuevoAncho && y >= 0 && y < alto) {
                imagenCizallada[y][nuevoX] = imagen[y][x];
            }
        }
    }

    return imagenCizallada;
}



int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Uso: " << argv[0] << " <nombre_del_archivo_entrada.bmp> <angulo (0 - 359)>" << endl;
        return 1;
    }

    const char* nombreArchivo = argv[1];
    int angulo = atoi(argv[2]);
    const vector<vector<Pixel>> imagen = leerArchivoBMP(nombreArchivo);

    while(true){
        if(angulo >= 0){
            break;
        }else{
            angulo = angulo + 360;
        }
    }

    int opcion;
    cout << "1. -Rotar la imagen (Horariamente)\n   -Rotar la imagen pero haciendo el procedimiento a la inversa (iterando en la nueva)\n2. Cizallar la imagen en x (Arriba hacia la derecha)\n3. Hacer ambas cosas\n" ;
    cin >> opcion;


    if(opcion == 1 || opcion == 3){  // Rotar normal
        cout << "Archivo input: " << nombreArchivo << "\nArchivo output: " << "rotar.bmp - rotarInversa.bmp" << "\nÁngulo: " << angulo << "\n";

        const vector<vector<Pixel>> imagenRotada = rotarImagen(imagen, angulo); 
        guardarMatrizEnBMP("rotar.bmp", imagenRotada);

        const vector<vector<Pixel>> imagenRotadaInversa = rotarImagenInversa(imagen, angulo);
        guardarMatrizEnBMP("rotarInversa.bmp", imagenRotadaInversa);
    }
    if(opcion == 2 || opcion == 3){  // Cizallar
        cout << "Archivo input: " << nombreArchivo << "\nArchivo output: " << "cizallar.bmp" << "\nÁngulo: " << angulo;

        const vector<vector<Pixel>> imagenCizallada = cizallarImagen(imagen, angulo);
        guardarMatrizEnBMP("cizallar.bmp", imagenCizallada);
    }
    if(opcion != 1 && opcion != 2 && opcion != 3){ // Opción inválida
        cout << "Qué le pasa pa";
    }

    return 0;
}