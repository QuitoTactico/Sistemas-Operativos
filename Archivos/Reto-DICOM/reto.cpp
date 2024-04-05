//#include "ioBMPExport.cpp"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

//L_UINT16
#define L_UINT16 unsigned short


#define GETGROUP(nTag)           ((L_UINT16)(nTag >> 16)) 
#define GETELEMENT(nTag)         ((L_UINT16)(nTag & 0xFFFF))


vector<char> leerHeader(const char* nombreArchivo) {
    ifstream archivo(nombreArchivo, ios::binary);

    if (!archivo) {
        cerr << "No se pudo abrir el archivo DICOM" << endl;
        exit(1);
    }

    // Leer los primeros 128 bytes del archivo en un vector
    vector<char> header(1000);
    
    // comienzo a leer desde el byte 128
    archivo.seekg(128, ios::beg);
    archivo.read(header.data(), header.size());
    /*
    vector<char*> header;
    archivo.read(reinterpret_cast<char*>(&header), 128);

    int alto = header[0x28];
    int ancho = header[0x28 + 0x2];
    int dataOffSet = header[0x28 + 0x4];
    */
    archivo.close();
    return header;
}




vector<vector<int>> leerDicom(const char* nombreArchivo) {
    ifstream archivo(nombreArchivo, ios::binary);

    if (!archivo) {
        cerr << "No se pudo abrir el archivo DICOM" << endl;
        exit(1);
    }

    vector<char> header = leerHeader(nombreArchivo);
    int alto = header[0];
    int ancho = header[1];
    int dataOffSet = header[2];

    // Mover el puntero al inicio de los datos de píxeles
    archivo.seekg(dataOffSet, ios::beg);

    // inicializo la matriz
    vector<vector<int>> matriz(alto, vector<int>(ancho));

    for (int i = 0; i < alto; ++i) {
        for (int j = 0; j < ancho; ++j) {
            archivo.read(reinterpret_cast<char*>(&matriz[i][j]), sizeof(float));
        }
        archivo.seekg(ancho % 4, ios::cur);
    }
    archivo.close();
    return matriz;
}

void escribirEnArchivo(const vector<char>& header) {
    ofstream archivo("reto.txt");
    if (!archivo) {
        cerr << "No se pudo abrir el archivo reto.txt" << endl;
        exit(1);
    }

    for (int i = 0; i < header.size(); i++) {
        archivo << header[i] << endl;
        cout << header[i] << endl;
    }

    cout << GETELEMENT(10);

    archivo.close();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <nombre_del_archivo_entrada.bmp>" << endl;
        return 1;
    }
    const char* nombreArchivo = argv[1];

    vector<char> header = leerHeader(nombreArchivo);
    escribirEnArchivo(header);

    return 0;
}