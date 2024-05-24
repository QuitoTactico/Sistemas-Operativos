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
    // Llamamos a la función de compresión
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

    // ID
    uint32_t id;
    cout << "Ingrese el ID: ";
    cin >> id;
    cin.ignore();

    // EDAD
    uint16_t edad;
    cout << "Ingrese la edad: ";
    cin >> edad;
    cin.ignore();

    // SEXO
    bool sexo;
    cout << "Ingrese el sexo (0 para masculino, 1 para femenino): ";
    cin >> sexo;
    cin.ignore();

    // Almacena la edad y el sexo en un solo byte. Sexo[7] y Edad [6..0]. BITS: 76543210
    uint8_t edadSexo = (sexo << 7) | (edad & 0x7F); // DIST: SEEEEEEE

    // NOMBRE
    string nombre;
    cout << "Ingrese el nombre completo del paciente: ";
    getline(cin, nombre);
    uint8_t nombreLen = nombre.size();

    // ABREVIACIÓN
    string abreviacion;
    cout << "Ingrese la abreviación (o \"ver\" para ver el glosario): ";
    getline(cin, abreviacion);
    if (abreviacion == "ver") {
        cout << GLOSARIO << endl;
        cout << "Ingrese la abreviación: ";
        getline(cin, abreviacion);
    }
    uint8_t abreviacionLen = abreviacion.size();

    // DESCRIPCIÓN
    string descripcion;
    cout << "Ingrese una descripción más detallada (máximo 65536 caracteres): ";
    getline(cin, descripcion);
    uint16_t descripcionLen = descripcion.size();

    // ---------------------------------- ESCRITURA DE LOS DATOS ----------------------------------

    // Abrimos el archivo .neko en modo binario (es llamado foto.neko por default)
    ofstream file("foto.neko", ios::binary);
    if (file.is_open()) {
        // Escribimos todos los datos con sus respectivos tamaños
        file.write(reinterpret_cast<char*>(&id), 4); // 4: int: ID
        file.write(reinterpret_cast<char*>(&edadSexo), 1); // 1: int: Sexo[7] y Edad [6..0]. SEEEEEEE
        file.write(reinterpret_cast<char*>(&nombreLen), 1); // 1: int: Largo del nombre
        file.write(nombre.c_str(), nombreLen); // X: str: Nombre
        file.write(reinterpret_cast<char*>(&abreviacionLen), 1); // 1: int: Largo de la abreviación
        file.write(abreviacion.c_str(), abreviacionLen); // X: str: Abreviación
        file.write(reinterpret_cast<char*>(&descripcionLen), 2); // 2: int: Largo de la descripción
        file.write(descripcion.c_str(), descripcionLen); // X: str: Descripción

        // Abrimos el archivo de la imagen en modo binario
        ifstream foto(nombreFoto, ios::binary);

        // ---------------------------------- ESCRITURA DE LA IMAGEN ----------------------------------

        // Comprobamos si el archivo se abrió correctamente
        if (!foto) {
            cerr << nombreFoto << " no existe o no se pudo abrir." << endl;
            return 1;
        } else {
            // Movemos el puntero al final del archivo
            foto.seekg(0, ios::end);

            // Sacamos la longitud del archivo
            int longitud = foto.tellg();

            // Regresamos el puntero al inicio del archivo
            foto.seekg(0, ios::beg);

            // Creamos un buffer para almacenar la imagen temporalmente
            vector<char> buffer(longitud);

            // Leemos la imagen y la guardamos en el buffer
            foto.read(buffer.data(), longitud);

            // Podemos cerrar el archivo de la imagen
            foto.close();

            // Escribimos la imagen que fue guardada en el buffer, en el archivo .neko
            file.write(buffer.data(), longitud);
        }

        // Éxito para los exitosos
        file.close();
        cerr << "\nEl archivo [" << nombreFoto << "] se ha procesado y almacenado en [foto.neko]" << endl;

        // Leer el archivo .neko en un buffer
        ifstream fileIn("foto.neko", ios::binary);
        string buffer((istreambuf_iterator<char>(fileIn)), istreambuf_iterator<char>());

        // Comprimimos y guardamos el buffer en un archivo .neko2
        guardar_comprimido(buffer, "foto.neko2");

    } else {
        // Fracaso para los fracasados...
        cout << "No se pudo abrir el archivo [foto.neko]." << endl;
    }

    return 0;
}
