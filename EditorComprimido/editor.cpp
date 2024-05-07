#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <unordered_map>
#include <sstream>

// prototipos de las funciones definidas en descompresor.cpp
std::string descomprimir(const std::vector<std::pair<int, char>>& compressed);
std::vector<std::pair<int, char>> leer_comprimido();

// función principal que permite editar el buffer
void editar_buffer(std::string& buffer) {
    std::vector<std::string> lines;
    std::string input;
    int cursorRow = 0;
    int cursorCol = 0;
    bool running = true;

    // dividimos el buffer en líneas
    std::istringstream iss(buffer);
    std::string line;
    while (std::getline(iss, line)) {
        lines.push_back(line);
    }

    // bucle principal
    while (running) {
        // imprimimos el buffer
        for (int i = 0; i < lines.size(); ++i) {
            if (i == cursorRow) {
                std::cout << "> ";
            } else {
                std::cout << "  ";
            }
            std::cout << lines[i] << std::endl;
        }

        // mostramos mensaje de instrucciones
        std::cout << "Comandos: Ctrl+S (Guardar), Ctrl+X (Salir)" << std::endl;

        // leemos entrada del usuario
        std::getline(std::cin, input);

        // procesamos entrada del usuario
        if (input.size() > 0 && input[0] == '\x1b') { // control character (escape sequence)
            if (input.size() > 1 && input[1] == 'S') { // Ctrl+S para guardar
                // llamamos a la función para guardar el buffer
                buffer = descomprimir(leer_comprimido());
            } else if (input.size() > 1 && input[1] == 'X') { // Ctrl+X para salir
                running = false;
            }
        } else { // editamos el texto
            if (input == "UP") {
                if (cursorRow > 0) {
                    --cursorRow;
                }
            } else if (input == "DOWN") {
                if (cursorRow < lines.size() - 1) {
                    ++cursorRow;
                }
            } else if (input == "LEFT") {
                if (cursorCol > 0) {
                    --cursorCol;
                }
            } else if (input == "RIGHT") {
                if (cursorCol < lines[cursorRow].size()) {
                    ++cursorCol;
                }
            } else if (input == "INSERT") {
                lines[cursorRow].insert(cursorCol, " ");
                ++cursorCol;
            } else if (input == "DELETE") {
                if (cursorCol < lines[cursorRow].size()) {
                    lines[cursorRow].erase(cursorCol, 1);
                }
            } else if (input == "NEWLINE") {
                lines.insert(lines.begin() + cursorRow + 1, lines[cursorRow].substr(cursorCol));
                lines[cursorRow] = lines[cursorRow].substr(0, cursorCol);
                ++cursorRow;
                cursorCol = 0;
            } else if (!input.empty() && std::isprint(input[0])) {
                lines[cursorRow].insert(cursorCol, input);
                ++cursorCol;
            }
        }

        // limpiamos la pantalla
        std::cout << "\x1b[2J";
        std::cout << "\x1b[H";
    }

    // reconstruimos el buffer
    buffer.clear();
    for (const std::string& line : lines) {
        buffer += line + "\n";
    }
}

int main() {
    std::string buffer;

    std::cout << "¿Desea abrir un archivo existente (S/N)? ";
    char opcion;
    std::cin >> opcion;

    if (std::toupper(opcion) == 'S') {
        // llamamos a la función para descomprimir el archivo
        buffer = descomprimir(leer_comprimido());
    }

    // llamamos a la función para editar el buffer
    editar_buffer(buffer);

    return 0;
}
