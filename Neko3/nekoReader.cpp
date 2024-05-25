#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

using namespace std;

// Funciones de descompresión
vector<int> leer_comprimido(const string& archivo);
string descomprimir(const vector<int>& comprimido);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Uso: " << argv[0] << " <archivo_neko.neko> <foto_a_crear.png>" << endl;
        return 1;
    }
    const char* nombreNeko = argv[1];
    const char* nombreFoto = argv[2];

    // Leer y descomprimir el archivo comprimido
    vector<int> comprimido = leer_comprimido(nombreNeko);
    string descomprimido = descomprimir(comprimido);

    // Convertimos la string descomprimida a un stringstream para facilitar la lectura binaria
    stringstream buffer(descomprimido);

    uint32_t id;
    uint8_t edadSexo;   uint16_t edad;   bool sexo;   string sexoString;
    uint8_t nombreLen;
    string nombre;
    uint8_t abreviacionLen;
    string abreviacion;
    uint16_t descripcionLen;
    string descripcion;

    // Leer los datos del archivo descomprimido
    buffer.read(reinterpret_cast<char*>(&id), 4);
    buffer.read(reinterpret_cast<char*>(&edadSexo), 1);
    edad = edadSexo & 0x7F;
    sexo = edadSexo >> 7;
    sexoString = sexo ? "Femenino" : "Masculino";

    buffer.read(reinterpret_cast<char*>(&nombreLen), 1);
    nombre.resize(nombreLen);
    buffer.read(&nombre[0], nombreLen);

    buffer.read(reinterpret_cast<char*>(&abreviacionLen), 1);
    abreviacion.resize(abreviacionLen);
    buffer.read(&abreviacion[0], abreviacionLen);

    buffer.read(reinterpret_cast<char*>(&descripcionLen), 2);
    descripcion.resize(descripcionLen);
    buffer.read(&descripcion[0], descripcionLen);

    // Leer la imagen
    buffer.seekg(0, ios::end);
    int longitud = buffer.tellg();
    int datosAnteriores = 4 + 1 + 1 + nombreLen + 1 + abreviacionLen + 2 + descripcionLen;
    int longitudImagen = longitud - datosAnteriores;
    buffer.seekg(datosAnteriores, ios::beg);
    char* imgBuffer = new char[longitudImagen];
    buffer.read(imgBuffer, longitudImagen);

    // Escribir la imagen a un archivo
    ofstream foto(nombreFoto, ios::binary);
    if (!foto) {
        cerr << "No se pudo abrir el archivo " << nombreFoto << endl;
        delete[] imgBuffer;
        return 1;
    } else {
        foto.write(imgBuffer, longitudImagen);
        foto.close();
        delete[] imgBuffer;
    }

    // Imprimir los datos
    cout << "ID: " << id << endl;
    cout << "Edad: " << edad << endl;
    cout << "Sexo: " << sexoString << endl;
    cout << "Nombre: " << nombre << endl;
    cout << "Abreviación: " << abreviacion << endl;
    cout << "Descripción: " << descripcion << endl;

    cerr << "\nLa foto recibida de [" << nombreNeko << "] se ha almacenado en [" << nombreFoto << "]" << endl;

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
