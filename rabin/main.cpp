#include <chrono>
#include <cstddef>
#include <ios>
#include <iostream>
#include <ratio>
#include <stdexcept>
#include <string>
#include <thread>
#include <ctime>
#include <ranges>
#include <vector>

#include "core.h"
#include "big_rand.h"
#include "file.h"
#include "utils.h"

int main()
{
    auto r = Rabin();

    {
        // Crypt data
        auto reader1 = FileReader("/Users/full-hat/Documents/MyProjects/FHCrypto/rabin/text.txt");
        auto data = reader1.ReadBlock_8();

        // Encrypt
        std::vector<uint8_t> block;
        std::vector<uint32_t> block_32;
        std::vector<uint8_t> encrypted;
        size_t real_size = 0;
        for (int i = 0; i < data.size(); ++i)
        {
            if (i == 119)
            {
                std::cout << "A";
            }
            block.push_back(data[i]);
            real_size++;

            if (block.size() == 30 * 4)
            {
                for (auto el : to_vec_32(block))
                {
                    block_32.push_back(el);
                }
                block.clear();
            }

            if (block_32.size() == 30)
            {
                auto encrypted_block = r.encrypt(block_32, real_size);
                real_size = 0;
                for (auto el : to_vec_8(encrypted_block, encrypted_block.size() * 4))
                {
                    encrypted.push_back(el);
                }
                block_32.clear();
            }
        }

        if (block.size() != 0)
        {
            auto encrypted_block = r.encrypt(to_vec_32(block), real_size);
            for (auto el : to_vec_8(encrypted_block, encrypted_block.size() * 4))
            {
                encrypted.push_back(el);
            }
        }
        WriteToFile(encrypted, "/Users/full-hat/Documents/MyProjects/FHCrypto/rabin/encrypted.txt");
    }
    
    {
        // Decrypt
        auto reader = FileReader("/Users/full-hat/Documents/MyProjects/FHCrypto/rabin/encrypted.txt");
        auto encrypted_data_file = reader.ReadBlock_8();
        auto encrypted_from_file = to_vec_32(encrypted_data_file);

        std::vector<uint8_t> decrypt_data;
        for (int i = 0; i < encrypted_from_file.size(); i += 32)
        {
            std::vector<uint32_t> block { encrypted_from_file.begin() + i, encrypted_from_file.begin() + 32 + i };
            auto decrypted = r.decrypt(block);
            auto decrypted_8 = to_vec_8(decrypted.first, decrypted.second);

            for (auto el : decrypted_8)
            {
                decrypt_data.push_back(el);
            }
        }
        WriteToFile(decrypt_data, "/Users/full-hat/Documents/MyProjects/FHCrypto/rabin/decrypted.txt");
    }

    return 0;
}
