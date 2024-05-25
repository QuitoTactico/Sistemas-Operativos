#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

// leemos el archivo comprimido para obtener la secuencia comprimida
std::vector<std::pair<int, char>> leer_comprimido(const std::string& archivo) {
    std::ifstream infile(archivo, std::ios::binary);
    std::vector<std::pair<int, char>> compressedBuffer;
    if (infile.is_open()) {
        int index;
        char character;
        while (infile.read(reinterpret_cast<char*>(&index), sizeof(int)) && infile.read(&character, sizeof(char))) {
            compressedBuffer.push_back(std::make_pair(index, character));
        }
        infile.close();
    } else {
        std::cerr << "Error al abrir el archivo para lectura." << std::endl;
    }
    return compressedBuffer;
}

// función para descomprimir la secuencia comprimida
std::string descomprimir(const std::vector<std::pair<int, char>>& compressed) {
    std::unordered_map<int, std::string> dictionary;
    int dictSize = 256;

    // Inicializamos el diccionario con caracteres ASCII extendidos
    for (int i = 0; i < 256; ++i) {
        dictionary[i] = std::string(1, static_cast<char>(i));
    }

    std::string descomprimido;
    for (const auto& entry : compressed) {
        int index = entry.first;
        char character = entry.second;

        std::string str;
        if (dictionary.find(index) != dictionary.end()) {
            str = dictionary[index];
        } else if (index == dictSize) {
            str = dictionary[index - 1] + character;
        } else {
            throw std::runtime_error("Error en la descompresión: índice fuera de rango.");
        }

        // Construimos la cadena descomprimida
        descomprimido += str;

        // Añadimos al diccionario la nueva cadena
        dictionary[dictSize++] = dictionary[index] + str[0];
    }

    return descomprimido;
}

int main() {
    // leemos el archivo comprimido
    std::vector<std::pair<int, char>> compressedBuffer = leer_comprimido("miau.bin");

    // descomprimimos la secuencia comprimida
    std::string descomprimido = descomprimir(compressedBuffer);
    
    // nos traen el texto descomprimido
    std::cout << "Texto descomprimido: " << descomprimido << std::endl;

    return 0;
}
