from compresor import comprimir, guardar_comprimido
from descompresor import leer_comprimido, descomprimir


def editar_buffer(buffer):
    lines = buffer.split("\n")
    cursorRow = 0
    cursorCol = 0
    running = True

    while running:
        for i, line in enumerate(lines):
            if i == cursorRow:
                print("> ", end="")
            else:
                print("  ", end="")
            print(line)

        input_usuario = input()
        print("Comandos: Ctrl+S (Guardar), Ctrl+X (Salir)")

        if input_usuario == "\x1bS":  # Ctrl+S para guardar
            archivo = input("Nombre del archivo a guardar: ")
            guardar_comprimido(buffer, archivo)
        elif input_usuario == "\x1bX":  # Ctrl+X para salir
            running = False
        else:
            print("Input no reconocido. Use Ctrl+S para guardar y Ctrl+X para salir.")

    buffer = "\n".join(lines)


if __name__ == "__main__":
    buffer = ""
    opcion = input("¿Desea abrir un archivo existente (S/N)? ")
    if opcion.upper() == "S":
        archivo = input("Nombre del archivo a abrir: ")
        buffer = descomprimir(leer_comprimido(archivo))

    editar_buffer(buffer)
