#include <ncurses.h> //sudo apt-get install libncurses5-dev libncursesw5-dev
#include "editor.h"

void editar_buffer(WINDOW* win, std::string& buffer) {
    int ch;
    int row = 0;
    int col = 0;
    int actual = 0;
    bool running = true;

    while (running) {
        wclear(win);
        mvwprintw(win, 0, 0, "%s", buffer.c_str()); // Imprime el buffer actual
        wmove(win, row, col);  // Mueve el cursor a la posición actual

        ch = wgetch(win);
        switch (ch) {
            case 24:  // Ctrl-X para guardar y salir
                {
                    char archivo[80];
                    echo();
                    mvwprintw(win, row + 1, 0, "Nombre del archivo a guardar: ");
                    wrefresh(win);
                    wgetstr(win, archivo);
                    noecho();
                    guardar_comprimido(comprimir(buffer), archivo);
                }
                
                running = false;
                break;

            case KEY_BACKSPACE:
            case 127:
                if (actual > 0) {
                    buffer.erase(actual - 1, 1);
                    actual--;
                    if (col > 0) {
                        col--;
                    } else {
                        row--;
                        col = buffer.find_last_of('\n', actual) + 1;
                    }
                }
                break;
            case '\n':
                buffer.insert(actual, 1, '\n');
                row++;
                col = 0;
                actual++;
                break;
            default:
                buffer.insert(buffer.begin() + actual, ch);
                actual++;
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
        std::vector<int> comprimido = leer_comprimido(archivo);
        buffer = descomprimir(comprimido);
    }

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    editar_buffer(stdscr, buffer);

    endwin();
    return 0;
}
