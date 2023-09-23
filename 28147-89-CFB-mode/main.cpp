#include <array>
#include <bitset>
#include <iostream>

#include "file.h"

#include "CFB.h"
#include "ECB.h"

int main(int argc, char** argv)
{
    const auto file_to_read = "/Users/full-hat/Documents/MyProjects/FHCrypto/28147-89-CFB-mode/text.txt";
    const auto file_to_write_encrypted = "/Users/full-hat/Documents/MyProjects/FHCrypto/28147-89-CFB-mode/encrypted.txt";
    const auto file_to_write_decrypted = "/Users/full-hat/Documents/MyProjects/FHCrypto/28147-89-CFB-mode/decrypted.txt";

    std::array<char, 32> key { "MyVeryGoodKeyForEncryptionSomeS" };

    FileReader file(file_to_read);
    const auto file_data = file.ReadBlock_64();
    
    GOST_28147_89_CFB cfb(key);
    auto encrypted = cfb.Encrypt(file_data.first, 0);
    auto decrypted = cfb.Decrypt(encrypted, 0);

    WriteToFile(encrypted, file_to_write_encrypted, file_data.second);
    WriteToFile(decrypted, file_to_write_decrypted, file_data.second);

    return 0;
}