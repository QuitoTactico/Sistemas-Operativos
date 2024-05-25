#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include "glosario.cpp"

using namespace std;

// Función de compresión usando LZ78
vector<pair<int, char>> comprimir(const string& buffer) {
    vector<pair<int, char>> compressed;
    unordered_map<string, int> dictionary;
    int dictSize = 256;

    string w;
    for (char c : buffer) {
        string wc = w + c;
        if (dictionary.find(wc) != dictionary.end()) {
            w = wc;
        } else {
            if (!w.empty()) {
                compressed.push_back(make_pair(dictionary[w], c));
            } else {
                compressed.push_back(make_pair(0, c));
            }
            dictionary[wc] = dictSize++;
            w = "";
        }
    }

    if (!w.empty()) {
        compressed.push_back(make_pair(dictionary[w], '\0'));
    }

    return compressed;
}

// Función para guardar el texto comprimido en un archivo binario
void guardar_comprimido(const string& buffer, const string& archivo) {
    vector<pair<int, char>> compressedBuffer = comprimir(buffer);

    ofstream outfile(archivo, ios::binary);
    if (outfile.is_open()) {
        for (const auto& entry : compressedBuffer) {
            outfile.write(reinterpret_cast<const char*>(&entry.first), sizeof(int));
            outfile.write(&entry.second, sizeof(char));
        }
        outfile.close();
        cout << "Archivo guardado de forma comprimida." << endl;
    } else {
        cerr << "Error al abrir el archivo para escritura." << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <foto_a_ingresar.png>" << endl;
        return 1;
    }
    const char* nombreFoto = argv[1];

    // ---------------------------------- TOMA DE LOS DATOS ----------------------------------

    uint32_t id;
    cout << "Ingrese el ID: ";
    cin >> id;
    cin.ignore();

    uint16_t edad;
    cout << "Ingrese la edad: ";
    cin >> edad;
    cin.ignore();

    bool sexo;
    cout << "Ingrese el sexo (0 para masculino, 1 para femenino): ";
    cin >> sexo;
    cin.ignore();

    uint8_t edadSexo = (sexo << 7) | (edad & 0x7F);

    string nombre;
    cout << "Ingrese el nombre completo del paciente: ";
    getline(cin, nombre);
    uint8_t nombreLen = nombre.size();

    string abreviacion;
    cout << "Ingrese la abreviación (o \"ver\" para ver el glosario): ";
    getline(cin, abreviacion);
    if (abreviacion == "ver") {
        cout << GLOSARIO << endl;
        cout << "Ingrese la abreviación: ";
        getline(cin, abreviacion);
    }
    uint8_t abreviacionLen = abreviacion.size();

    string descripcion;
    cout << "Ingrese una descripción más detallada (máximo 65536 caracteres): ";
    getline(cin, descripcion);
    uint16_t descripcionLen = descripcion.size();

    ofstream file("foto.neko", ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<char*>(&id), 4);
        file.write(reinterpret_cast<char*>(&edadSexo), 1);
        file.write(reinterpret_cast<char*>(&nombreLen), 1);
        file.write(nombre.c_str(), nombreLen);
        file.write(reinterpret_cast<char*>(&abreviacionLen), 1);
        file.write(abreviacion.c_str(), abreviacionLen);
        file.write(reinterpret_cast<char*>(&descripcionLen), 2);
        file.write(descripcion.c_str(), descripcionLen);

        ifstream foto(nombreFoto, ios::binary);
        if (!foto) {
            cerr << nombreFoto << " no existe o no se pudo abrir." << endl;
            return 1;
        } else {
            foto.seekg(0, ios::end);
            int longitud = foto.tellg();
            foto.seekg(0, ios::beg);

            vector<char> buffer(longitud);
            foto.read(buffer.data(), longitud);
            foto.close();

            file.write(buffer.data(), longitud);
        }

        file.close();
        cerr << "\nEl archivo [" << nombreFoto << "] se ha procesado y almacenado en [foto.neko]" << endl;

        ifstream fileIn("foto.neko", ios::binary);
        string buffer((istreambuf_iterator<char>(fileIn)), istreambuf_iterator<char>());

        guardar_comprimido(buffer, "foto.neko2");

    } else {
        cout << "No se pudo abrir el archivo [foto.neko]." << endl;
    }

    return 0;
}
