#ifndef LZW_H
#define LZW_H

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <iomanip>

class LZW {
private:
    std::unordered_map<std::string, int> dictionary;
    const int MAX_DICT_SIZE = 4096;

public:
    LZW();

    void comprimir(const std::string &inputFile, const std::string &outputFile);
    void descomprimir(const std::string &inputFile, const std::string &outputFile);
    void mostrarDiccionario();

private:
    void inicializarDiccionario();
    void escribirCodigo(int code, std::ofstream &fout);
};


#endif
