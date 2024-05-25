// sudo apt-get install libcrypto++-dev
// g++ RSA_lib.cpp -o RSA_lib -lcryptopp

#include <iostream>
#include <fstream>
#include <vector>
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>
#include <cryptopp/filters.h>

using namespace std;
using namespace CryptoPP;

void generarClaves(RSA::PublicKey& publicKey, RSA::PrivateKey& privateKey) {
    AutoSeededRandomPool rng;

    privateKey.GenerateRandomWithKeySize(rng, 2048);
    RSAFunction params(privateKey);
    publicKey.Initialize(params);
}

void cifrarRSA(const vector<byte>& datos, const RSA::PublicKey& publicKey, vector<byte>& cifradoDatos) {
    AutoSeededRandomPool rng;

    RSAES_OAEP_SHA_Encryptor encryptor(publicKey);

    StringSource ss1(
        datos.data(), datos.size(), true,
        new PK_EncryptorFilter(rng, encryptor,
            new VectorSink(cifradoDatos)
        )
    );
}

void descifrarRSA(const vector<byte>& cifradoDatos, const RSA::PrivateKey& privateKey, vector<byte>& datosOriginales) {
    AutoSeededRandomPool rng;

    RSAES_OAEP_SHA_Decryptor decryptor(privateKey);

    StringSource ss2(
        cifradoDatos.data(), cifradoDatos.size(), true,
        new PK_DecryptorFilter(rng, decryptor,
            new VectorSink(datosOriginales)
        )
    );
}

void leerArchivo(const string& nombreArchivo, vector<byte>& datos) {
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

void escribirArchivo(const string& nombreArchivo, const vector<byte>& datos) {
    ofstream archivo(nombreArchivo, ios::binary);
    if (archivo.is_open()) {
        archivo.write(reinterpret_cast<const char*>(datos.data()), datos.size());
        archivo.close();
    } else {
        cerr << "No se pudo escribir el archivo " << nombreArchivo << endl;
    }
}

int main() {
    string nombreArchivoEntrada = "entrada.bin";
    string nombreArchivoCifrado = "cifrado.bin";
    string nombreArchivoDescifrado = "descifrado.bin";

    RSA::PublicKey publicKey;
    RSA::PrivateKey privateKey;

    generarClaves(publicKey, privateKey);

    vector<byte> datos;
    vector<byte> cifradoDatos;
    vector<byte> datosOriginales;

    leerArchivo(nombreArchivoEntrada, datos);
    cifrarRSA(datos, publicKey, cifradoDatos);
    escribirArchivo(nombreArchivoCifrado, cifradoDatos);

    leerArchivo(nombreArchivoCifrado, cifradoDatos);
    descifrarRSA(cifradoDatos, privateKey, datosOriginales);
    escribirArchivo(nombreArchivoDescifrado, datosOriginales);

    return 0;
}
