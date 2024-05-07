#include "LZW.h"

LZW::LZW() {
    inicializarDiccionario();
}

void LZW::inicializarDiccionario() {
    // Inicializar el diccionario con los caracteres ASCII
    for (int i = 0; i < 256; ++i) {
        std::string ch(1, char(i));
        dictionary[ch] = i;
    }
}

void LZW::comprimir(const std::string &inputFile, const std::string &outputFile) {
    std::ifstream fin(inputFile, std::ios::binary);
    std::ofstream fout(outputFile, std::ios::binary);

    if (!fin.is_open() || !fout.is_open()) {
        std::cerr << "Error al abrir los archivos." << std::endl;
        return;
    }

    std::string current;
    char ch;
    int i=0;
    while (fin.get(ch)) {

        //---------------
        std::string temp = current + ch;
        std::cout << "Caracter Archivo: ("<<i<<") " << temp << std::endl;
        ++i;
        
        if (dictionary.find(temp) != dictionary.end()) {
            current = temp;
        } else {
            escribirCodigo(dictionary[current], fout);
            if (dictionary.size() < static_cast<size_t>(MAX_DICT_SIZE)) {
                dictionary[temp] = static_cast<int>(dictionary.size());
            }
            current = std::string(1, ch);
        }
    }

    if (!current.empty()) {
        escribirCodigo(dictionary[current], fout);
    }

    fin.close();
    fout.close();
    std::cout << "Archivo comprimido con éxito!" << std::endl;
}

void LZW::descomprimir(const std::string &inputFile, const std::string &outputFile) {
    std::ifstream fin(inputFile, std::ios::binary);
    std::ofstream fout(outputFile, std::ios::binary);

    if (!fin.is_open() || !fout.is_open()) {
        std::cerr << "Error al abrir los archivos." << std::endl;
        return;
    }

    std::vector<std::string> dictionaryReverse(static_cast<size_t>(MAX_DICT_SIZE));
    for (auto entry : dictionary) {
        dictionaryReverse[entry.second] = entry.first;
    }

    int code;
    std::string current, previous;
    while (fin.read(reinterpret_cast<char*>(&code), sizeof(code))) {
        std::string entry;
        if (code < static_cast<int>(dictionaryReverse.size())) {
            entry = dictionaryReverse[code];
        } else if (code == static_cast<int>(dictionaryReverse.size())) {
            entry = previous + previous[0];
        } else {
            std::cerr << "Error de descompresión: código no válido." << std::endl;
            return;
        }

        fout << entry;

        if (!previous.empty() && dictionaryReverse.size() < static_cast<size_t>(MAX_DICT_SIZE)) {
            dictionaryReverse.push_back(previous + entry[0]);
        }
        previous = entry;
    }

    fin.close();
    fout.close();
    std::cout << "Archivo descomprimido con éxito!" << std::endl;
}

void LZW::escribirCodigo(int code, std::ofstream &fout) {
    fout.write(reinterpret_cast<const char*>(&code), sizeof(code));
}

void LZW::mostrarDiccionario() {
    std::cout << "Diccionario actual:" << std::endl;
    size_t i = 0;
    for (const auto &entry : dictionary) {
        std::cout <<"["<<std::setw(3)<<std::dec<<i <<"] -> '"<< entry.first << "':{"<< std::hex <<entry.second<<" HEX}"<<std::endl;
        ++i;
    }
}
