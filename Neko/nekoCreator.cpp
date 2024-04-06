#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
using namespace std;

// 4: int: ID
// 1: int: Edad
// 1: int: Largo del nombre
// X: str: Nombre
// 1: int: Largo de la abreviación
// X: str: Abreviación
// 2: int: Largo de la descripción
// X: str: Descripción

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <foto_a_ingresar.png>" << endl;
        return 1;
    }
    const char* nombreFoto = argv[1];


    uint32_t id;
    cout << "Ingrese el ID: ";
    cin >> id;
    cin.ignore();

    // Por cómo funciona cin (lee como caracteres) toca guardarlo como variable uint16_t y convertirlo a uint8_t para guardarlo en .neko 
    // En nuestro caso, nos saltamos ese paso cuando lo obligamos a guardarse en .neko en un solo byte.
    uint16_t edad;
    cout << "Ingrese la edad: ";
    cin >> edad;
    cin.ignore();

    string nombre;
    cout << "Ingrese el nombre del paciente: ";
    getline(cin, nombre);
    uint8_t nombreLen = nombre.size();

    string abreviacion;
    cout << "Ingrese la abreviación (ver GLOSARIOINCMNSZ.pdf): ";
    getline(cin, abreviacion);
    uint8_t abreviacionLen = abreviacion.size();

    string descripcion;
    cout << "Ingrese una descripción más detallada (máximo 65536 caracteres): ";
    getline(cin, descripcion);
    uint16_t descripcionLen = descripcion.size();

    ofstream file("foto.neko", ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<char*>(&id), 4);                // 4: int: ID
        file.write(reinterpret_cast<char*>(&edad), 1);              // 1: int: Edad
        file.write(reinterpret_cast<char*>(&nombreLen), 1);         // 1: int: Largo del nombre
        file.write(nombre.c_str(), nombreLen);                      // X: str: Nombre
        file.write(reinterpret_cast<char*>(&abreviacionLen), 1);    // 1: int: Largo de la abreviación
        file.write(abreviacion.c_str(), abreviacionLen);            // X: str: Abreviación
        file.write(reinterpret_cast<char*>(&descripcionLen), 2);    // 2: int: Largo de la descripción
        file.write(descripcion.c_str(), descripcionLen);            // X: str: Descripción

        // Abre el archivo de la imagen en modo binario
        ifstream foto(nombreFoto, ios::binary);

        /*
        // Comprueba si el archivo se abrió correctamente
        if (!neko) {
            cerr << "No se pudo abrir el archivo neko_input.neko" << endl;
            return 1;
        }*/

        // Comprueba si el archivo se abrió correctamente
        if (!foto) {
            cerr << nombreFoto << " no existe o no se pudo abrir." << endl;
            return 1;
        }
        else{
            // Mueve el puntero al final del archivo
            foto.seekg(0, ios::end);

            // Obtiene la longitud del archivo
            int longitud = foto.tellg();

            // Mueve el puntero de nuevo al inicio del archivo
            foto.seekg(0, ios::beg);

            // Crea un buffer para almacenar la imagen
            char* buffer = new char[longitud];

            // Lee la imagen en el buffer
            foto.read(buffer, longitud);

            // Cierra el archivo de la imagen
            foto.close();

            // Escribe la imagen en el archivo .neko
            file.write(buffer, longitud);

            // Libera el buffer
            delete[] buffer;
        }

        file.close();
        cerr << "\nEl archivo [" << nombreFoto << "] se ha procesado y almacenado en [foto.neko]" << endl;
    } else {
        cout << "No se pudo abrir el archivo." << endl;
    }

    return 0;
}