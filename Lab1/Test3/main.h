#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <stdexcept>

inline void calculate_and_print(std::istream &in, std::ostream &out)
{
    double width, height;
    std::string widthInput, heightInput;

    std::getline(in, widthInput);
    std::istringstream widthStream(widthInput);
    if (!(widthStream >> width) || !widthStream.eof()) {
        std::cout << widthStream.str();
        throw std::invalid_argument("Invalid width");
    }

    std::getline(in, heightInput);
    std::istringstream heightStream(heightInput);
    if (!(heightStream >> height) || !heightStream.eof()) {
        throw std::invalid_argument("Invalid height");
    }

    out << "Area of a square: " << (width * height);
}