#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
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

    //NOMBRE
    string nombre;
    cout << "Ingrese el nombre del paciente: ";
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
        file.write(reinterpret_cast<char*>(&edad), 1);              // 1: int: Sexo[7] y Edad [6..0]
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
    } else {
        //fracaso para los fracasados...
        cout << "No se pudo abrir el archivo [foto.neko]." << endl;
    }

    return 0;
}