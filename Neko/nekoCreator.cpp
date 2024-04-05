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

int main() {
    uint32_t id;
    cout << "Ingrese el ID: ";
    cin >> id;
    cin.ignore();

    uint8_t edad;
    cout << "Ingrese la edad: ";
    cin >> edad;
    cin.ignore();

    string nombre;
    cout << "Ingrese el nombre del paciente: ";
    getline(cin, nombre);
    uint8_t nombreLen = nombre.size();

    string abreviacion;
    cout << "Ingrese la abreviación: ";
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
        file.close();
        cout << "Datos guardados en el archivo." << endl;
    } else {
        cout << "No se pudo abrir el archivo." << endl;
    }

    return 0;
}