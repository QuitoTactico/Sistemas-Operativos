#include <iostream>
#include <string>

#include "RLE.h"
#include "LZ77.h"
#include "LZW.h"
#include "LZ78.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 5) {
        cout << "Uso: " << argv[0] << " <modo (C/D)> <algoritmo (RLE|LZ77|LZ78|Huffman|LZW)> <archivo_entrada> <archivo_salida>" << endl;
        return 1;
    }

    string modo = argv[1];
    string algorithm = argv[2];
    string inputFileName = argv[3];
    string outputFileName = argv[4];

    if (algorithm == "RLE" && modo == "C") {
        RLE rle = RLE();;
        rle.compress(inputFileName, outputFileName);
    } else if (algorithm == "RLE" && modo == "D") {
        RLE rle = RLE();;
        rle.decompress(inputFileName, outputFileName);   
    } 

    else if (algorithm == "LZ78" && modo == "C") {
        LZ78 lz78 = LZ78();
        lz78.comprimir(inputFileName, outputFileName);
    } else if (algorithm == "LZ78" && modo == "D") {
        LZ78 lz78 = LZ78();;
        lz78.descomprimir(inputFileName, outputFileName);
    } 
    
    /*
    else if (algorithm == "LZ77" && modo == "C") {
        LZ77 lz77 = LZ77();
        lz77.compress(inputFileName, outputFileName);
    } else if (algorithm == "LZ77" && modo == "D") {
        LZ77 lz77 = LZ77();;
        lz77.decompress(inputFileName, outputFileName);
    } 
    */

    /*
    else if (algorithm == "Huffman" && modo == "C") {
        std::cout << "Se llama HuffmanCompressor("<<inputFileName<<", "<<outputFileName<< ")" << std::endl;
    } else if (algorithm == "Huffman" && modo == "D") {
        std::cout << "Se llama HuffmanDecompressor("<<inputFileName<<", "<<outputFileName<< ")" << std::endl;
    } 
    */
    
    else if (algorithm == "LZW" && modo == "C") {
        LZW lzw = LZW();
        lzw.comprimir(inputFileName, outputFileName);
    } else if (algorithm == "LZW" && modo == "D") {
        LZW lzw = LZW();
        lzw.descomprimir(inputFileName, outputFileName);
    }

    return 0;
}
