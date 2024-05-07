#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

// función de compresión usando LZ78
std::vector<std::pair<int, char>> comprimir(const std::string& buffer) {
    std::vector<std::pair<int, char>> compressed;
    std::unordered_map<std::string, int> dictionary;
    int dictSize = 256;

    std::string w;
    for (char c : buffer) {
        std::string wc = w + c;
        if (dictionary.find(wc) != dictionary.end()) {
            w = wc;
        } else {
            if (!w.empty()) {
                compressed.push_back(std::make_pair(dictionary[w], c));
            }
            dictionary[wc] = dictSize++;
            w = std::string(1, c);
        }
    }

    if (!w.empty()) {
        compressed.push_back(std::make_pair(dictionary[w], '\0'));
    }

    return compressed;
}

// función para guardar el texto comprimido en un archivo binario
void guardar_comprimido(const std::string& buffer) {
    // llamamos a la función de compresión
    std::vector<std::pair<int, char>> compressedBuffer = comprimir(buffer);

    std::ofstream outfile("archivo_comprimido.bin", std::ios::binary);
    if (outfile) {
        for (const auto& entry : compressedBuffer) {
            outfile.write(reinterpret_cast<const char*>(&entry.first), sizeof(int));
            outfile.write(&entry.second, sizeof(char));
        }
        outfile.close();
        std::cout << "Archivo guardado de forma comprimida." << std::endl;
    } else {
        std::cerr << "Error al abrir el archivo para escritura." << std::endl;
    }
}

int main() {
    std::string buffer = "Contenido del buffer a comprimir";

    // llamamos a la función para guardar el texto comprimido
    guardar_comprimido(buffer);

    return 0;
}
