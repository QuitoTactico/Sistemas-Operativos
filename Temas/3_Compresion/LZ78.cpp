#include "LZ78.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <bitset>
#include <cstdint>

void LZ78::comprimir(const std::string& archivo_fuente, const std::string& archivo_comprimido) {
    std::ifstream entrada(archivo_fuente, std::ios::binary);
    std::ofstream salida(archivo_comprimido, std::ios::binary);

    if (!entrada.is_open() || !salida.is_open()) {
        std::cerr << "Error al abrir archivos." << std::endl;
        return;
    }

    std::unordered_map<std::string, int> dictionary;
    int nextCode = 1;
    std::string buffer;
    char c;
    while (entrada.get(c)) {
        buffer += c;
        if (dictionary.find(buffer) == dictionary.end()) {
            int code = dictionary[buffer.substr(0, buffer.size() - 1)];
            std::bitset<16> codeBits(code);
            uint16_t codeBytes = codeBits.to_ulong();
            salida.write(reinterpret_cast<const char*>(&codeBytes), sizeof(codeBytes));
            dictionary[buffer] = nextCode++;
            buffer.clear();
        }
    }

    if (!buffer.empty()) {
        int code = dictionary[buffer];
        std::bitset<16> codeBits(code);
        uint16_t codeBytes = codeBits.to_ulong();
        salida.write(reinterpret_cast<const char*>(&codeBytes), sizeof(codeBytes));
    }

    entrada.close();
    salida.close();
    std::cout << "Comprimido LZ78" << std::endl;
}

void LZ78::descomprimir(const std::string& archivo_comprimido, const std::string& archivo_descomprimido) {
    std::ifstream entrada(archivo_comprimido, std::ios::binary);
    std::ofstream salida(archivo_descomprimido, std::ios::binary);

    if (!entrada.is_open() || !salida.is_open()) {
        std::cerr << "Error al abrir archivos." << std::endl;
        return;
    }

    std::unordered_map<int, std::string> dictionary;
    int nextCode = 1;

    uint16_t code;
    std::string buffer;
    while (entrada.read(reinterpret_cast<char*>(&code), sizeof(code))) {
        if (code == 0) { // Manejar el caso especial para el primer caracter
            entrada.read(reinterpret_cast<char*>(&code), sizeof(code));
            buffer = static_cast<char>(code);
            salida << buffer;
        } else {
            buffer = dictionary[code - 1];
            salida << buffer;
            entrada.read(reinterpret_cast<char*>(&code), sizeof(code));
            buffer = static_cast<char>(code);
            salida << buffer;
        }
        dictionary[nextCode++] = buffer;
    }

    entrada.close();
    salida.close();
    std::cout << "Descomprimido LZ78" << std::endl;
}
