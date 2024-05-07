# pip install windows-curses
import curses

def main(stdscr):
    print('hola000')
    '''
    stdscr = curses.initscr()
    print('hola001')
    curses.cbreak()
    print('hola002')
    stdscr.keypad(True)
    print('hola003')
    '''

    print('hola')
    try:
        # Inicializar curses
        curses.curs_set(1)
        stdscr.nodelay(0)
        stdscr.timeout(-1)

        # Crear un buffer para almacenar el texto
        buffer = [""]

        print('hola2')
        # Inicializar la posición del cursor
        cursor_y, cursor_x = 0, 0

        while True:
            # Dibujar el buffer
            stdscr.clear()
            for i, line in enumerate(buffer):
                stdscr.addstr(i, 0, line)

            # Mover el cursor a su posición
            stdscr.move(cursor_y, cursor_x)

            # Actualizar la pantalla
            stdscr.refresh()

            # Esperar a que el usuario presione una tecla
            c = stdscr.getch()

            # Si el usuario presiona Ctrl+X, salir
            if c == 24:
                break
            # Si el usuario presiona Enter, añadir una nueva línea al buffer
            elif c == 10:
                buffer.insert(cursor_y, "")
                cursor_y += 1
                cursor_x = 0
            # Si el usuario presiona Backspace, eliminar el carácter anterior
            elif c == 8:
                if cursor_x > 0:
                    buffer[cursor_y] = buffer[cursor_y][:cursor_x-1] + buffer[cursor_y][cursor_x:]
                    cursor_x -= 1
                elif cursor_y > 0:
                    cursor_x = len(buffer[cursor_y-1])
                    buffer[cursor_y-1] += buffer[cursor_y]
                    del buffer[cursor_y]
                    cursor_y -= 1
            # Si el usuario presiona cualquier otra tecla, añadirla al buffer
            else:
                buffer[cursor_y] = buffer[cursor_y][:cursor_x] + chr(c) + buffer[cursor_y][cursor_x:]
                cursor_x += 1
    finally:
        print('hola3')
        # Finalizar curses
        stdscr.keypad(False)
        curses.echo()
        curses.nocbreak()
        curses.endwin()
        print('hola4')


# Iniciar el programa
curses.wrapper(main)

#main()