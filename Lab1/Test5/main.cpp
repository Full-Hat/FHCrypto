#include <iostream>
#include <stdexcept>

#include "main.h"

int main(int argc, char *argv[])
{
    if (argc != 3 || std::any_of(argv[1], argv[1] + std::strlen(argv[1]), [](char c) { return c == '<' || c == '>' || c == '|' || c == '*' || c == '?'; }) ||
        std::any_of(argv[2], argv[2] + std::strlen(argv[2]), [](char c) { return c == '<' || c == '>' || c == '|' || c == '*' || c == '?'; })) {
        throw std::invalid_argument("Bad extension");
    }

    for (auto el : FindFiles(argv[1], argv[2]))
    {
        std::cout << el << std::endl;
    }
}
