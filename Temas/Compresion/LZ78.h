#ifndef LZ78_H
#define LZ78_H

#include <string>

class LZ78 {
public:
    void comprimir(const std::string& archivo_fuente, const std::string& archivo_comprimido);
    void descomprimir(const std::string& archivo_comprimido, const std::string& archivo_descomprimido);
};

#endif // LZ78_H
