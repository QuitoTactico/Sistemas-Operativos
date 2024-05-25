#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

// Funciones de descompresión
vector<int> leer_comprimido(const string& archivo);
string descomprimir(const vector<int>& comprimido);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <foto_a_crear.png>" << endl;
        return 1;
    }
    const char* nombreFoto = argv[1];

    // Leer y descomprimir el archivo comprimido
    vector<int> comprimido = leer_comprimido("foto_comprimido.neko");
    string descomprimido = descomprimir(comprimido);

    // Escribir el archivo descomprimido a un archivo temporal
    ofstream fileWrite("foto_descomprimido.neko", ios::binary);
    fileWrite.write(descomprimido.c_str(), descomprimido.size());
    fileWrite.close();

    // Reabrir el archivo descomprimido para leer los datos
    ifstream file("foto_descomprimido.neko", ios::binary);

    if (file.is_open()) {
        uint32_t id;
        uint8_t edadSexo;   uint16_t edad;   bool sexo;   string sexoString;
        uint8_t nombreLen;
        string nombre;
        uint8_t abreviacionLen;
        string abreviacion;
        uint16_t descripcionLen;
        string descripcion;

        // Leer los datos del archivo descomprimido
        file.read(reinterpret_cast<char*>(&id), 4);
        file.read(reinterpret_cast<char*>(&edadSexo), 1);
        edad = edadSexo & 0x7F;
        sexo = edadSexo >> 7;
        sexoString = sexo ? "Femenino" : "Masculino";

        file.read(reinterpret_cast<char*>(&nombreLen), 1);
        nombre.resize(nombreLen);
        file.read(&nombre[0], nombreLen);

        file.read(reinterpret_cast<char*>(&abreviacionLen), 1);
        abreviacion.resize(abreviacionLen);
        file.read(&abreviacion[0], abreviacionLen);

        file.read(reinterpret_cast<char*>(&descripcionLen), 2);
        descripcion.resize(descripcionLen);
        file.read(&descripcion[0], descripcionLen);

        // Leer la imagen
        file.seekg(0, ios::end);
        int longitud = file.tellg();
        int datosAnteriores = 4 + 1 + 1 + nombreLen + 1 + abreviacionLen + 2 + descripcionLen;
        int longitudImagen = longitud - datosAnteriores;
        file.seekg(datosAnteriores, ios::beg);
        char* buffer = new char[longitudImagen];
        file.read(buffer, longitudImagen);
        file.close();

        // Escribir la imagen a un archivo
        ofstream foto(nombreFoto, ios::binary);
        if (!foto) {
            cerr << "No se pudo abrir el archivo " << nombreFoto << endl;
            delete[] buffer;
            return 1;
        } else {
            foto.write(buffer, longitudImagen);
            foto.close();
            delete[] buffer;
        }

        // Imprimir los datos
        cout << "ID: " << id << endl;
        cout << "Edad: " << edad << endl;
        cout << "Sexo: " << sexoString << endl;
        cout << "Nombre: " << nombre << endl;
        cout << "Abreviación: " << abreviacion << endl;
        cout << "Descripción: " << descripcion << endl;

        cerr << "\nLa foto recibida de [foto_comprimido.neko] se ha almacenado en [" << nombreFoto << "]" << endl;
    } else {
        cerr << "No se pudo abrir el archivo descomprimido [foto_descomprimido.neko]." << endl;
    }

    return 0;
}

// Funciones de descompresión
vector<int> leer_comprimido(const string& archivo) {
    vector<int> comprimido;
    ifstream input(archivo, ios::binary);
    char bytes[2];
    while (input.read(bytes, 2)) {
        int numero = (unsigned char)(bytes[0]) << 8 | (unsigned char)(bytes[1]);
        comprimido.push_back(numero);
    }
    input.close();
    return comprimido;
}

string descomprimir(const vector<int>& comprimido) {
    unordered_map<int, string> dictionary;
    for (int i = 0; i < 256; ++i) {
        dictionary[i] = string(1, char(i));
    }

    string descomprimido, w;
    int dict_size = 256;

    if (!comprimido.empty()) {
        w = dictionary[comprimido[0]];
        descomprimido = w;
    }

    for (size_t i = 1; i < comprimido.size(); ++i) {
        string entry;
        if (dictionary.count(comprimido[i])) {
            entry = dictionary[comprimido[i]];
        } else if (comprimido[i] == dict_size) {
            entry = w + w[0];
        } else {
            throw runtime_error("Error en la descompresión: índice fuera de rango.");
        }

        descomprimido += entry;
        dictionary[dict_size++] = w + entry[0];
        w = entry;
    }

    return descomprimido;
}
