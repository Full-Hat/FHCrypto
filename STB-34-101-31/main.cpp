#include <__algorithm/ranges_for_each.h>
#include <_types/_uint8_t.h>
#include <algorithm>
#include <span>
#include <string>
#include <vector>
#include <iostream>

#include "CTR.h"
#include "file.h"

int main()
{
    std::string files[] {
        "/Users/full-hat/Documents/MyProjects/FHCrypto/STB-34-101-31/test.txt",
        "/Users/full-hat/Documents/MyProjects/FHCrypto/STB-34-101-31/decrypted.txt",
    };

    std::array<uint8_t, 32> key { "0000000000000000000000000000000" };

    std::vector<uint8_t> text { '1', '2', '3', '4', '5', '6', '7', '8', '1', '2', '3', '4', '5', '6', '7', '8' };

    STB_34_101_31_CTR crypter(key);

    FileReader reader(files[0]);

    auto encrypted = crypter.Encrypt(text);
    auto decrypted = crypter.Decrypt(encrypted);
    WriteToFile(decrypted, files[1]);

    std::ranges::for_each(decrypted, [](auto value) { std::cout << value << " "; });
}