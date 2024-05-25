#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>

using namespace std;

using ull = unsigned long long;

ull gcd(ull a, ull b) {
    while (b != 0) {
        ull temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

ull modInverse(ull e, ull phi) {
    ull t = 0, newT = 1;
    ull r = phi, newR = e;
    while (newR != 0) {
        ull quotient = r / newR;
        t = newT;
        newT = t - quotient * newT;
        r = newR;
        newR = r - quotient * newR;
    }
    if (r > 1) return 0;
    if (t < 0) t += phi;
    return t;
}

ull modExp(ull base, ull exp, ull mod) {
    ull result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

void generarClaves(ull& n, ull& e, ull& d) {
    // Primos pequeños para simplificar (en un escenario real, deberían ser mucho mayores)
    ull p = 61, q = 53;
    n = p * q;
    ull phi = (p - 1) * (q - 1);

    // Elegir e (1 < e < phi) que sea coprimo con phi
    e = 17; // e es elegido comúnmente como 3, 17, 65537 por ser números primos y pequeños.
    
    // Calcular d
    d = modInverse(e, phi);
}

void cifrarRSA(const vector<ull>& datos, ull n, ull e, vector<ull>& cifradoDatos) {
    cifradoDatos.resize(datos.size());
    for (size_t i = 0; i < datos.size(); ++i) {
        cifradoDatos[i] = modExp(datos[i], e, n);
    }
}

void descifrarRSA(const vector<ull>& cifradoDatos, ull n, ull d, vector<ull>& datosOriginales) {
    datosOriginales.resize(cifradoDatos.size());
    for (size_t i = 0; i < cifradoDatos.size(); ++i) {
        datosOriginales[i] = modExp(cifradoDatos[i], d, n);
    }
}

void leerArchivo(const string& nombreArchivo, vector<ull>& datos) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (archivo.is_open()) {
        archivo.seekg(0, ios::end);
        size_t tamaño = archivo.tellg();
        archivo.seekg(0, ios::beg);
        datos.resize(tamaño);
        archivo.read(reinterpret_cast<char*>(datos.data()), tamaño);
        archivo.close();
    } else {
        cerr << "No se pudo abrir el archivo " << nombreArchivo << endl;
    }
}

void escribirArchivo(const string& nombreArchivo, const vector<ull>& datos) {
    ofstream archivo(nombreArchivo, ios::binary);
    if (archivo.is_open()) {
        archivo.write(reinterpret_cast<const char*>(datos.data()), datos.size() * sizeof(ull));
        archivo.close();
    } else {
        cerr << "No se pudo escribir el archivo " << nombreArchivo << endl;
    }
}

int main() {
    string nombreArchivoEntrada = "entrada.bin";
    string nombreArchivoCifrado = "cifrado.bin";
    string nombreArchivoDescifrado = "descifrado.bin";

    ull n, e, d;
    generarClaves(n, e, d);

    vector<ull> datos;
    vector<ull> cifradoDatos;
    vector<ull> datosOriginales;

    leerArchivo(nombreArchivoEntrada, datos);
    cifrarRSA(datos, n, e, cifradoDatos);
    escribirArchivo(nombreArchivoCifrado, cifradoDatos);

    leerArchivo(nombreArchivoCifrado, cifradoDatos);
    descifrarRSA(cifradoDatos, n, d, datosOriginales);
    escribirArchivo(nombreArchivoDescifrado, datosOriginales);

    return 0;
}
