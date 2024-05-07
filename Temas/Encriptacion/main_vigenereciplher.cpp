// main.cpp
#include "VigenereCipher.h"
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Uso: " << argv[0] << " <mode> <key> <Archivo IN> <Archivo out>\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string key = argv[2];
    std::string inputFile = argv[3];
    std::string outputFile = argv[4];

    std::ifstream input(inputFile);
    if (!input) {
        std::cerr << "Error: No se puede abrir el archivo de entrada.\n";
        return 1;
    }

    std::ofstream output(outputFile);
    if (!output) {
        std::cerr << "Error: No se puede abrir el archivo de salida.\n";
        return 1;
    }

    VigenereCipher cipher(key);
    std::string line;
    if (mode == "e") { //encriptar
        while (getline(input, line)) {
            output << cipher.encrypt(line) << '\n';
        }
    } else if (mode == "d") { //desencriptar
        while (getline(input, line)) {
            output << cipher.decrypt(line) << '\n';
        }
    } else {
        std::cerr << "Error: Modo inválido. Use '(e)ncriptar' o '(d)esencriptar''.\n";
        return 1;
    }

    input.close();
    output.close();
    return 0;
}
