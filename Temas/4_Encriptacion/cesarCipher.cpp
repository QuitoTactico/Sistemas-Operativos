#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

class CesarCipher {
private:
    int shift; // Desplazamiento de cifrado/descifrado

public:
    CesarCipher(int shift) : shift(shift) {}

    string processText(const string& text, bool encrypt) {
        string processedText;
        for (char c : text) {
            if (isalpha(c)) {
                bool isUpperCase = isupper(c);
                c = toupper(c); // Convertir a mayúscula para el tratamiento

                int newChar = (c + shift) % 26;
                if (isUpperCase) {
                    newChar += 'A';
                } else {
                    newChar += 'a';
                }

                if (islower(c)) {
                    newChar = tolower(newChar);
                }

                processedText += (char)newChar;
            } else if (isdigit(c)) {
                int newDigit = (c - '0' + shift) % 10;
                processedText += (char)(newDigit + '0');
            } else {
                processedText += c;
            }
        }
        return processedText;
    }

    string encrypt(const string& text) {
        return processText(text, true);
    }

    string decrypt(const string& text) {
        return processText(text, false);
    }
};

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Uso: cesar_cipher [encriptar/desencriptar] <archivo_entrada> <archivo_salida>" << endl;
        return 1;
    }

    string operation = argv[1];
    string inputFilePath = argv[2];
    string outputFilePath = argv[3];

    bool encrypt = operation == "e";
    if (!encrypt && operation != "d") {
        cerr << "Operación no válida: " << operation << endl;
        return 1;
    }

    int shift;
    cout << "Ingrese el desplazamiento (positivo para cifrar, negativo para descifrar): ";
    cin >> shift;

    CesarCipher cipher(shift);

    // Abrir el archivo de entrada en modo binario
    ifstream inputFile(inputFilePath, ios::binary);
    if (!inputFile.is_open()) {
        cerr << "Error al abrir el archivo de entrada: " << inputFilePath << endl;
        return 1;
    }

    // Obtener el tamaño del archivo (opcional, para mejorar la eficiencia)
    inputFile.seekg(0, ios::end);
    streamsize fileSize = inputFile.tellg();
    inputFile.seekg(0, ios::beg);

    // Crear un buffer para leer el archivo de entrada
    vector<char> buffer(fileSize);
    inputFile.read(buffer.data(), fileSize);
    inputFile.close();

    // Procesar el texto del buffer (cifrar o descifrar)
    string text(buffer.begin(), buffer.end());
    string processedText = encrypt ? cipher.encrypt(text) : cipher.decrypt(text);

    // Abrir el archivo de salida en modo binario para preservar formato
    ofstream outputFile(outputFilePath, ios::binary);
    if (!outputFile.is_open()) {
        cerr << "Error al abrir el archivo de salida: " << outputFilePath << endl;
        return 1;
    }

    // Escribir el texto procesado en el archivo de salida
    outputFile.write(processedText.data(), processedText.size());
    outputFile.close();

    cout << "Archivo " << (encrypt ? "cifrado" : "descifrado") << " con éxito: " << outputFilePath << endl;

    return 0;
}
