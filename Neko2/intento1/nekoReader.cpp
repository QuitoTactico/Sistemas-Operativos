#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

// Función para leer el archivo comprimido
vector<pair<int, char>> leer_comprimido(const string& archivo) {
    ifstream infile(archivo, ios::binary);
    vector<pair<int, char>> compressedBuffer;
    if (infile.is_open()) {
        int index;
        char character;
        while (infile.read(reinterpret_cast<char*>(&index), sizeof(int)) && infile.read(&character, sizeof(char))) {
            compressedBuffer.push_back(make_pair(index, character));
        }
        infile.close();
    } else {
        cerr << "Error al abrir el archivo para lectura." << endl;
    }
    return compressedBuffer;
}

// Función para descomprimir la secuencia comprimida
string descomprimir(const vector<pair<int, char>>& compressed) {
    unordered_map<int, string> dictionary;
    int dictSize = 256;

    // Inicializamos el diccionario con caracteres ASCII extendidos
    for (int i = 0; i < 256; ++i) {
        dictionary[i] = string(1, static_cast<char>(i));
    }

    string descomprimido;
    string w;
    for (const auto& entry : compressed) {
        int index = entry.first;
        char character = entry.second;

        string str;
        if (dictionary.find(index) != dictionary.end()) {
            str = dictionary[index];
        } else if (index == dictSize) {
            str = w + character;
        } else {
            cerr << "Error en la descompresión: índice fuera de rango." << endl;
            cerr << "Índice: " << index << ", Tamaño del diccionario: " << dictSize << endl;
            throw runtime_error("Error en la descompresión: índice fuera de rango.");
        }

        descomprimido += str;

        if (!w.empty()) {
            dictionary[dictSize++] = w + str[0];
        }

        w = str;
    }

    return descomprimido;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Uso: " << argv[0] << " <archivo_comprimido.bin> <foto_a_crear.png>" << endl;
        return 1;
    }
    const char* archivoComprimido = argv[1];
    const char* nombreFoto = argv[2];

    vector<pair<int, char>> compressedBuffer = leer_comprimido(archivoComprimido);

    string descomprimido;
    try {
        descomprimido = descomprimir(compressedBuffer);
    } catch (const runtime_error& e) {
        cerr << e.what() << endl;
        return 1;
    }

    istringstream file(descomprimido);

    uint32_t id;
    uint8_t edadSexo;
    uint16_t edad;
    bool sexo;
    string sexoString;
    uint8_t nombreLen;
    string nombre;
    uint8_t abreviacionLen;
    string abreviacion;
    uint16_t descripcionLen;
    string descripcion;

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

    file.seekg(0, ios::end);
    int longitud = file.tellg();

    int datosAnteriores = 4 + 1 + 1 + nombreLen + 1 + abreviacionLen + 2 + descripcionLen;
    int longitudImagen = longitud - datosAnteriores;

    file.seekg(datosAnteriores, ios::beg);

    vector<char> buffer(longitudImagen);
    file.read(buffer.data(), longitudImagen);

    ofstream foto(nombreFoto, ios::binary);
    if (!foto) {
        cerr << "No se pudo abrir el archivo " << nombreFoto << endl;
        return 1;
    } else {
        foto.write(buffer.data(), longitudImagen);
        foto.close();
    }

    cout << "ID: " << id << endl;
    cout << "Edad: " << edad << endl;
    cout << "Sexo: " << sexoString << endl;
    cout << "Nombre: " << nombre << endl;
    cout << "Abreviación: " << abreviacion << endl;
    cout << "Descripción: " << descripcion << endl;

    cerr << "\nLa foto recibida de [" << archivoComprimido << "] se ha almacenado en [" << nombreFoto << "]" << endl;

    // Crear un nuevo archivo .neko para verificar lo leído
    ofstream nuevoNeko("nuevo_foto.neko", ios::binary);
    if (nuevoNeko.is_open()) {
        nuevoNeko.write(reinterpret_cast<char*>(&id), 4);
        nuevoNeko.write(reinterpret_cast<char*>(&edadSexo), 1);
        nuevoNeko.write(reinterpret_cast<char*>(&nombreLen), 1);
        nuevoNeko.write(nombre.c_str(), nombreLen);
        nuevoNeko.write(reinterpret_cast<char*>(&abreviacionLen), 1);
        nuevoNeko.write(abreviacion.c_str(), abreviacionLen);
        nuevoNeko.write(reinterpret_cast<char*>(&descripcionLen), 2);
        nuevoNeko.write(descripcion.c_str(), descripcionLen);
        nuevoNeko.write(buffer.data(), longitudImagen);
        nuevoNeko.close();
        cerr << "El archivo nuevo_foto.neko ha sido creado para verificación." << endl;
    } else {
        cerr << "No se pudo crear el archivo nuevo_foto.neko." << endl;
    }

    return 0;
}
