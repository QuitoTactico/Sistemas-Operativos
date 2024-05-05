// VigenereCipher.h
#ifndef VIGENERE_CIPHER_H
#define VIGENERE_CIPHER_H

#include <string>

class VigenereCipher {
public:
    VigenereCipher(const std::string& key);
    std::string encrypt(const std::string& text);
    std::string decrypt(const std::string& text);

private:
    std::string key;
    static char shiftChar(char c, int shift);
};

#endif