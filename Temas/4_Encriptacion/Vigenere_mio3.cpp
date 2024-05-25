#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

void cifrarVigenere(const vector<char>& datos, const string& clave, vector<char>& cifradoDatos) {
    int datosLen = datos.size(), claveLen = clave.length();
    cifradoDatos.resize(datosLen);

    // Cifrado
    for(int i = 0; i < datosLen; ++i) {
        char caracterDato = datos[i];
        char caracterClave = clave[i % claveLen];
        cifradoDatos[i] = (caracterDato + caracterClave) % 256;  // Modulo 256 para valores binarios
    }
}

void descifrarVigenere(const vector<char>& cifradoDatos, const string& clave, vector<char>& datosOriginales) {
    int datosLen = cifradoDatos.size(), claveLen = clave.length();
    datosOriginales.resize(datosLen);

    // Descifrado
    for(int i = 0; i < datosLen; ++i) {
        char caracterCifrado = cifradoDatos[i];
        char caracterClave = clave[i % claveLen];
        datosOriginales[i] = (caracterCifrado - caracterClave + 256) % 256;  // Modulo 256 para valores binarios
    }
}

void leerArchivo(const string& nombreArchivo, vector<char>& datos) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (archivo.is_open()) {
        archivo.seekg(0, ios::end);
        size_t tamaño = archivo.tellg();
        archivo.seekg(0, ios::beg);
        datos.resize(tamaño);
        archivo.read(datos.data(), tamaño);
        archivo.close();
    } else {
        cerr << "No se pudo abrir el archivo " << nombreArchivo << endl;
    }
}

void escribirArchivo(const string& nombreArchivo, const vector<char>& datos) {
    ofstream archivo(nombreArchivo, ios::binary);
    if (archivo.is_open()) {
        archivo.write(datos.data(), datos.size());
        archivo.close();
    } else {
        cerr << "No se pudo escribir el archivo " << nombreArchivo << endl;
    }
}

int main() {
    string nombreArchivoEntrada = "entrada.bin";
    string nombreArchivoCifrado = "cifrado.bin";
    string nombreArchivoDescifrado = "descifrado.bin";
    string clave = "CLAVE";

    vector<char> datos;
    vector<char> cifradoDatos;
    vector<char> datosOriginales;

    leerArchivo(nombreArchivoEntrada, datos);
    cifrarVigenere(datos, clave, cifradoDatos);
    escribirArchivo(nombreArchivoCifrado, cifradoDatos);

    leerArchivo(nombreArchivoCifrado, cifradoDatos);
    descifrarVigenere(cifradoDatos, clave, datosOriginales);
    escribirArchivo(nombreArchivoDescifrado, datosOriginales);

    return 0;
}
