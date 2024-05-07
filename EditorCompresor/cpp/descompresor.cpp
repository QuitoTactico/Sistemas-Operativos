#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>

std::vector<int> leer_comprimido(const std::string& archivo) {
    std::vector<int> comprimido;
    std::ifstream input(archivo, std::ios::binary);
    char bytes[2];
    while (input.read(bytes, 2)) {
        int numero = (unsigned char)(bytes[0]) << 8 | (unsigned char)(bytes[1]);
        comprimido.push_back(numero);
    }
    input.close();
    return comprimido;
}

std::string descomprimir(const std::vector<int>& comprimido) {
    std::unordered_map<int, std::string> dictionary;
    for (int i = 0; i < 256; ++i) {
        dictionary[i] = std::string(1, char(i));
    }

    std::string descomprimido, w;
    int dict_size = 256;

    if (!comprimido.empty()) {
        w = dictionary[comprimido[0]];
        descomprimido = w;
    }

    for (size_t i = 1; i < comprimido.size(); ++i) {
        std::string entry;
        if (dictionary.count(comprimido[i])) {
            entry = dictionary[comprimido[i]];
        } else if (comprimido[i] == dict_size) {
            entry = w + w[0];
        } else {
            throw std::runtime_error("Error en la descompresión: índice fuera de rango.");
        }

        descomprimido += entry;
        dictionary[dict_size++] = w + entry[0];
        w = entry;
    }

    return descomprimido;
}

int main() {
    auto comprimido = leer_comprimido("miau.bin");
    std::string descomprimido = descomprimir(comprimido);
    std::cout << "Texto descomprimido: " << descomprimido << std::endl;
    return 0;
}
