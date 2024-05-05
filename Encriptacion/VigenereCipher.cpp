// VigenereCipher.cpp
#include "VigenereCipher.h"
#include <cctype>
#include <stdexcept>

VigenereCipher::VigenereCipher(const std::string& key) {
    if (key.empty()) {
        throw std::invalid_argument("Key cannot be empty.");
    }
    this->key = key;  // Acepta cualquier carácter en la clave.
}

std::string VigenereCipher::encrypt(const std::string& text) {
    std::string output;
    output.reserve(text.size());  // Reserva espacio para la salida

    for (size_t i = 0, j = 0; i < text.size(); ++i) {
        char plain_char = text[i];
        char key_char = key[j % key.size()];
        char encrypted_char = static_cast<char>(((unsigned char)plain_char + (unsigned char)key_char) % 256);
        output.push_back(encrypted_char);
        j++;
    }
    return output;
}

std::string VigenereCipher::decrypt(const std::string& text) {
    std::string output;
    output.reserve(text.size());  // Reserva espacio para la salida

    for (size_t i = 0, j = 0; i < text.size(); ++i) {
        char encrypted_char = text[i];
        char key_char = key[j % key.size()];
        char decrypted_char = static_cast<char>(((unsigned char)encrypted_char - (unsigned char)key_char + 256) % 256);
        output.push_back(decrypted_char);
        j++;
    }
    return output;
}



/*
// VigenereCipher.cpp
#include "VigenereCipher.h"
#include <cctype>
#include <stdexcept>  // Include for std::invalid_argument

VigenereCipher::VigenereCipher(const std::string& key) {
    for (char c : key) {
        if (!isalpha(c)) {
            throw std::invalid_argument("Key must be alphabetic only.");
        }
        this->key += toupper(c);
    }
}

std::string VigenereCipher::encrypt(const std::string& text) {
    std::string output;
    for (size_t i = 0, j = 0; i < text.size(); ++i) {
        if (isalpha(text[i])) {
            char c = shiftChar(toupper(text[i]), key[j] - 'A');
            output += c;
            j = (j + 1) % key.size();
        } else {
            output += text[i];
        }
    }
    return output;
}

std::string VigenereCipher::decrypt(const std::string& text) {
    std::string output;
    for (size_t i = 0, j = 0; i < text.size(); ++i) {
        if (isalpha(text[i])) {
            char c = shiftChar(toupper(text[i]), 'Z' - key[j] + 1);
            output += c;
            j = (j + 1) % key.size();
        } else {
            output += text[i];
        }
    }
    return output;
}

char VigenereCipher::shiftChar(char c, int shift) {
    return 'A' + (c - 'A' + shift + 26) % 26;
}
*/