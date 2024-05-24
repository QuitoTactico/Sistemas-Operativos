import curses

def main(stdscr):
    # Inicializar curses
    curses.cbreak()
    stdscr.keypad(True)

    try:
        # Crear un buffer para almacenar el texto
        buffer = [""]
        cursor_y, cursor_x = 0, 0

        while True:
            stdscr.clear()
            for i, line in enumerate(buffer):
                stdscr.addstr(i, 0, line)
            stdscr.move(cursor_y, cursor_x)
            stdscr.refresh()
            c = stdscr.getch()

            if c == curses.KEY_EXIT:  # Normalmente Ctrl+C
                break
            elif c == 10:  # Enter
                buffer.append("")
                cursor_y += 1
                cursor_x = 0
            elif c == curses.KEY_BACKSPACE:
                if cursor_x > 0:
                    buffer[cursor_y] = buffer[cursor_y][:cursor_x-1] + buffer[cursor_y][cursor_x:]
                    cursor_x -= 1
                elif cursor_y > 0:
                    cursor_x = len(buffer[cursor_y-1])
                    buffer[cursor_y-1] += buffer[cursor_y]
                    del buffer[cursor_y]
                    cursor_y -= 1
            else:
                buffer[cursor_y] = buffer[cursor_y][:cursor_x] + chr(c) + buffer[cursor_y][cursor_x:]
                cursor_x += 1
    finally:
        # Finalizar curses
        stdscr.keypad(False)
        curses.echo()
        curses.nocbreak()
        curses.endwin()

curses.wrapper(main)
