#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>

std::vector<int> comprimir(const std::string& texto) {
    std::unordered_map<std::string, int> dictionary;
    for (int i = 0; i < 256; ++i) {
        dictionary[std::string(1, char(i))] = i;
    }

    std::string w;
    std::vector<int> comprimido;
    int dict_size = 256;

    for (char c : texto) {
        std::string wc = w + c;
        if (dictionary.count(wc)) {
            w = wc;
        } else {
            comprimido.push_back(dictionary[w]);
            dictionary[wc] = dict_size++;
            w = std::string(1, c);
        }
    }

    if (!w.empty()) {
        comprimido.push_back(dictionary[w]);
    }

    return comprimido;
}

void guardar_comprimido(const std::vector<int>& comprimido, const std::string& archivo) {
    std::ofstream output(archivo, std::ios::binary);
    for (int numero : comprimido) {
        output.put(char((numero >> 8) & 0xFF));
        output.put(char(numero & 0xFF));
    }
    output.close();
    std::cout << "Archivo guardado de forma comprimida." << std::endl;
}

int main() {
    std::string texto = "Eclesiastes 33 de eclesiastes digo yo, el yo que soy, el yo que fui, el yo que seré.";
    auto comprimido = comprimir(texto);
    guardar_comprimido(comprimido, "miau.bin");
    return 0;
}
