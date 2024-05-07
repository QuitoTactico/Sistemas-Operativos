#ifndef LZ77_H
#define LZ77_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class LZ77 {
public:
    LZ77() {}
    ~LZ77() {}

    void compress(const string& inputFileName, const string& outputFileName);
    void decompress(const string& inputFileName, const string& outputFileName);

private:
    struct Token {
        int distance;
        int length;
        char nextChar;
    };

    void writeToken(Token token, ofstream& outputFile);
    Token readToken(ifstream& inputFile);

    vector<Token> search(const string& input, int start, int windowSize, int lookaheadBuffer);
};

#endif // LZ77_H
