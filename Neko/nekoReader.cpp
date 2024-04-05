#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>

using namespace std;

int main() {
    uint32_t id;
    uint8_t edad;
    uint8_t nombreLen;
    string nombre;
    uint8_t abreviacionLen;
    string abreviacion;
    uint16_t descripcionLen;
    string descripcion;

    ifstream file("foto.neko", ios::binary);
    if (file.is_open()) {
        file.read(reinterpret_cast<char*>(&id), 4);

        file.read(reinterpret_cast<char*>(&edad), 1);

        file.read(reinterpret_cast<char*>(&nombreLen), 1); //obtiene el tamaño del string (no es necesariamente el largo,
                                                           // hay caracteres que ocupan más de un byte)
        nombre.resize(nombreLen);           //le cambia el tamaño al string para que pueda guardar lo que se lee
        file.read(&nombre[0], nombreLen);   //le pasa la dirección de memoria del primer elemento del string
                                            // ya la función se encarga de llenar todas las demás direcciones de memoria

        file.read(reinterpret_cast<char*>(&abreviacionLen), 1);
        abreviacion.resize(abreviacionLen);
        file.read(&abreviacion[0], abreviacionLen);

        file.read(reinterpret_cast<char*>(&descripcionLen), 2);
        descripcion.resize(descripcionLen);
        file.read(&descripcion[0], descripcionLen);

        file.close();

        cout << "ID: " << id << endl;  //los uint16_t son tratados por cout como enteros, como acá
        //pero los uint8_t son tratados como unsigned char, así que toca asegurarse de que se imprima como entero
        cout << "Edad: " << static_cast<int>(edad) << endl;     
        cout << "Nombre: " << nombre << endl;
        cout << "Abreviación: " << abreviacion << endl;
        cout << "Descripción: " << descripcion << endl;
    } else {
        cout << "No se pudo abrir el archivo." << endl;
    }

    return 0;
}