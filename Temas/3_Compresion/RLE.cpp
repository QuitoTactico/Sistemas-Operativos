#include <iostream>
#include <fstream>
#include <string>

#include "RLE.h"

using namespace std;


void RLE::compress(const string& inputFileName, const string& outputFileName) {
    ifstream inputFile(inputFileName, ios::binary);
    ofstream outputFile(outputFileName, ios::binary);

    if (!inputFile.is_open()) {
        cerr << "No se puede abrir el archivo de entrada." << endl;
        return;
    }

    if (!outputFile.is_open()) {
        cerr << "No se puede abrir el archivo de salida." << endl;
        return;
    }

    char currentByte, nextByte;
    int count = 0;

    while (inputFile.get(currentByte)) {
        count = 1;
        while (inputFile.peek() == currentByte && count < 255) {
            count++;
            inputFile.get(nextByte);
        }
        outputFile.put(static_cast<char>(count));
        outputFile.put(currentByte);
    }
    inputFile.close();
    outputFile.close();
    std::cout << "Archivo ("<<inputFileName<<") comprimido Ok, el archivo creado es: " << outputFileName << std::endl;
}

void RLE::decompress(const string& inputFileName, const string& outputFileName) {
    ifstream inputFile(inputFileName, ios::binary);
    ofstream outputFile(outputFileName, ios::binary);

    if (!inputFile.is_open()) {
        cerr << "No se puede abrir el archivo de entrada." << endl;
        return;
    }

    if (!outputFile.is_open()) {
        cerr << "No se puede abrir el archivo de salida." << endl;
        return;
    }

    char count, byte;
    while (inputFile.get(count) && inputFile.get(byte)) {
        for (int i = 0; i < static_cast<unsigned char>(count); ++i) {
            outputFile.put(byte);
        }
    }

    inputFile.close();
    outputFile.close();
    std::cout << "Archivo ("<<inputFileName<<") descomprimido Ok, el archivo creado es: " << outputFileName << std::endl;
}