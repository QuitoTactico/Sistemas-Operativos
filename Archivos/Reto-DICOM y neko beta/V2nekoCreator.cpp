#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstring>

//quiero pedir estos datos: un ID de 10 digitos, un array de 5 chars llamado abreviacion, una edad que va hasta 130 máximo, un nombre de paciente de tamaño variable (máximo 64 caracteres, o sea, puedes usar todo un byte si quieres para decir qué tan largo es) y una descripción larga de tamaño variable (máximo 10000 caracteres, eso serían 2 bytes para decir qué tan largo es)

int main() {
    //char id[11];
    uint32_t id;
    std::cout << "Ingrese el ID de 10 dígitos: ";
    //std::cin.getline(id, 11);
    std::cin >> id;

    std::cin.ignore();

    char abreviacion[5];
    std::cout << "Ingrese la abreviación de 5 caracteres: ";
    //std::cin.getline(abreviacion, 6);
    std::cin.read(abreviacion, 5);

    std::cin.ignore();

    uint16_t edad;
    std::cout << "Ingrese la edad (hasta 130): ";
    std::cin >> edad;

    //std::cin.ignore(); // Ignorar el salto de línea después de leer la edad
    std::cin.ignore();

    char nombre[65];
    std::cout << "Ingrese el nombre del paciente (máximo 64 caracteres): ";
    std::cin >> nombre;
    uint8_t nombreLen = strlen(nombre);

    std::cin.ignore();

    char descripcion[10001];
    std::cout << "Ingrese la descripción larga (máximo 10000 caracteres): ";
    std::cin.getline(descripcion, 10001);

    std::ofstream file("foto.neko", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<char*>(&id), 4);
        file.write(abreviacion, 5);
        file.write(reinterpret_cast<char*>(&edad), 1);
        file.write(nombre, strlen(nombre));
        file.write(descripcion, strlen(descripcion));
        file.close();
        std::cout << "Datos guardados en el archivo." << std::endl;
    } else {
        std::cout << "No se pudo abrir el archivo." << std::endl;
    }

    return 0;
}