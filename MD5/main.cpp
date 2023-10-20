#include "md5.h"
#include <cstring>
#include <iomanip>
#include <iostream>

#include <iostream>
#include <string>
#include <string_view>
#include <sstream>

#include <iostream>
#include <fstream>
#include <cryptopp/hex.h>
#include <cryptopp/md5.h>

// Function to write to a file
void writeFile(const std::string& filename, const std::string& content) {
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << content;
        outFile.close();
    } else {
        std::cout << "Unable to open file for writing.\n";
    }
}

// Function to read from a file
std::string readFile(const std::string& filename) {
    std::ifstream inFile(filename);
    std::string content;
    if (inFile.is_open()) {
        std::getline(inFile, content, '\0');  // Read until EOF
        inFile.close();
    } else {
        std::cout << "Unable to open file for reading.\n";
    }
    return content;
}

int main()
{
    auto data = readFile("/Users/full-hat/Documents/MyProjects/FHCrypto/MD5/input.txt");

	md5::md5_t md5;
	md5.process(data.c_str(), std::strlen(data.c_str()));
	md5.finish();

	char str[MD5_STRING_SIZE];

	md5.get_string(str);

	for (unsigned int i = 0; i < MD5_STRING_SIZE; i++)
		std::cout << str[i];
}
