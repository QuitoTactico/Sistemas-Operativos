#ifndef RLE_H
#define RLE_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class RLE {
public:
    RLE() {}
    ~RLE() {}

    void compress(const string& inputFileName, const string& outputFileName);
    void decompress(const string& inputFileName, const string& outputFileName);
};

#endif // RLE_H