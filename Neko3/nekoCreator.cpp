#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

using namespace std;

// Funciones de compresión
vector<int> comprimir(const string& texto);
void guardar_comprimido(const vector<int>& comprimido, const string& archivo);

// Función de cifrado Vigenere
void cifrarVigenere(string& data, const string& clave);

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Uso: " << argv[0] << " <archivo_neko.neko> <foto_a_ingresar.png> <clave>" << endl;
        return 1;
    }
    const char* nombreNeko = argv[1];
    const char* nombreFoto = argv[2];
    string clave = argv[3];

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

    // Buffer para almacenar todos los datos binarios
    stringstream buffer;

    // Escribimos todos los datos con sus respectivos tamaños
    buffer.write(reinterpret_cast<char*>(&id), 4);
    buffer.write(reinterpret_cast<char*>(&edadSexo), 1);
    buffer.write(reinterpret_cast<char*>(&nombreLen), 1);
    buffer.write(nombre.c_str(), nombreLen);
    buffer.write(reinterpret_cast<char*>(&abreviacionLen), 1);
    buffer.write(abreviacion.c_str(), abreviacionLen);
    buffer.write(reinterpret_cast<char*>(&descripcionLen), 2);
    buffer.write(descripcion.c_str(), descripcionLen);

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
        char* imgBuffer = new char[longitud];

        // Leemos la imagen y la guardamos en el buffer
        foto.read(imgBuffer, longitud);

        // Podemos cerrar el archivo de la imagen
        foto.close();

        // Escribimos la imagen que fue guardada en el buffer
        buffer.write(imgBuffer, longitud);

        // Liberamos el buffer
        delete[] imgBuffer;
    }

    // Convertimos el stringstream a string
    string fileContent = buffer.str();

    // Cifrar el contenido del archivo
    cifrarVigenere(fileContent, clave);

    // Comprimir el contenido del archivo cifrado
    vector<int> comprimido = comprimir(fileContent);

    // Guardar el archivo comprimido
    guardar_comprimido(comprimido, nombreNeko);

    cerr << "\nEl archivo [" << nombreFoto << "] se ha procesado, cifrado y almacenado en [" << nombreNeko << "]" << endl;

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

// Función de cifrado Vigenere
void cifrarVigenere(string& data, const string& clave) {
    int dataLen = data.length(), claveLen = clave.length();
    for (int i = 0; i < dataLen; ++i) {
        data[i] = (data[i] + clave[i % claveLen]) % 256;
    }
}
