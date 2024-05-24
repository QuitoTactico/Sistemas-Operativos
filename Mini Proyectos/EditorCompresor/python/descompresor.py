def leer_comprimido(archivo):
    with open(archivo, "rb") as f:
        comprimido = []
        while True:
            bytes = f.read(2)
            if not bytes:
                break
            comprimido.append(int.from_bytes(bytes, byteorder='big'))
    return comprimido


def descomprimir(comprimido):
    dictionary = {i: chr(i) for i in range(256)}
    dict_size = 256
    w = chr(comprimido.pop(0))
    descomprimido = w

    for k in comprimido:
        entry = ""
        if k in dictionary:
            entry = dictionary[k]
        elif k == dict_size:
            entry = w + w[0]
        else:
            raise ValueError("Error en la descompresión: índice fuera de rango.")

        descomprimido += entry
        dictionary[dict_size] = w + entry[0]
        dict_size += 1

        w = entry

    return descomprimido

'''
if __name__ == "__main__":
    comprimido = leer_comprimido("miau.bin")
    descomprimido = descomprimir(comprimido)
    print("Texto descomprimido:", descomprimido)
'''