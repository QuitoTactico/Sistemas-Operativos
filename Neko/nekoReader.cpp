#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <foto_a_crear.png>" << endl;
        return 1;
    }
    const char* nombreFoto = argv[1];

    uint32_t id;
    uint8_t edad;
    uint8_t nombreLen;
    string nombre;
    uint8_t abreviacionLen;
    string abreviacion;
    uint16_t descripcionLen;
    string descripcion;

    ifstream file("foto.neko", ios::binary);
    if (file.is_open()) {
        // ---------------------------------- LECTURA DE LOS DATOS ----------------------------------

        //ID
        file.read(reinterpret_cast<char*>(&id), 4);

        //EDAD
        file.read(reinterpret_cast<char*>(&edad), 1);

        //NOMBRE
        file.read(reinterpret_cast<char*>(&nombreLen), 1); //obtiene el tamaño del string (no es necesariamente el largo,
                                                           //hay caracteres que ocupan más de un byte)
        nombre.resize(nombreLen);           //le cambia el tamaño al string para que pueda guardar lo que se lee
        file.read(&nombre[0], nombreLen);   //le pasa la dirección de memoria del primer elemento del string
                                            //ya la función se encarga de llenar todas las demás direcciones de memoria

        //ABREVIACIÓN
        file.read(reinterpret_cast<char*>(&abreviacionLen), 1);
        abreviacion.resize(abreviacionLen);
        file.read(&abreviacion[0], abreviacionLen);

        //DESCRIPCIÓN
        file.read(reinterpret_cast<char*>(&descripcionLen), 2);
        descripcion.resize(descripcionLen);
        file.read(&descripcion[0], descripcionLen);

        // ---------------------------------- LECTURA DE LA IMAGEN ----------------------------------

        //movemos el puntero hasta el final del archivo para saber qué tan largo es
        file.seekg(0, ios::end);
        int longitud = file.tellg();

        //calculamos la longitud de la imagen (longitud de todo el archivo - longitud de los datos normales)
        int datosAnteriores = 4+1+1+nombreLen+1+abreviacionLen+2+descripcionLen;
        int longitudImagen = longitud - datosAnteriores;

        //movemos el puntero al inicio de la imagen
        file.seekg(datosAnteriores, ios::beg);

        //buffer para guardar la imagen temporalmente
        char* buffer = new char[longitudImagen];
        file.read(buffer, longitudImagen);

        //ya podemos cerrar el archivo .neko
        file.close();

        //abrimos el archivo de la imagen output en modo binario
        ofstream foto(nombreFoto, ios::binary);

        // ---------------------------------- ESCRITURA DE LA IMAGEN ----------------------------------

        //si el archivo de la imagen se abrió correctamente, guarda los datos en él
        if (!foto) {
            cerr << "No se pudo abrir el archivo " << nombreFoto << endl;
            delete[] buffer;
            return 1;
        }
        else{
            //tan fácil como escribir la imagen que guardamos en el buffer, en el archivo
            foto.write(buffer, longitudImagen);

            //y cerramos el archivo de la imagen, así como hicimos con el .neko
            foto.close();
            delete[] buffer; //también liberamos el buffer
        }

        // ---------------------------------- IMPRESIÓN DE LOS DATOS ----------------------------------

        //los uint16_t son tratados por cout como enteros, como en ID
        //pero los uint8_t como en edad son tratados como unsigned char, 
        //así que toca asegurarse de que se imprima como entero
        cout << "ID: " << id << endl;  
        cout << "Edad: " << static_cast<int>(edad) << endl;     
        cout << "Nombre: " << nombre << endl;
        cout << "Abreviación: " << abreviacion << endl;
        cout << "Descripción: " << descripcion << endl;
        /*podría imprimir la dimensión de la imagen, pero ya que se puede montar cualquier tipo de imagen 
        (de hecho, cualquier tipo de archivo), no hay forma de saber dónde se ubica la longitud de la imagen 
        en todos los tipos diferentes de headers que existen. También es trabajo innecesario, 
        lo importante de nekoReader es recibir y guardar los datos. */

        cerr << "\nLa foto recibida de [foto.neko] se ha almacenado en [" << nombreFoto << "]" << endl;
    } else {
        cout << "No se pudo abrir el archivo [foto.neko] o no existe.\nPor favor ubicar el archivo en esta misma ruta y con permisos de lectura." << endl;
        return 1;
    }

    return 0;
}