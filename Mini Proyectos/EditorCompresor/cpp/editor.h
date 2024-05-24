#ifndef EDITOR_H
#define EDITOR_H

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <fstream>


// DESCOMPRESOR
std::vector<int> leer_comprimido(const std::string& archivo);
std::string descomprimir(const std::vector<int>& comprimido);


// COMPRESOR
void mostrarDiccionario(const std::unordered_map<std::string, int>& dictionary);
std::vector<int> comprimir(const std::string& texto);
void guardar_comprimido(const std::vector<int>& comprimido, const std::string& archivo);


#endif // EDITOR_H