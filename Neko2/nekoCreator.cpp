#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

// Funciones de compresión
vector<int> comprimir(const string& texto);
void guardar_comprimido(const vector<int>& comprimido, const string& archivo);

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

    // Almacena la edad y el sexo en un solo byte
    uint8_t edadSexo = (sexo << 7) | (edad & 0x7F);

    // NOMBRE
    string nombre;
    cout << "Ingrese el nombre completo del paciente: ";
    getline(cin, nombre);
    uint8_t nombreLen = nombre.size();

    // ABREVIACIÓN
    string abreviacion;
    cout << "Ingrese la abreviación: ";
    getline(cin, abreviacion);
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
        file.write(reinterpret_cast<char*>(&id), 4);
        file.write(reinterpret_cast<char*>(&edadSexo), 1);
        file.write(reinterpret_cast<char*>(&nombreLen), 1);
        file.write(nombre.c_str(), nombreLen);
        file.write(reinterpret_cast<char*>(&abreviacionLen), 1);
        file.write(abreviacion.c_str(), abreviacionLen);
        file.write(reinterpret_cast<char*>(&descripcionLen), 2);
        file.write(descripcion.c_str(), descripcionLen);

        // Abrimos el archivo de la imagen en modo binario
        ifstream foto(nombreFoto, ios::binary);

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
            char* buffer = new char[longitud];

            // Leemos la imagen y la guardamos en el buffer
            foto.read(buffer, longitud);

            // Podemos cerrar el archivo de la imagen
            foto.close();

            // Escribimos la imagen que fue guardada en el buffer, en el archivo .neko
            file.write(buffer, longitud);

            // Liberamos el buffer
            delete[] buffer;
        }

        // Cerramos el archivo .neko
        file.close();

        // Leer el contenido del archivo binario para comprimirlo
        ifstream fileRead("foto.neko", ios::binary);
        string fileContent((istreambuf_iterator<char>(fileRead)), istreambuf_iterator<char>());
        fileRead.close();

        // Comprimir el contenido del archivo
        vector<int> comprimido = comprimir(fileContent);

        // Guardar el archivo comprimido
        guardar_comprimido(comprimido, "foto_comprimido.neko");

        cerr << "\nEl archivo [" << nombreFoto << "] se ha procesado y almacenado en [foto_comprimido.neko]" << endl;
    } else {
        cerr << "No se pudo abrir el archivo [foto.neko]." << endl;
    }

    return 0;
}

// Funciones de compresión
vector<int> comprimir(const string& texto) {
    unordered_map<string, int> dictionary;
    for (int i = 0; i < 256; ++i) {
        dictionary[string(1, char(i))] = i;
    }

    string w;
    vector<int> comprimido;
    int dict_size = 256;

    for (char c : texto) {
        string wc = w + c;
        if (dictionary.count(wc)) {
            w = wc;
        } else {
            comprimido.push_back(dictionary[w]);
            dictionary[wc] = dict_size++;
            w = string(1, c);
        }
    }

    if (!w.empty()) {
        comprimido.push_back(dictionary[w]);
    }

    return comprimido;
}

void guardar_comprimido(const vector<int>& comprimido, const string& archivo) {
    ofstream output(archivo, ios::binary);
    for (int numero : comprimido) {
        output.put(char((numero >> 8) & 0xFF));
        output.put(char(numero & 0xFF));
    }
    output.close();
}
