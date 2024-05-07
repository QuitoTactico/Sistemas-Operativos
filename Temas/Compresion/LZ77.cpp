#include "LZ77.h"

void LZ77::compress(const string& inputFileName, const string& outputFileName) {
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

    // Leer todo el contenido del archivo de entrada
    string inputContent((istreambuf_iterator<char>(inputFile)), (istreambuf_iterator<char>()));

    const int windowSize = 4096; // Tamaño de la ventana deslizante
    const int lookaheadBuffer = 15; // Tamaño del buffer de búsqueda adelantada

    int inputIndex = 0;
    while (static_cast<size_t>(inputIndex) < inputContent.size()) {
        vector<Token> tokens = search(inputContent, inputIndex, windowSize, lookaheadBuffer);

        for (const auto& token : tokens) {
            // Escribir el token en el archivo de salida
            writeToken(token, outputFile);
        }
    }

    inputFile.close();
    outputFile.close();
    cout << "Archivo (" << inputFileName << ") comprimido correctamente. Archivo creado: " << outputFileName << endl;
}

void LZ77::writeToken(Token token, ofstream& outputFile) {
    // Escribir el token en el archivo de salida
    outputFile.write(reinterpret_cast<const char*>(&token), sizeof(Token));
}

void LZ77::decompress(const string& inputFileName, const string& outputFileName) {
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

    Token token;
    while (true) {
        token = readToken(inputFile);
        if (inputFile.eof()) break;

        if (token.length == 0) {
            outputFile.put(token.nextChar);
        } else {
            // Mover el puntero de lectura a la posición inicial de la secuencia a copiar
            int startPos = outputFile.tellp();
            int startPosBuffer = startPos - token.distance;
            inputFile.seekg(startPosBuffer);

            // Leer y escribir los bytes de la secuencia a copiar
            for (int i = 0; i < token.length; ++i) {
                char c;
                inputFile.get(c);
                outputFile.put(c);
            }
            outputFile.put(token.nextChar);
        }
    }

    inputFile.close();
    outputFile.close();
    cout << "Archivo (" << inputFileName << ") descomprimido correctamente. Archivo creado: " << outputFileName << endl;
}

LZ77::Token LZ77::readToken(ifstream& inputFile) {
    Token token;
    inputFile.read(reinterpret_cast<char*>(&token), sizeof(Token));
    return token;
}

vector<LZ77::Token> LZ77::search(const string& input, int start, int windowSize, int lookaheadBuffer) {
    vector<Token> tokens;
    int end = min(start + lookaheadBuffer, static_cast<int>(input.size()));

    while (start < end) {
        int bestMatchDistance = -1;
        int bestMatchLength = -1;
        char bestMatchChar = input[start + bestMatchLength];

        for (int i = max(0, start - windowSize); i < start; ++i) {
            int j = start;
            int length = 0;
            while (j < end && input[i + length] == input[j]) {
                ++j;
                ++length;
            }

            if (length > bestMatchLength) {
                bestMatchDistance = start - i;
                bestMatchLength = length;
                bestMatchChar = input[j];
            }
        }

        tokens.push_back({bestMatchDistance, bestMatchLength, bestMatchChar});
        start += (bestMatchLength + 1);
    }

    return tokens;
}
