#include <iostream>
#include <string>
using namespace std;

void cifrarVigenere(const string& texto, const string& clave) {
    int textoLen = texto.length(), claveLen = clave.length();
    string cifradoTexto;

    // Cifrado
    for(int i = 0; i < textoLen; ++i) {
        char caracterTexto = texto[i];
        char caracterClave = clave[i % claveLen];
        cifradoTexto += ((caracterTexto + caracterClave - 2 * 'A') % 26) + 'A';
    }

    cout << "Texto cifrado: " << cifradoTexto << endl;
}

void descifrarVigenere(const string& cifradoTexto, const string& clave) {
    int textoLen = cifradoTexto.length(), claveLen = clave.length();
    string textoOriginal;

    // Descifrado
    for(int i = 0; i < textoLen; ++i) {
        char caracterCifrado = cifradoTexto[i];
        char caracterClave = clave[i % claveLen];
        textoOriginal += ((caracterCifrado - caracterClave + 26) % 26) + 'A';
    }

    cout << "Texto original: " << textoOriginal << endl;
}

int main() {
    string texto = "TEXTOPLANO";
    string clave = "CLAVE";
    
    cifrarVigenere(texto, clave);
    
    // Asumiendo que "UPXVPRWNQP" es el texto cifrado generado por cifrarVigenere para este ejemplo
    string textoCifrado = "UPXVPRWNQP";
    descifrarVigenere(textoCifrado, clave);
    
    return 0;
}
