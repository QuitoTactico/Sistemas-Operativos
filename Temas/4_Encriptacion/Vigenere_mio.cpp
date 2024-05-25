#include <iostream>
#include <string>
using namespace std;

void cifrarVigenere(string texto, string clave) {
    int textoLen = texto.length(), claveLen = clave.length();
    string nuevaClave, cifradoTexto;
    
    // Generar nueva clave en caso de que la longitud del texto original y la clave no coincidan
    for(int i = 0, j = 0; i < textoLen; ++i, ++j) {
        if(j == claveLen)
            j = 0;
        nuevaClave += clave[j];
    }
    
    // Cifrado
    for(int i = 0; i < textoLen; ++i) {
        cifradoTexto += ((texto[i] + nuevaClave[i] - 2 * 'A') % 26) + 'A';
    }
    
    cout << "Texto cifrado: " << cifradoTexto << endl;
}

void descifrarVigenere(string cifradoTexto, string clave) {
    int textoLen = cifradoTexto.length(), claveLen = clave.length();
    string nuevaClave, textoOriginal;
    
    // Generar nueva clave en caso de que la longitud del texto cifrado y la clave no coincidan
    for(int i = 0, j = 0; i < textoLen; ++i, ++j) {
        if(j == claveLen)
            j = 0;
        nuevaClave += clave[j];
    }
    
    // Descifrado
    for(int i = 0; i < textoLen; ++i) {
        textoOriginal += ((cifradoTexto[i] - nuevaClave[i] + 26) % 26) + 'A';
    }
    
    cout << "Texto original: " << textoOriginal << endl;
}

int main() {
    string texto = "TEXTOPLANO";
    string clave = "CLAVE";
    
    cifrarVigenere(texto, clave);
    
    string textoCifrado = "UPXVPRWNQP"; // El texto cifrado generado por la función cifrarVigenere
    descifrarVigenere(textoCifrado, clave);
    
    return 0;
}
