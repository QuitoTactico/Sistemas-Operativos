#include <ncurses.h> //sudo apt-get install libncurses5-dev libncursesw5-dev
#include "editor.h"

void guardar_comprimido(const std::string& buffer, const std::string& archivo) {
    std::vector<int> comprimido = comprimir(buffer);
    std::ofstream output(archivo, std::ios::binary);
    for (int numero : comprimido) {
        output.put(char((numero >> 8) & 0xFF));
        output.put(char(numero & 0xFF));
    }
    output.close();
}

std::string leer_comprimido(const std::string& archivo) {
    std::ifstream input(archivo, std::ios::binary);
    std::vector<int> comprimido;
    char bytes[2];
    while (input.read(bytes, 2)) {
        int numero = (unsigned char)(bytes[0]) << 8 | (unsigned char)(bytes[1]);
        comprimido.push_back(numero);
    }
    input.close();
    return descomprimir(comprimido);
}

void editar_buffer(WINDOW* win, std::string& buffer) {
    int ch;
    int row = 0;
    int col = 0;
    bool running = true;

    while (running) {
        wclear(win);
        mvwprintw(win, 0, 0, buffer.c_str());  // Imprime el buffer actual
        wmove(win, row, col);  // Mueve el cursor a la posición actual

        ch = wgetch(win);
        switch (ch) {
            case 24:  // Ctrl-X para salir
                running = false;
                break;
            case 19:  // Ctrl-S para guardar
                {
                    char archivo[80];
                    echo();
                    mvwgetstr(win, 1, 0, archivo);
                    noecho();
                    guardar_comprimido(buffer, archivo);
                }
                break;
            case KEY_BACKSPACE:
            case 127:
                if (col > 0) {
                    buffer.erase(col - 1, 1);
                    col--;
                }
                break;
            case '\n':
                buffer.insert(col, 1, '\n');
                col = 0;
                row++;
                break;
            default:
                buffer.insert(buffer.begin() + col, ch);
                col++;
                break;
        }
    }
}

int main() {
    std::string buffer;
    char opcion;

    std::cout << "¿Desea abrir un archivo existente (S/N)? ";
    std::cin >> opcion;

    if (opcion == 'S' || opcion == 's') {
        char archivo[80];
        std::cout << "Nombre del archivo a abrir: ";
        std::cin >> archivo;
        buffer = leer_comprimido(archivo);
    }

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    editar_buffer(stdscr, buffer);

    endwin();
    return 0;
}
