#include <iostream>
#include <fstream>
#include <cstdint>
#include <stdio.h>

// http://incmnsz.mx:8080/intranet/downloads/uploads/2014/02/GLOSARIOINCMNSZ.pdf 

int main() {
    char* character = new char[3];
    std::cin.read(character, 3);
    //std::cin >> character;

    uint16_t number = 9143;
    std::string text = "Hello, world!";

    std::ofstream file("foto.neko", std::ios::binary);
    if (file.is_open()) {
        //file.write(reinterpret_cast<char*>(&character), sizeof(character));
        file.write(character, 3);
        file.write(reinterpret_cast<char*>(&number), 2);
        file.write(text.c_str(), text.length());
        file.close();
        std::cout << "Data saved to file." << std::endl;
    } else {
        std::cout << "Unable to open file." << std::endl;
    }

    std::cin.get(character, 3);

    return 0;
}