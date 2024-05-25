#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include "glosario.cpp"

using namespace std;

// encriptamos con Vigenere y luego comprimimos con LZW

// #bytes:  tipo:   descripción

//      4:   int:   ID
//      1:   int:   Sexo[7] y Edad [6..0]
//      1:   int:   Largo del nombre
//      X:   str:   Nombre
//      1:   int:   Largo de la abreviación
//      X:   str:   Abreviación
//      2:   int:   Largo de la descripción
//      X:   str:   Descripción

// ------------------------------------------------------

// LZW
vector<int> comprimir(const string& texto);
void guardar_comprimido(const vector<int>& comprimido, const string& archivo);
// Vigenere
void encriptarVigenere(string& data, const string& clave);

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
    // por cómo funciona cin (lee como caracteres) toca guardarlo como variable uint16_t y convertirlo a uint8_t para guardarlo en .neko 
    // en nuestro caso, nos saltamos ese paso cuando lo obligamos a guardarse en .neko en un solo byte.
    uint16_t edad;
    cout << "Ingrese la edad: ";
    cin >> edad;
    cin.ignore();

    // SEXO
    bool sexo;
    cout << "Ingrese el sexo (0 para masculino, 1 para femenino): ";
    cin >> sexo;
    cin.ignore();

    // Almacena la edad y el sexo en un solo byte   Sexo[7] y Edad [6..0].  BITS: 76543210
    uint8_t edadSexo = (sexo << 7) | (edad & 0x7F);                    //   DIST: SEEEEEEE

    // NOMBRE
    string nombre;
    cout << "Ingrese el nombre completo del paciente: ";
    getline(cin, nombre);
    uint8_t nombreLen = nombre.size();

    // ABREVIACIÓN
    string abreviacion;
    cout << "Ingrese la abreviación (o \"ver\" para ver el glosario): ";
    getline(cin, abreviacion);
    if (abreviacion == "ver") {     // por si el usuario quiere ver el glosario de abreviaciones
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

    // buffer para almacenar todos los datos binarios
    stringstream buffer;

    // escribimos todos los datos con sus respectivos tamaños
    buffer.write(reinterpret_cast<char*>(&id), 4);
    buffer.write(reinterpret_cast<char*>(&edadSexo), 1);
    buffer.write(reinterpret_cast<char*>(&nombreLen), 1);
    buffer.write(nombre.c_str(), nombreLen);
    buffer.write(reinterpret_cast<char*>(&abreviacionLen), 1);
    buffer.write(abreviacion.c_str(), abreviacionLen);
    buffer.write(reinterpret_cast<char*>(&descripcionLen), 2);
    buffer.write(descripcion.c_str(), descripcionLen);

    // abrimos el archivo de la imagen en modo binario
    ifstream foto(nombreFoto, ios::binary);

    // comprobamos si el archivo sí se abrió bien, o si existe en primer lugar
    if (!foto) {
        cerr << nombreFoto << " no existe o no se pudo abrir." << endl;
        return 1;
    } else {
        // movemos el puntero al final del archivo
        foto.seekg(0, ios::end);

        // sacamos la longitud del archivo
        int longitud = foto.tellg();

        // regresamos el puntero al inicio del archivo
        foto.seekg(0, ios::beg);

        // creamos un buffer para almacenar la imagen temporalmente
        char* imgBuffer = new char[longitud];

        // leemos la imagen y la guardamos en el buffer
        foto.read(imgBuffer, longitud);

        // podemos cerrar el archivo de la imagen
        foto.close();

        // escribimos la imagen que fue guardada en el buffer
        buffer.write(imgBuffer, longitud);

        // liberamos el buffer
        delete[] imgBuffer;
    }

    // convertimos el stringstream a string, fué mucho más fácil usar el stringstream
    string fileContent = buffer.str();

    // ciframos el contenido del archivo
    encriptarVigenere(fileContent, clave);

    // comprimimos el contenido del archivo cifrado
    vector<int> comprimido = comprimir(fileContent);

    // guardamos el archivo comprimido
    guardar_comprimido(comprimido, nombreNeko);

    cerr << "\nEl archivo [" << nombreFoto << "] se ha procesado, cifrado y almacenado en [" << nombreNeko << "]" << endl;

    return 0;
}

// ------------------------------------------------------

// compresión con LZW
vector<int> comprimir(const string& texto) {
    unordered_map<string, int> dictionary;

    // creamos el diccionario con todo ASCII posible
    for (int i = 0; i < 256; ++i) {
        dictionary[string(1, char(i))] = i;
    }

    string w;
    vector<int> comprimido;
    int dict_size = 256;

    // recorremos el texto, si la combinación de w + c está en el diccionario, w = w + c
    // si no, guardamos el valor de w en comprimido, añadimos w + c al diccionario y w = c
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

// ------------------------------------------------------

// cifrado con Vigenere
void encriptarVigenere(string& data, const string& clave) {
    int dataLen = data.length();
    int claveLen = clave.length();

    // casi como en el César, en este en vez de sumar un constante, sumamos dependiendo de la clave
    // como la clave puede no ser del mismo largo del contenido, la repetimos y ya!
    for (int i = 0; i < dataLen; ++i) {
        data[i] = (data[i] + clave[i % claveLen]) % 256;
    }
}
