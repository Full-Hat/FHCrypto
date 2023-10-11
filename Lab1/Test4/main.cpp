#include <iostream>
#include <fstream>

#include "main.h"

int main()
{
    int rows;
    std::cout << "Enter the number of rows: ";
    std::cin >> rows;

    if (!std::cin) { 
        throw std::invalid_argument("Invalid input. Please enter a number.");
    }

    std::string html = generateTable(rows);
    std::ofstream file("index.html");
    if (!file) {
        throw std::runtime_error("Unable to open file.");
    }
    file << html;
    file.close();

    std::string command = "open index.html";
    int result = system(command.c_str());
    if (result != 0) {
        throw std::runtime_error("Failed to open file in browser.");
    }

    return 0;
}