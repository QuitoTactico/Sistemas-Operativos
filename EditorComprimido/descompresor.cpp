#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

// leemos el archivo comprimido para obtener la secuencia comprimida
std::vector<std::pair<int, char>> leer_comprimido() {
    std::ifstream infile("archivo_comprimido.bin", std::ios::binary);
    std::vector<std::pair<int, char>> compressedBuffer;
    if (infile) {
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

int main() {
    // leemos el archivo comprimido
    std::vector<std::pair<int, char>> compressedBuffer = leer_comprimido();

    // descomprimimos la secuencia comprimida
    std::string descomprimido = descomprimir(compressedBuffer);
    
    // nos traen el texto descomprimido
    std::cout << "Texto descomprimido: " << descomprimido << std::endl;

    return 0;
}
