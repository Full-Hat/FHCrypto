#include <iostream>
#include <ostream>
#include <random>
#include <sstream>
#include <string>

inline void WriteMessage(std::ostream &stream)
{
    stream << "Hello, world!" << std::endl;
    stream << "Andhiagain!" << std::endl;
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(5, 50);
    std::string message(distribution(generator), '!');
    stream << message << std::endl;
}
