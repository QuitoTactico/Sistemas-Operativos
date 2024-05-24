#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include "glosario.cpp"

using namespace std;

// #bytes: tipo: descripción

// 4: int: ID
// 1: int: Sexo[7] y Edad [6..0]
// 1: int: Largo del nombre
// X: str: Nombre
// 1: int: Largo de la abreviación
// X: str: Abreviación
// 2: int: Largo de la descripción
// X: str: Descripción


// función de compresión usando LZ98
std::vector<std::pair<int, char>> comprimir(const std::string& buffer) {
    std::vector<std::pair<int, char>> compressed;
    std::unordered_map<std::string, int> dictionary;
    int dictSize = 256;

    std::string w;
    for (char c : buffer) {
        std::string wc = w + c;
        if (dictionary.find(wc) != dictionary.end()) {
            w = wc;
        } else {
            if (!w.empty()) {
                compressed.push_back(std::make_pair(dictionary[w], c));
            }
            dictionary[wc] = dictSize++;
            w = std::string(1, c);
        }
    }

    if (!w.empty()) {
        compressed.push_back(std::make_pair(dictionary[w], '\0'));
    }

    return compressed;
}

// función para guardar el texto comprimido en un archivo binario
void guardar_comprimido(const std::string& buffer, const std::string& archivo) {
    // llamamos a la función de compresión
    std::vector<std::pair<int, char>> compressedBuffer = comprimir(buffer);

    std::ofstream outfile(archivo, std::ios::binary);
    if (outfile.is_open()) {
        for (const auto& entry : compressedBuffer) {
            outfile.write(reinterpret_cast<const char*>(&entry.first), sizeof(int));
            outfile.write(&entry.second, sizeof(char));
        }
        outfile.close();
        std::cout << "Archivo guardado de forma comprimida." << std::endl;
    } else {
        std::cerr << "Error al abrir el archivo para escritura." << std::endl;
    }
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <foto_a_ingresar.png>" << endl;
        return 1;
    }
    const char* nombreFoto = argv[1];

    // ---------------------------------- TOMA DE LOS DATOS ----------------------------------

    //ID
    uint32_t id;
    cout << "Ingrese el ID: ";
    cin >> id;
    cin.ignore();

    //EDAD  
    //por cómo funciona cin (lee como caracteres) toca guardarlo como variable uint16_t y convertirlo a uint8_t para guardarlo en .neko 
    //en nuestro caso, nos saltamos ese paso cuando lo obligamos a guardarse en .neko en un solo byte.
    uint16_t edad;
    cout << "Ingrese la edad: ";
    cin >> edad;
    cin.ignore();

    //SEXO
    bool sexo;
    cout << "Ingrese el sexo (0 para masculino, 1 para femenino): ";
    cin >> sexo;
    cin.ignore();

    //almacena la edad y el sexo en un solo byte. Sexo[7] y Edad [6..0].  BITS: 76543210
    uint8_t edadSexo = (sexo << 7) | (edad & 0x7F);                  //   DIST: SEEEEEEE

    //NOMBRE
    string nombre;
    cout << "Ingrese el nombre completo del paciente: ";
    getline(cin, nombre);
    uint8_t nombreLen = nombre.size();

    //ABREVIACIÓN
    string abreviacion;
    cout << "Ingrese la abreviación (o \"ver\" para ver el glosario): ";
    getline(cin, abreviacion);
    if (abreviacion == "ver") {     //por si el usuario quiere ver el glosario de abreviaciones
        cout << GLOSARIO << endl;
        cout << "Ingrese la abreviación: ";
        getline(cin, abreviacion);
    }
    uint8_t abreviacionLen = abreviacion.size();

    //DESCRIPCIÓN
    string descripcion;
    cout << "Ingrese una descripción más detallada (máximo 65536 caracteres): ";
    getline(cin, descripcion);
    uint16_t descripcionLen = descripcion.size();

    // ---------------------------------- ESCRITURA DE LOS DATOS ----------------------------------

    //abrimos el archivo .neko en modo binario (es llamado foto.neko por default)
    ofstream file("foto.neko", ios::binary);
    if (file.is_open()) {  
        //escribimos todos los datos con sus respectivos tamaños    //bytes: tipo: descripción
        file.write(reinterpret_cast<char*>(&id), 4);                // 4: int: ID
        file.write(reinterpret_cast<char*>(&edadSexo), 1);          // 1: int: Sexo[7] y Edad [6..0]. SEEEEEEE
        file.write(reinterpret_cast<char*>(&nombreLen), 1);         // 1: int: Largo del nombre
        file.write(nombre.c_str(), nombreLen);                      // X: str: Nombre
        file.write(reinterpret_cast<char*>(&abreviacionLen), 1);    // 1: int: Largo de la abreviación
        file.write(abreviacion.c_str(), abreviacionLen);            // X: str: Abreviación
        file.write(reinterpret_cast<char*>(&descripcionLen), 2);    // 2: int: Largo de la descripción
        file.write(descripcion.c_str(), descripcionLen);            // X: str: Descripción

        //abrimos el archivo de la imagen en modo binario
        ifstream foto(nombreFoto, ios::binary);

        // ---------------------------------- ESCRITURA DE LA IMAGEN ----------------------------------

        //comprobamos si el archivo se abrió correctamente
        if (!foto) {
            cerr << nombreFoto << " no existe o no se pudo abrir." << endl;
            return 1;
        }
        else{
            //movemos el puntero al final del archivo
            foto.seekg(0, ios::end);

            //sacamos la longitud del archivo
            int longitud = foto.tellg();

            //regresamos el puntero al inicio del archivo
            foto.seekg(0, ios::beg);

            //creamos un buffer para almacenar la imagen temporalmente
            char* buffer = new char[longitud];

            //leemos la imagen y la guardamos en el buffer
            foto.read(buffer, longitud);

            //podemos cerrar el archivo de la imagen
            foto.close();

            //escribimos la imagen que fué guardada en el buffer, en el archivo .neko
            file.write(buffer, longitud);

            //liberamos el buffer
            delete[] buffer;
        }

        //éxito para los exitosos
        file.close();
        cerr << "\nEl archivo [" << nombreFoto << "] se ha procesado y almacenado en [foto.neko]" << endl;

        // Leer el archivo .neko en un buffer
        std::ifstream fileIn("foto.neko", std::ios::binary);
        std::string buffer((std::istreambuf_iterator<char>(fileIn)), std::istreambuf_iterator<char>());

        // comprimimos y guardamos el buffer en un archivo .neko2
        guardar_comprimido(buffer, "foto.neko2");

    } else {
        //fracaso para los fracasados...
        cout << "No se pudo abrir el archivo [foto.neko]." << endl;
    }

    return 0;
}