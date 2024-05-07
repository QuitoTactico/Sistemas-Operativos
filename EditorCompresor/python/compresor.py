def comprimir(texto):
    dictionary = {chr(i): i for i in range(256)}
    dict_size = 256
    w = ""
    comprimido = []

    for c in texto:
        wc = w + c
        if wc in dictionary:
            w = wc
        else:
            comprimido.append(dictionary[w])
            dictionary[wc] = dict_size
            dict_size += 1
            w = c

    if w:
        comprimido.append(dictionary[w])

    return comprimido


def guardar_comprimido(texto, archivo):
    comprimido = comprimir(texto)
    with open(archivo, "wb") as f:
        for entry in comprimido:
            f.write(entry.to_bytes(2, byteorder='big'))

    print("Archivo guardado de forma comprimida.")

'''
if __name__ == "__main__":
    texto = "Eclesiastes 33 de eclesiastes digo yo, el yo que soy, el yo que fui, el yo que seré."

    guardar_comprimido(texto, "miau.bin")
'''
